/* FILE: MAIN.C
 * PROGRAMMER: AS3
 * LAST UPDATE: 09.06.2015
 * PURPOSE: output a cow and other animation.
*/
#pragma warning(disable: 4244) 

#include "vec.h"
#include "anim.h"
#include "units.h"

/* Имя класса окна */
#define WND_CLASS_NAME "My window class"

INT AS3_MouseWheel;

/* Ссылка вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* Главная функция программы.
 *   - дескриптор экземпляра приложения:
 *       HINSTANCE hInstance;
 *   - дескриптор предыдущего экземпляра приложения
 *     (не используется и должно быть NULL):
 *       HINSTANCE hPrevInstance;
 *   - командная строка:
 *       CHAR *CmdLine;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) код возврата в операционную систему.
 *   0 - при успехе.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* HINSTANCE hIns = LoadLibrary("shell32"); */

  /* Регистрация класса окна */
  wc.style = CS_VREDRAW | CS_HREDRAW; /* Стиль окна: полностью перерисовывать
                                       * при изменении вертикального или
                                       * горизонтального размеров
                                       * еще можно CS_DBLCLKS для добавления
                                       * отработки двойного нажатия */
  wc.cbClsExtra = 0; /* Дополнительное количество байт для класса */
  wc.cbWndExtra = 0; /* Дополнительное количество байт для окна */
  wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
  wc.hCursor = LoadCursor(NULL, IDC_HAND); /* Загрузка курсора (системного) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK); /* Загрузка пиктограммы (системной) */
  wc.hInstance = hInstance; /* Дескриптор приложения, регистрирующего класс */
  wc.lpszMenuName = NULL; /* Имя ресурса меню */
  wc.lpfnWndProc = MyWindowFunc; /* Указатель на функцию обработки */
  wc.lpszClassName = WND_CLASS_NAME;

  /* Регистрация класса в системе */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* Создание окна */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* Имя класса окна */
      "Title",                      /* Заголовок окна */
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,          /* Стили окна - окно общего вида */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Позиция окна (x, y) - по умолчанию */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Размеры окна (w, h) - по умолчанию */
      NULL,                         /* Дескриптор родительского окна */
      NULL,                         /* Дескриптор загруженного меню */
      hInstance,                    /* Дескриптор приложения */
      NULL);                        /* Указатель на дополнительные параметры */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  
  AS3_AnimAddUnit(AS3_UnitModelCreate());
  AS3_AnimAddUnit(AS3_UnitControlCreate());

  /* Запуск цикла сообщений окна */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    /* Обработка сообщений от клавиатуры */
    TranslateMessage(&msg);
    /* Передача сообщений в функцию окна */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

/* Функция обработки сообщения окна.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 *   - номер сообщения (см. WM_***):
 *       UINT Msg;
 *   - параметр сообшения ('word parameter'):
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter'):
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  CREATESTRUCT *cs;
  PAINTSTRUCT ps;
  static HBITMAP hBm;
  static HDC hMemDC; 
  static INT w, h; 

  switch (Msg)
  {
  case WM_CREATE:    
    cs = (CREATESTRUCT *)lParam;     
    SetTimer(hWnd, 111, 50, NULL); 
    AS3_AnimInit(hWnd); 
    return 0;

  case WM_SIZE:   
    AS3_AnimResize(LOWORD(lParam), HIWORD(lParam));
    AS3_AnimRender();  
    return 0;

  case WM_TIMER:    
    AS3_AnimRender();
    AS3_AnimCopyFrame();
    return 0; 

  case WM_MOUSEWHEEL:
    AS3_MouseWheel += (SHORT)HIWORD(wParam) / WHEEL_DELTA;
    return 0;

  case WM_ERASEBKGND:    
    return 1;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    AS3_AnimCopyFrame();
    return 0;     

  case WM_DESTROY:
    AS3_AnimClose();   
    KillTimer(hWnd, 111);
    PostQuitMessage(0);
    return 0;

  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* End of 'MAIN.C' file. */
