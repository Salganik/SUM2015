/* FILENAME: RENDER.H
* PROGRAMMER: AS3
* PURPOSE: Rendering system declaration module.
* LAST UPDATE: 13.06.2015
*/

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* Матрицы */
extern MATR
AS3_RndMatrWorld,
AS3_RndMatrView,
AS3_RndMatrProj,
AS3_RndMatrWorldViewProj;

/* Матрица изменения примитива при создании */
extern MATR AS3_RndPrimMatrConvert;

/* Параметры проецирования */
extern DBL
AS3_RndWp, AS3_RndHp,    /* размеры обрасти проецирования */
AS3_RndProjDist;         /* расстояние до плоскости проекции */

/* Шейдер по умолчанию */
extern UINT AS3_RndProg;

/* Функция загрузки шейдеров для одной программы.
* АРНУМЕНТЫ:
*   - префикс имени файла:
*       CHAR *FileNamePrefix;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (UINT) номер загруженной программы.
*/
UINT AS3_ShaderLoad(CHAR *FileNamePrefix);

/* Функция уничтодения шейдеров для программы.
* АРНУМЕНТЫ:
*   - номер программы:
*       UINT PrgId;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AS3_ShaderFree(UINT PrgId);

/***
* Работа с примитивами
***/

/* Виды примитивов */
typedef enum tagas3PRIM_TYPE
{
  AS3_PRIM_TRIMESH,  /* примитив - набор треугольников */
  AS3_PRIM_GRID      /* регулярная сетка треугольников (через triangle strip) */
} as3PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagas3PRIM
{
  as3PRIM_TYPE Type; /* Тип примитива (AS3_PRIM_***) */
  INT
      VA,          /* массив вершин */
      VBuf, IBuf,  /* буфера вершин и индексов */
      NumOfI;      /* количество индексов для вывода примитива */
} as3PRIM;

/* Тип хранения текстурных координат */
typedef struct tagas3UV
{
  FLT U, V; /* текстурные координаты */
} as3UV;

/* Функция задания текстурных координат.
* АРГУМЕНТЫ:
*   - новые значени координат:
*       FLT U, V;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (as3UV) построенные координаты.
*/
__inline as3UV UVSet(FLT U, FLT V)
{
  as3UV UV = { U, V };

  return UV;
} /* End of 'UVSet' function */

/* Тип хранения одной вершины паримитива */
typedef struct tagas3VERTEX
{
  VEC P;    /* позиция вершины */
  as3UV T;  /* текстурные координаты вершины */
  VEC N;    /* нормаль в вершине */
  COLOR C;  /* цвет вершины */
} as3VERTEX;

/* Функция создания примитива.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       as3PRIM *Prim;
*   - тип примитива (AS3_PRIM_***):
*       as3PRIM_TYPE Type;
*   - количество вершин и индексов:
*       INT NoofV, NoofI;
*   - массив вершин:
*       as3VERTEX *Vertices;
*   - массив индексов:
*       INT *Indices;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AS3_PrimCreate(as3PRIM *Prim, as3PRIM_TYPE Type, 
                    INT NoofV, INT NoofI, as3VERTEX *Vertices, INT *Indices);

/* Функция удаления примитива.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       as3PRIM *Prim;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AS3_PrimFree(as3PRIM *Prim);

/* Функция отрисовки примитива.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       as3PRIM *Prim;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AS3_PrimDraw(as3PRIM *Prim);

/* Функция создания примитива плоскость.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       as3PRIM *Prim;
*   - касательные вектора-стороны:
*       VEC Du, Dv;
*   - разбиение:
*       INT N, M;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (BOOL) TRUE при успехе, иначе FALSE.
*/
BOOL AS3_PrimCreatePlane(as3PRIM *Prim, VEC Du, VEC Dv, INT N, INT M);

/* Функция создания примитива сфера.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       as3PRIM *Prim;
*   - центр сферы:
*       VEC С;
*   - радиус сферы:
*       FLT R;
*   - разбиение:
*       INT N, M;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (BOOL) TRUE при успехе, иначе FALSE.
*/
BOOL AS3_PrimCreateSphere(as3PRIM *Prim, VEC C, FLT R, INT N, INT M);

/* Функция загрузки геометрического объекта.
* АРГУМЕНТЫ:
*   - структура объекта для загрузки:
*       as3PRIM *GObj;
*   - имя файла:
*       CHAR *FileName;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (BOOL) TRUE при успехе, FALSE иначе.
*/
BOOL AS3_PrimLoad(as3PRIM *GObj, CHAR *FileName);

/***
* Работа с геометрическими объектами
***/

/* Геометрический объект - набор примитивов */
typedef struct tagas3GEOM
{
  INT NumOfPrimitives; /* количество примитивов */
  as3PRIM *Prims;      /* массив примитивов */
} as3GEOM;

/* Функция добавления примитива к геометрическому объекту.
* АРГУМЕНТЫ:
*   - указатель на геометрический объект:
*       as3GEOM *G;
*   - указатель на добавляемый примитив:
*       as3PRIM *Prim;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (INT) номер добавленного примитива в массиве (-1 при ошибке).
*/
INT AS3_GeomAddPrim(as3GEOM *G, as3PRIM *Prim);

/* Функция освобождения геометрического объекта.
* АРГУМЕНТЫ:
*   - указатель на геометрический объект:
*       as3GEOM *G;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AS3_GeomFree(as3GEOM *G);

/* Функция отображения геометрического объекта.
* АРГУМЕНТЫ:
*   - указатель на геометрический объект:
*       as3GEOM *G;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AS3_GeomDraw(as3GEOM *G);

/* Функция загрузки геометрического объекта из G3D файла.
* АРГУМЕНТЫ:
*   - указатель на геометрический объект:
*       as3GEOM *G;
*   - имя файла:
*       CHAR *FileName;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (BOOL) TRUE при успехе, иначе - FALSE.
*/
BOOL AS3_GeomLoad(as3GEOM *G, CHAR *FileName);

/* Функция загрузки текстуры.
* АРГУМЕНТЫ:
*   - имя файла:
*       CHAR *FileName;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (INT ) идентификатор OpenGL для текстуры.
*/
INT AS3_TextureLoad(CHAR *FileName);

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
