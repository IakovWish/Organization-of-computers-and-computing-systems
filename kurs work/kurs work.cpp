#include <iostream>
#include <Windows.h>

using namespace std;

#define clk_UP 20
#define clk_DOWN 40

#define frame_UP 60
#define frame_DOWN 80

#define ad_UP 100
#define ad_MIDL 110
#define ad_DOWN 120

#define cbe_UP 140
#define cbe_MIDL 150
#define cbe_DOWN 160

#define irdy_UP 180
#define irdy_DOWN 200

#define trdy_UP 220
#define trdy_DOWN 240

#define devsel_UP 260
#define devsel_DOWN 280

int CLK;
int Xclk;
int CLK_flag;

int Xframe;
int FRAME_flag;

int Xad;
int AD_flag;

int Xcbe;
int CBE_flag;

int Xirdy;
int IRDY_flag;

int Xtrdy;
int TRDY_flag;

int Xdevsel;
int DEVSEL_flag;

int WORD_value;
int WORD_cnt;

int cnt;
int cnt1;

int flag;

void Line(HDC, int, int, int, int, int r = 0, int g = 0, int b = 0);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Создаём прототип функции окна, которая будет определена ниже

char szProgName[] = "ЭВМ тест"; // объявляем строку-имя программы

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hWnd;
	MSG lpMsg;
	WNDCLASS w; // создаём экземпляр структуры WNDCLASS

	/*И начинаем её заполнять*/
	w.lpszClassName = szProgName; //имя программы - объявлено выше
	w.hInstance = hInstance; //идентификатор текущего приложения
	w.lpfnWndProc = WndProc; //указатель на функцию окна
	w.hCursor = LoadCursor(NULL, IDC_ARROW); //загружаем курсор
	w.hIcon = 0; //иконки у нас не будет пока
	w.lpszMenuName = 0; //и меню пока не будет
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //цвет фона окна
	w.style = CS_HREDRAW | CS_VREDRAW; //стиль - перерисовываемое по х и по у
	w.cbClsExtra = 0;
	w.cbWndExtra = 0;

	/*Если не удалось зарегистрировать класс окна - выходим*/
	if (!RegisterClass(&w))
	{
		return 0;
	}

	/*Создадим окно в памяти, заполнив аргументы CreateWindow*/
	hWnd = CreateWindow(szProgName, //Имя программы
		"ЭВМ", //Заголовок окна
		WS_OVERLAPPEDWINDOW, //Стиль окна - перекрывающееся
		0, //положение окна на экране по х
		0, //положение по у
		1500, //ширина
		800, //высота
		(HWND)NULL, //идентификатор родительского окна
		(HMENU)NULL, //идентификатор меню
		(HINSTANCE)hInstance, //идентификатор экземпляра программы
		(HINSTANCE)NULL); //отсутствие дополнительных параметров

	ShowWindow(hWnd, nCmdShow); // Выводим окно из памяти на экран
	UpdateWindow(hWnd); // Обновим содержимое окна

	/*Цикл обработки сообщений*/
	while (GetMessage(&lpMsg, NULL, 0, 0)) // Получаем сообщение из очереди
	{
		TranslateMessage(&lpMsg); // Преобразует сообщения клавиш в символы
		DispatchMessage(&lpMsg); // Передаёт сообщение соответствующей функции окна
	}
	return(lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) // Функция окна
{
	HDC hdc; //создаём контекст устройства
	PAINTSTRUCT ps; //создаём экземпляр структуры графического вывода

	//Цикл обработки сообщений
	switch (messg)
	{
	case WM_PAINT: // сообщение рисования

		WORD_value = 5;
		WORD_cnt = -1;

		cnt = 1;
		cnt1 = 1;

		flag = 0;

		hdc = BeginPaint(hWnd, &ps); // начинаем рисовать

		HPEN hPen; //Объявляется кисть
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255)); //Создаётся объект
		SelectObject(hdc, hPen); //Объект делается текущим

		//MoveToEx(hdc, 0, 200, NULL); //сделать текущими координаты x1, y1
		CLK_flag = clk_UP;
		FRAME_flag = frame_UP;
		IRDY_flag = irdy_UP;
		TRDY_flag = trdy_UP;
		DEVSEL_flag = devsel_UP;

		for (CLK = 0, Xclk = 0, Xframe = 0, Xad = 0, Xcbe = 0, Xirdy = 0, Xtrdy = 0, Xdevsel = 0; CLK < 25; CLK++)
		{
			MoveToEx(hdc, Xclk, clk_UP, NULL);
			LineTo(hdc, Xclk, clk_DOWN);
			Xclk += 15;
			LineTo(hdc, Xclk, clk_DOWN);
			LineTo(hdc, Xclk, clk_UP);
			Xclk += 15;
			LineTo(hdc, Xclk, clk_UP);

			MoveToEx(hdc, Xframe, FRAME_flag, NULL); //сделать текущими координаты x1, y1

			if (Xclk > 30 && WORD_cnt == -1)
			{
				FRAME_flag = frame_DOWN;
				LineTo(hdc, Xframe, FRAME_flag);
			}
			else if (WORD_cnt == WORD_value - 1)
			{
				FRAME_flag = frame_UP;
				LineTo(hdc, Xframe, FRAME_flag);
			}

			Xframe += 30;
			LineTo(hdc, Xframe, FRAME_flag);


			MoveToEx(hdc, Xad, ad_MIDL, NULL);

			if (FRAME_flag == frame_DOWN && WORD_cnt == -1)
			{
				Xad += 10;
				LineTo(hdc, Xad, ad_UP);
				Xad += 10;
				LineTo(hdc, Xad, ad_UP);
				Xad += 10;
				LineTo(hdc, Xad, ad_MIDL);
				Xad -= 10;
				LineTo(hdc, Xad, ad_DOWN);
				Xad -= 10;
				LineTo(hdc, Xad, ad_DOWN);
				Xad -= 10;
				LineTo(hdc, Xad, ad_MIDL);

				Xad += 30;
				WORD_cnt = 0;
			}
			else if (WORD_cnt >= 0 && WORD_cnt < WORD_value)
			{
				if (cnt == 4)
				{
					Xad += 10;
					LineTo(hdc, Xad, ad_UP);
					Xad += 100;
					LineTo(hdc, Xad, ad_UP);
					Xad += 10;
					LineTo(hdc, Xad, ad_MIDL);
					Xad -= 10;
					LineTo(hdc, Xad, ad_DOWN);
					Xad -= 100;
					LineTo(hdc, Xad, ad_DOWN);
					Xad -= 10;
					LineTo(hdc, Xad, ad_MIDL);

					Xad += 120;

					WORD_cnt++;
					cnt = 0;
				}
				cnt++;
			}
			else
			{
				Xad += 30;
				LineTo(hdc, Xad, ad_MIDL);
			}


			MoveToEx(hdc, Xcbe, cbe_MIDL, NULL);

			if (FRAME_flag == frame_DOWN && flag == 0)
			{
				Xcbe += 10;
				LineTo(hdc, Xcbe, cbe_UP);
				Xcbe += 10;
				LineTo(hdc, Xcbe, cbe_UP);
				Xcbe += 10;
				LineTo(hdc, Xcbe, cbe_MIDL);
				Xcbe -= 10;
				LineTo(hdc, Xcbe, cbe_DOWN);
				Xcbe -= 10;
				LineTo(hdc, Xcbe, cbe_DOWN);
				Xcbe -= 10;
				LineTo(hdc, Xcbe, cbe_MIDL);

				Xcbe += 30;
				flag = 1;
			}
			else if (flag == 1)
			{

				if (WORD_cnt == WORD_value)
				{
					Xcbe += 10;
					LineTo(hdc, Xcbe, cbe_UP);
					Xcbe += 580;
					LineTo(hdc, Xcbe, cbe_UP);
					Xcbe += 10;
					LineTo(hdc, Xcbe, cbe_MIDL);
					Xcbe -= 10;
					LineTo(hdc, Xcbe, cbe_DOWN);
					Xcbe -= 580;
					LineTo(hdc, Xcbe, cbe_DOWN);
					Xcbe -= 10;
					LineTo(hdc, Xcbe, cbe_MIDL);

					Xcbe += 600;
					flag = 2;
				}
			}
			else
			{
				Xcbe += 30;
				LineTo(hdc, Xcbe, cbe_MIDL);
			}



			MoveToEx(hdc, Xirdy, IRDY_flag, NULL);

			Xirdy += 30;
			LineTo(hdc, Xirdy, IRDY_flag);

			if (flag == 1)
			{
				IRDY_flag = irdy_DOWN;
				LineTo(hdc, Xirdy, IRDY_flag);
			}
			else if (WORD_cnt == WORD_value)
			{
				IRDY_flag = irdy_UP;
				LineTo(hdc, Xirdy, IRDY_flag);
			}

			MoveToEx(hdc, Xtrdy, TRDY_flag, NULL);

			Xtrdy += 30;
			LineTo(hdc, Xtrdy, TRDY_flag);

			if (cnt == 4)
			{
				TRDY_flag = trdy_DOWN;
				LineTo(hdc, Xtrdy, TRDY_flag);
			}
			else if (TRDY_flag == trdy_DOWN)
			{
				TRDY_flag = trdy_UP;
				LineTo(hdc, Xtrdy, TRDY_flag);
			}


			MoveToEx(hdc, Xdevsel, DEVSEL_flag, NULL);

			Xdevsel += 30;
			LineTo(hdc, Xdevsel, DEVSEL_flag);

			if (flag == 1)
			{
				DEVSEL_flag = devsel_DOWN;
				LineTo(hdc, Xdevsel, DEVSEL_flag);
			}
			else if (WORD_cnt == WORD_value)
			{
				DEVSEL_flag = devsel_UP;
				LineTo(hdc, Xdevsel, DEVSEL_flag);
			}
		}

		DeleteObject(hPen);

		EndPaint(hWnd, &ps); // заканчиваем рисовать
		break;

	case WM_KEYDOWN: // Обработка нажатия клавиши

		break;

	case WM_DESTROY: // сообщение выхода - разрушение окна
		PostQuitMessage(0); // Посылаем сообщение выхода с кодом 0 - нормальное завершение
		break;

	default:
		return(DefWindowProc(hWnd, messg, wParam, lParam)); //освобождаем очередь приложения от нераспознаных
	}
	return 0;
}

void Line(HDC hdc, int x1, int y1, int x2, int y2, int r, int g, int b) // обычная линия
{
	HPEN hPen; //Объявляется кисть
	hPen = CreatePen(PS_SOLID, 1, RGB(r, g, b)); //Создаётся объект
	SelectObject(hdc, hPen); //Объект делается текущим

	MoveToEx(hdc, x1, y1, NULL); //сделать текущими координаты x1, y1
	LineTo(hdc, x2, y2);
	DeleteObject(hPen);
}