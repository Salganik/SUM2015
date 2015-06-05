/* FILE NAME: T05GLOBE.C
 * PROGAMMER: AS3
 * DATE: 04.06.2015
 * PURPOSE: WinAPI windowed application sample
 */
#include <time.h>
#include <math.h>
#include <windows.h>
#include "globe.h"

VOID DrawSphere( HDC hDC, INT w, INT h )
{
  FLOAT i, j, n = 55, m = 55, x, y, z, t = clock() / (DOUBLE)CLOCKS_PER_SEC;
  DOUBLE phi, theta;

  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCBrushColor(hDC, RGB(255, 0, 0));

  for (i = 0; i < n; i++)
  {
    theta  = i / (n - 1.0) * PI;
    for (j = 0; j < n; j++)
    {
      phi = j / (m - 1.0) * PI * 2 + sin(t * 100);
      
      x = R * sin(theta) * sin(phi);
      y = R * cos(theta);
      z = R * sin(theta) * cos(phi);
      x += w / 2;
      y += h / 2;
      if (z > 0)
        Ellipse(hDC, x - 5, y - 5, x + 5, y + 5);
    }
  }
}