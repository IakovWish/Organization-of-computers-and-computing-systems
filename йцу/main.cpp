#include <iostream>

using namespace std;

int main()
{
    /*В массиве из 10 чисел поделить все отрицательные элементы на максимальный элемент массива*/
    int MASS[10];
    int* mass = MASS;

    __asm
    {
        mov esi, mass // в esi теперь адресс массива MASS
        mov eax, 10 // так как в массиве 10 элементов
        mov edx, eax // записываем 10 в edx
        xor ecx, ecx // обнуляем счетчик
        xor eax, eax // и eax
        mov ebx, -5
        
        start : // заполнение текущего элемента массива
        cmp ecx, edx // сравниваем счетчик с количеством элементов

        je End // если они равны, значит мы прошли все элементы

        mov[esi], ebx // если нет, то кладем новый элемент по адрессу
        inc ebx
        add esi, 4 // так как у нас int, увеличиваем адресс на 4
        inc ecx // элемент заполнен, счетчик увеличиваем
        jmp start // возвращаемся чтобы заполнить следующий элемент

        End : // все элементы заполнены
    }

    for (int i = 0; i < 10; i++)
    {
        cout << mass[i] << " ";
    }
    cout << endl;

    __asm
    {
        mov esi, mass
        add esi, 40

        xor ecx, ecx

        sub esi, 4

        mov ebx, [esi]

        start1:
        inc ecx
        cmp ebx, [esi]
        JNL end1
        JNGE new_max

        new_max :
        mov ebx, [esi]
        jmp end1

        end1 :
        sub esi, 4
        cmp ecx, 10
        je end2
        jmp start1

        end2 :
        add esi, 4
        xor ecx, ecx

        start2:
        inc ecx
        cmp [esi], 0
        JNGE divis
        jmp end3

        divis:
        xor edx, edx
        mov eax, [esi]
        cdq // расширяем делимое (EAX) до EDX:EAX
        idiv ebx
        mov [esi], edx

        end3:
        add esi, 4
        cmp ecx, 10
        JNE start2
    }

    for (int i = 0; i < 10; i++)
    {
        cout << mass[i] << " ";
    }

    return 0;
}
