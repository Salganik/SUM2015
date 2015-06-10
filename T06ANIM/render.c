/* FILENAME: RENDER.C
 * PROGRAMMER: AS3
 * PURPOSE: Rendering system module.
 * LAST UPDATE: 09.06.2015
 */

#include <stdio.h>
#include <string.h>
#include "vec.h"
#include "anim.h"
#include "render.h"

#define AS3_UNIT_MATR \
{                 \
  {               \
    {1, 0, 0, 0}, \
    {0, 1, 0, 0}, \
    {0, 0, 1, 0}, \
    {0, 0, 0, 1}  \
  }               \
}

/* Матрицы */
MATR
  AS3_RndMatrWorld = AS3_UNIT_MATR,
  AS3_RndMatrView = AS3_UNIT_MATR,
  AS3_RndMatrWorldView = AS3_UNIT_MATR;

/* Параметры проецирования */
DBL
  AS3_RndWp = 3, AS3_RndHp = 3,     /* размеры обрасти проецирования */
  AS3_RndProjDist = 5;              /* расстояние до плоскости проекции */

/* Функция преобразования из мировой системы координат в кадр.
 * АРГУМЕНТЫ:
 *   - исходная точка:
 *       VEC P;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (POINT) координаты в кадре.
 */
POINT AS3_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* преобразование СК */
  P = PointTransform(P, AS3_RndMatrWorldView);

  Pp.X = P.X * AS3_RndProjDist / -P.Z;
  Pp.Y = P.Y * AS3_RndProjDist / -P.Z;

  Ps.x = AS3_Anim.W / 2 + Pp.X * AS3_Anim.W / AS3_RndWp;
  Ps.y = AS3_Anim.H / 2 - Pp.Y * AS3_Anim.H / AS3_RndHp;

  return Ps;
} /* End of 'AS3_RndWorldToScreen' function */


/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       as3GOBJ *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL AS3_RndGObjLoad( as3GOBJ *GObj, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0;
  static CHAR Buf[10000];

  memset(GObj, 0, sizeof(as3GOBJ));
  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* Allocate memory for data */
  if ((GObj->V = malloc(sizeof(VEC) * nv + sizeof(INT [3]) * nf)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  GObj->F = (INT (*)[3])(GObj->V + nv);

  /* Read vertices */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf",
        &GObj->V[nv].X, &GObj->V[nv].Y, &GObj->V[nv].Z);
      nv++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT a, b, c;

      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
        sscanf(Buf + 2, "%i %i %i", &a, &b, &c);

      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }

  fclose(F);

  GObj->NumOfV = nv;
  GObj->NumOfF = nf;
  return TRUE;
} /* End of 'AS3_RndGObjLoad' function */

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       as3GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_RndGObjDraw( as3GOBJ *GObj )
{
  INT i;
  POINT *pnts;

  if ((pnts = malloc(sizeof(POINT) * GObj->NumOfV)) == NULL)
    return;

  /* проецируем все точки */
  AS3_RndMatrWorldView = MatrMulMatr(AS3_RndMatrWorld, AS3_RndMatrView);
  for (i = 0; i < GObj->NumOfV; i++)
    pnts[i] = AS3_RndWorldToScreen(GObj->V[i]);

  /* рисуем треугольники */
  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT
      n1 = GObj->F[i][0],
      n2 = GObj->F[i][1],
      n3 = GObj->F[i][2];

    MoveToEx(AS3_Anim.hDC, pnts[n1].x, pnts[n1].y, NULL);
    LineTo(AS3_Anim.hDC, pnts[n2].x, pnts[n2].y);
    LineTo(AS3_Anim.hDC, pnts[n3].x, pnts[n3].y);
    LineTo(AS3_Anim.hDC, pnts[n1].x, pnts[n1].y);
  }

  free(pnts);
} /* End of 'AS3_RndGObjDraw' function */

/* Функция освобождения памяти из-под геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       as3GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_RndGObjFree( as3GOBJ *GObj )
{
  free(GObj->V);
  memset(GObj, 0, sizeof(as3GOBJ));
} /* End of 'AS3_RndGObjFree' function */

/* END OF 'RENDER.C' FILE */

