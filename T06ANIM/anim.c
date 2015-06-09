/* FILENAME: ANIM.C
 * PROGRAMMER: AS3
 * PURPOSE: Animation system module.
 * LAST UPDATE: 08.06.2015
 */

#include <stdio.h>
#include "anim.h"

/* ��������� �������� �������� */
static as3ANIM AS3_Anim;


/* ������ ��� ������������� �� ������� */
static INT64
  TimeFreq,  /* ������ ��������� � ������� */
  TimeStart, /* ����� ������ �������� */
  TimeOld,   /* ����� �������� ����� */
  TimePause, /* ����� ������� � ����� */
  TimeFPS;   /* ����� ��� ������ FPS */
static INT
  FrameCounter; /* ������� ������ */

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID AS3_AnimSetPause( BOOL NewPauseFlag )
{
  AS3_Anim.IsPause = NewPauseFlag;
}
VOID AS3_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  AS3_Anim.IsPause = FALSE;
  FrameCounter = 0;
  AS3_Anim.hWnd = hWnd;
  /* �������������� ����� ����� */
  AS3_Anim.hDC = CreateCompatibleDC(hDC);
  AS3_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(AS3_Anim.hDC, AS3_Anim.hBmFrame);
  AS3_Anim.W = 30;
  AS3_Anim.H = 30;
  AS3_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* ������������� ������� */
  QueryPerformanceFrequency(&li);
  QueryPerformanceCounter(&li);
} /* End of 'AS3_AnimInit' function */

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS3_AnimClose( VOID )
{
  INT i;

  /* ������������ ������ ��-��� �������� �������� */
  for (i = 0; i < AS3_Anim.NumOfUnits; i++)
  {
    AS3_Anim.Units[i]->Close(AS3_Anim.Units[i], &AS3_Anim);
    free(AS3_Anim.Units[i]);
    AS3_Anim.Units[i] = NULL;
  }

  /* ������� ������� GDI */
  DeleteObject(AS3_Anim.hBmFrame);
  DeleteDC(AS3_Anim.hDC);
} /* End of 'AS3_AnimClose' function */

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID AS3_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(AS3_Anim.hWnd);

  /* ������������� ������ � ������ ����� */
  DeleteObject(AS3_Anim.hBmFrame);
  AS3_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(AS3_Anim.hDC, AS3_Anim.hBmFrame);

  /* ���������� ������� */
  AS3_Anim.W = W;
  AS3_Anim.H = H;

  ReleaseDC(AS3_Anim.hWnd, hDC);
} /* End of 'AS3_AnimResize' function */

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS3_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  /*** ���������� ������� ***/
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

  /* ��������� FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    static CHAR Buf[100];

    sprintf(Buf, "FPS: %.5f", AS3_Anim.FPS);
    SetWindowText(AS3_Anim.hWnd, Buf);

    AS3_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* ����� "��������" ����� */
  TimeOld = li.QuadPart;

  FrameCounter++;
  /* ����� �� ��������� ��������� �������� */
  for (i = 0; i < AS3_Anim.NumOfUnits; i++)
    AS3_Anim.Units[i]->Response(AS3_Anim.Units[i], &AS3_Anim);

  /* ������� ���� */
  SelectObject(AS3_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(AS3_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(AS3_Anim.hDC, RGB(0, 0, 0));
  Rectangle(AS3_Anim.hDC, 0, 0, AS3_Anim.W + 1, AS3_Anim.H + 1);

  /* ��������� �������� */
  for (i = 0; i < AS3_Anim.NumOfUnits; i++)
  {
    SelectObject(AS3_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(AS3_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(AS3_Anim.hDC, RGB(255, 255, 255));
    SetDCPenColor(AS3_Anim.hDC, RGB(0, 0, 0));

    AS3_Anim.Units[i]->Render(AS3_Anim.Units[i], &AS3_Anim);
  }
} /* End of 'AS3_AnimRender' function */

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS3_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* ����� ����� */
  hDC = GetDC(AS3_Anim.hWnd);
  BitBlt(hDC, 0, 0, AS3_Anim.W, AS3_Anim.H, AS3_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(AS3_Anim.hWnd, hDC);
} /* End of 'AS3_AnimCopyFrame' function */

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       as3UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID AS3_AnimAddUnit( as3UNIT *Unit )
{
  if (AS3_Anim.NumOfUnits < AS3_MAX_UNITS)
  {
    AS3_Anim.Units[AS3_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &AS3_Anim);
  }
} /* End of 'AS3_AnimAddUnit' function */


/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS3_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* ������� ����� */
  static RECT SaveRC;               /* ����������� ������ */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* ��������� ������ ������ ���� */
    GetWindowRect(AS3_Anim.hWnd, &SaveRC);

    /* ���������� � ����� �������� ��������� ���� */
    hmon = MonitorFromWindow(AS3_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* �������� ���������� ��� �������� */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* ��������� � ������ ����� */
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
    /* ��������������� ������ ���� */
    SetWindowPos(AS3_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'AS3_AnimFlipFullScreen' function */

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS3_AnimDoExit( VOID )
{
  PostMessage(AS3_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'AS3_AnimDoExit' function */

/* END OF 'ANIM.C' FILE */
