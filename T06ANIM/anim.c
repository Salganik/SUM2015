/* FILENAME: ANIM.C
 * PROGRAMMER: AS3
 * PURPOSE: Animation system module.
 * LAST UPDATE: 08.06.2015
 */

#include "anim.h"
#include <mmsystem.h>
#include "vec.h"
#pragma comment(lib, "winmm")

/* Системный контекст анимации */
as3ANIM AS3_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Сохраненные мышиные координаты */
static INT
  AS3_MouseOldX, AS3_MouseOldY;

/* Глобальная переменная - счетчик прокрутки колеса мыши */
INT AS3_MouseWheel;

/* Получение значения оси джойстика */
#define AS3_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)
MATR AS3_RndMatrProj = AS3_UNIT_MATR;
/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;
  POINT pt;

  memset(&AS3_Anim, 0, sizeof(as3ANIM));
  /* Инициализируем буфер кадра */
  AS3_Anim.hWnd = hWnd;
  AS3_Anim.hDC = CreateCompatibleDC(hDC);
  AS3_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(AS3_Anim.hDC, AS3_Anim.hBmFrame);
  AS3_Anim.W = 30;
  AS3_Anim.H = 30;
  AS3_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* таймер */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  AS3_Anim.IsPause = FALSE;
  FrameCounter = 0;

  /* Инициализация ввода */
  GetCursorPos(&pt);
  ScreenToClient(AS3_Anim.hWnd, &pt);
  AS3_MouseOldX = pt.x;
  AS3_MouseOldY = pt.y;
  GetKeyboardState(AS3_Anim.KeysOld);
}

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < AS3_Anim.NumOfUnits; i++)
  {
    AS3_Anim.Units[i]->Close(AS3_Anim.Units[i], &AS3_Anim);
    free(AS3_Anim.Units[i]);
    AS3_Anim.Units[i] = NULL;
  }

  /* Удаляем объекты GDI */
  DeleteObject(AS3_Anim.hBmFrame);
  DeleteDC(AS3_Anim.hDC);
} /* End of 'AS3_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(AS3_Anim.hWnd);

  /* Перевыделение памяти у буфера кадра */
  DeleteObject(AS3_Anim.hBmFrame);
  AS3_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(AS3_Anim.hDC, AS3_Anim.hBmFrame);

  /* Сохранение размера */
  AS3_Anim.W = W;
  AS3_Anim.H = H;

  ReleaseDC(AS3_Anim.hWnd, hDC);

  /* корректируем отношение сторон вывода */
  if (W > H)
    AS3_RndWp = (DBL)W / H * 3, AS3_RndHp = 3;
  else
    AS3_RndWp = 3, AS3_RndHp = (DBL)H / W * 3;
  AS3_RndMatrProj = MatrFrustum(-AS3_RndWp / 2, AS3_RndWp / 2, -AS3_RndHp / 2, AS3_RndHp / 2, AS3_RndProjDist, 800);

} /* End of 'AS3_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  /*** Обновление таймера ***/
  QueryPerformanceCounter(&li);
  AS3_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  AS3_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  if (!AS3_Anim.IsPause)
    AS3_Anim.DeltaTime = AS3_Anim.GlobalDeltaTime;
  else
  {
    TimePause += li.QuadPart - TimeOld;
    AS3_Anim.DeltaTime = 0;
  }

  AS3_Anim.Time = (DBL)(li.QuadPart - TimePause - TimeStart) / TimeFreq;

  /* вычисляем FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    AS3_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  FrameCounter++;

  /*** Обновление ввода ***/

  /* Клавиатура */
  GetKeyboardState(AS3_Anim.Keys);
  for (i = 0; i < 256; i++)
    AS3_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    AS3_Anim.KeysClick[i] = AS3_Anim.Keys[i] && !AS3_Anim.KeysOld[i];
  memcpy(AS3_Anim.KeysOld, AS3_Anim.Keys, sizeof(AS3_Anim.KeysOld));

  /* Мышь */
  /* колесо */
  AS3_Anim.MsWheel = AS3_MouseWheel;
  AS3_MouseWheel = 0;
  /* абсолютная позиция */
  GetCursorPos(&pt);
  ScreenToClient(AS3_Anim.hWnd, &pt);
  AS3_Anim.MsX = pt.x;
  AS3_Anim.MsY = pt.y;
  /* относительное перемещение */
  AS3_Anim.MsDeltaX = pt.x - AS3_MouseOldX;
  AS3_Anim.MsDeltaY = pt.y - AS3_MouseOldY;
  AS3_MouseOldX = pt.x;
  AS3_MouseOldY = pt.y;

  /* Джойстик */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID1, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(AS3_Anim.JButsOld, AS3_Anim.JButs, sizeof(AS3_Anim.JButs));
        for (i = 0; i < 32; i++)
          AS3_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          AS3_Anim.JButsClick[i] = AS3_Anim.JButs[i] && !AS3_Anim.JButsOld[i];

        /* Оси */
        AS3_Anim.JX = AS3_GET_AXIS_VALUE(X);
        AS3_Anim.JY = AS3_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          AS3_Anim.JZ = AS3_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASU)
          AS3_Anim.JU = AS3_GET_AXIS_VALUE(U);
        if (jc.wCaps & JOYCAPS_HASV)
          AS3_Anim.JV = AS3_GET_AXIS_VALUE(V);
        if (jc.wCaps & JOYCAPS_HASR)
          AS3_Anim.JR = AS3_GET_AXIS_VALUE(R);

        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            AS3_Anim.JPOV = 0;
          else
            AS3_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  /* опрос на изменение состояний объектов */
  for (i = 0; i < AS3_Anim.NumOfUnits; i++)
    AS3_Anim.Units[i]->Response(AS3_Anim.Units[i], &AS3_Anim);

  /* очистка фона */
  SelectObject(AS3_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(AS3_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(AS3_Anim.hDC, RGB(0, 0, 0));
  Rectangle(AS3_Anim.hDC, 0, 0, AS3_Anim.W + 1, AS3_Anim.H + 1);

  /* рисование объектов */
  for (i = 0; i < AS3_Anim.NumOfUnits; i++)
  {
    SelectObject(AS3_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AS3_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(AS3_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(AS3_Anim.hDC, RGB(0, 0, 0));

    AS3_RndMatrWorld = MatrIdentity();

    AS3_Anim.Units[i]->Render(AS3_Anim.Units[i], &AS3_Anim);
  }
} /* End of 'AS3_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* вывод кадра */
  hDC = GetDC(AS3_Anim.hWnd);
  BitBlt(hDC, 0, 0, AS3_Anim.W, AS3_Anim.H, AS3_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(AS3_Anim.hWnd, hDC);
} /* End of 'AS3_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       as3UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimAddUnit( as3UNIT *Unit )
{
  if (AS3_Anim.NumOfUnits < AS3_MAX_UNITS)
  {
    AS3_Anim.Units[AS3_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &AS3_Anim);
  }
} /* End of 'AS3_AnimAddUnit' function */


/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(AS3_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(AS3_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(AS3_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(AS3_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* восстанавливаем размер окна */
    SetWindowPos(AS3_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'AS3_AnimFlipFullScreen' function */

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimDoExit( VOID )
{
  PostMessage(AS3_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'AS3_AnimDoExit' function */

/* Функция установки паузы.
 * АРГУМЕНТЫ:
 *   - флаг установки паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimSetPause( BOOL NewPauseFlag )
{
  AS3_Anim.IsPause = NewPauseFlag;
}


/* END OF 'ANIM.C' FILE */
