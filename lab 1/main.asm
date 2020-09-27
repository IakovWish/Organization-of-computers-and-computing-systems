.586
.MODEL FLAT, stdcall
include console.inc

.DATA
consoleOutHandle DD ? ; дескриптор выходного буфера
consoleInHandle DD ? ; дескриптор входного буфера
numBytes DD ?
IN_STR Db "Введите массив из 10 чисел через пробел", 10, 0
space dw 32 ; пробел
TITL Db "В массиве из 10 чисел переставить элементы в зеркальном порядке.", 0
n dw 10 ; \n
SymCount Dw 10 dup(?) ; строка с количеством символов
CRD COORD <?> ; структура координат
MASS DD 10 dup (?) ; массив

.CODE
START proc

	; образовать консоль, вначале освободить уже существующую
	INVOKE FreeConsole
	INVOKE AllocConsole

	; получить дескрипторы
	INVOKE GetStdHandle, STD_INPUT_HANDLE
	MOV consoleInHandle, EAX
	INVOKE GetStdHandle, STD_OUTPUT_HANDLE
	MOV consoleOutHandle, EAX

	; задать заголовок окна консоли
	INVOKE CharToOem, OFFSET TITL, OFFSET TITL
	INVOKE SetConsoleTitle, OFFSET TITL

	; задать размер окна консоли
	MOV CRD.X, 80
	MOV CRD.Y, 25
	mov eax, CRD
	INVOKE SetConsoleScreenBufferSize, consoleOutHandle, EAX

	INVOKE FillConsoleOutputAttribute, consoleOutHandle, BACKGROUND_RED + BACKGROUND_GREEN, 3000, 0, offset numBytes ; задать цвет окна консоли

	; установить позицию курсора
	MOV CRD.X,0
	MOV CRD.Y,0
	mov eax, CRD
	INVOKE SetConsoleCursorPosition, consoleOutHandle, EAX

	INVOKE SetConsoleTextAttribute, consoleOutHandle, FOREGROUND_BLUE + BACKGROUND_GREEN ; задать цветовые атрибуты выводимого текста

	INVOKE PrintStr, offset IN_STR, consoleOutHandle ; Вывод сообщения Введите массив:

	;ЗАПОЛНЯЕМ МАССИВ

	lea EDI, MASS ; берем адресс массива
	sub edi, 4 ; потом прибавим
	xor ebx, ebx ; обнуляем ebx, так как будем использовать его как счетчик
	sub ebx, 1 ; потом прибавим

@spacer:
	xor esi, esi ; обнуляем признак нового числа
	add edi, 4 ; переходим к заполнению следующего элемента
	add ebx, 1 ; сдвигаем ebx на 1
	cmp ebx, 10 ; сравниваем с 10, 
	je @exit ; если равно, то на выход

@NewSymb: ; возвращаемся сюда, для чтения нового символа

	INVOKE ReadSymbol, consoleInHandle, consoleOutHandle, 0 ; читаем символ в eax
	cmp eax, 32 ; если это пробел, то
	je @spacer ; пропускаем его

	cmp eax, 45 ; если не поймали '-'
	JNE @standart ; идем как обычно
	mov eax, -1 ; если поймали, то заменяем на единицу на -1
	mov [EDI], eax ; и кладем в массив
	jmp @NewSymb ; смотрим следующий символ

@standart: ; классика
	sub eax, 48 ; так как ввод был ASCII
	cmp esi, 0 ; если это первая цифра числа
	jne @NotOtr ; если нет уходим
	mov ecx, [edi] ; если текущий элемент массива
	cmp ecx, -1 ; равен -1
	jne @NotOtr ; если нет уходим
	mov ecx, [EDI] ; вынимаем этот -1 и 
	imul ecx ; умножаем на считаную цифру
@NotOtr:
	cmp esi, 0 ; смотрим какая это цифра в числе
	je @Not1Symb ; если это первая цифра, то переходим
	mov ecx, [EDI] ; если это не первая цифра, то прошлое число сохраняем
	mov esi, eax ; считанное число тоже сохраняем
	mov eax, 10 ; запомнили 10
	imul ecx ; умножили прошлое число на 10
	cmp eax, 0 ; если текущее число отрицательное
	JNGE @Otrz ; уходим
	add eax, esi ; сложили прошлое x10 со считанным
	jmp @Not1Symb ; продолжаем
@Otrz: ; если текущее число отрицательное
	mov ecx, eax ; запомнили число
	mov eax, -1 ; положили -1
	imul esi ; умножили считанное число на -1
	add eax, ecx ; сложили два отрицательных числа
@Not1Symb:
	mov esi, 1 ; подняли флаг
	mov [EDI], eax ; и положили обратно в массив уже со знаком число
	jmp @NewSymb ; возвращаемся для чтения нового символа

@exit:

	INVOKE PrintStr, offset n, consoleOutHandle ; массив заполнен ставим \n

	;зеркалим массив

	mov edx, 36 ; разность между первым и вторым элементом
	lea EDI, MASS ; снова взяли адрес первого

start1: ; возвращаемся если есть пара
	mov ebx, [edi] ; кладем крайний слева элемент в ebx
	add edi, edx ; добегаем до крайнего справа элемента
	mov eax, [edi] ; кладем крайний справа элемент в eax
	mov [edi], ebx ; сразу кладем в крайний справа элемент из ebx (крайний левый)
	sub edi, edx ; возврвщаемся к крайнему слева элементу
	mov [edi], eax ; кладем в него крайний правый элемент
	sub edx, 8 ; уменьшаем edx на 8, так как 2 элемента по 4 мы уже заполнили
	add edi, 4 ; двигаем edi на 4 чтобы использовать следующую пару
	cmp edx, 4 ; пока edx не стал равен 4м
	JNL start1 ; возвращаемся и заполняем следующую пару

	xor esi, esi ; обнуляем esi (смещение)
@start2: ; возвращаемся сюда пока не выведем весь массив
	lea EDI, MASS ; берем адресс первого
	add edi, esi ; прибавляем смещение
	INVOKE IntToStr, [EDI], offset SymCount ; переводим в стр
	INVOKE PrintStr, offset SymCount, consoleOutHandle ; выводим
	INVOKE PrintStr, offset space, consoleOutHandle ; пробел
	add esi, 4 ; увеличиваем смещение на 4
	cmp esi, 40 ; пока смещение не станет равным 40
	jne @start2 ; будем возвращаться

	INVOKE ReadSymbol, consoleInHandle, consoleOutHandle, 1 ; чтобы не закрывалась консоль
	INVOKE ExitProcess, 0 ; конец веселья
	RET
	START ENDP
END START