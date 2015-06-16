/* FILENAME: GEOM.C
 * PROGRAMMER: AS3
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "render.h"

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       as3GEOM *G;
 *   - указатель на добавляемый примитив:
 *       as3PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива в массиве (-1 при ошибке).
 */
INT AS3_GeomAddPrim( as3GEOM *G, as3PRIM *Prim )
{
  as3PRIM *new_bulk;

  /* Выделяем память под новый массив примитивов */
  if ((new_bulk = malloc(sizeof(as3PRIM) * (G->NumOfPrimitives + 1))) == NULL)
    return -1;

  /* Копируем старые примитивы */
  if (G->Prims != NULL)
  {
    memcpy(new_bulk, G->Prims, sizeof(as3PRIM) * G->NumOfPrimitives);
    free(G->Prims);
  }
  /* указываем на новый массив примитивлв */
  G->Prims = new_bulk;

  /* Сохраняем новый элемент */
  G->Prims[G->NumOfPrimitives] = *Prim;
  return G->NumOfPrimitives++;
} /* End of 'AS3_GeomAddPrim' function */

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       as3GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_GeomFree( as3GEOM *G )
{
  INT i;

  if (G->Prims != NULL)
  {
    for (i = 0; i < G->NumOfPrimitives; i++)
      AS3_PrimFree(&G->Prims[i]);
    free(G->Prims);
  }
  memset(G, 0, sizeof(as3GEOM));
} /* End of 'AS3_GeomFree' function */

/* Функция отображения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       as3GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_GeomDraw( as3GEOM *G )
{
  INT i, loc;

  /* посылаем количество частей */
  glUseProgram(AS3_RndProg);
  loc = glGetUniformLocation(AS3_RndProg, "TotalParts");
  if (loc != -1)
    glUniform1f(loc, G->NumOfPrimitives);
  glUseProgram(0);

  for (i = 0; i < G->NumOfPrimitives; i++)
  {
    /* посылаем номер текущей части */
    glUseProgram(AS3_RndProg);
    loc = glGetUniformLocation(AS3_RndProg, "PartNo");
    if (loc != -1)
      glUniform1f(loc, i);
    glUseProgram(0);
    AS3_PrimDraw(&G->Prims[i]);
  }
} /* End of 'AS3_GeomDraw' function */

/* Функция загрузки геометрического объекта из G3D файла.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       as3GEOM *G;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе - FALSE.
 */
BOOL AS3_GeomLoad( as3GEOM *G, CHAR *FileName )
{
  FILE *F;
  INT i, j, n;
  CHAR Sign[4];
  MATR M;
  static CHAR MtlName[300];

  memset(G, 0, sizeof(as3GEOM));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  M = MatrTranspose(MatrInverse(AS3_RndPrimMatrConvert));

  /* читаем сигнатуру */
  fread(Sign, 1, 4, F);
  if (*(DWORD *)Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }

  /* читаем количество примитивов в объекте */
  fread(&n, 4, 1, F);
  fread(MtlName, 1, 300, F);

  /* читаем примитивы */
  for (i = 0; i < n; i++)
  {
    INT nv, ni, *Ind;
    as3VERTEX *Vert;
    as3PRIM P;

    /* читаем количество вершин и индексов */
    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    /* читаем имя материала текущего примитива */
    fread(MtlName, 1, 300, F);

    /* выделяем память под вершины и индексы */
    if ((Vert = malloc(sizeof(as3VERTEX) * nv + sizeof(INT) * ni)) == NULL)
      break;
    Ind = (INT *)(Vert + nv);

    /* читаем данные */
    fread(Vert, sizeof(as3VERTEX), nv, F);
    /* конвертируем геометрию */
    for (j = 0; j < nv; j++)
    {
      Vert[j].P = PointTransform(Vert[j].P, AS3_RndPrimMatrConvert);
      Vert[j].N = VectorTransform(Vert[j].N, M);
    }
    fread(Ind, sizeof(INT), ni, F);

    /* заносим в примитив */
    AS3_PrimCreate(&P, AS3_PRIM_TRIMESH, nv, ni, Vert, Ind);

    free(Vert);

    /* добавляем примитив к объекту */
    AS3_GeomAddPrim(G, &P);
  }
  fclose(F);
  AS3_RndPrimMatrConvert = MatrIdentity();
  return TRUE;
} /* End of 'AS3_GeomDraw' function */

/* END OF 'GEOM.C' FILE */
