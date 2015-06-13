/* FILE: RENDER.H
 * PROGRAMMER: AS3
 * DATE: 09.06.2015
 * PURPOSE: include stuff that renders stuff.
*/

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>


/* Тип представления объекта - набор вершин и треугольников */
typedef struct tagas3GOBJ
{
  INT
      VA,          /* массив вершин */
      VBuf, IBuf,  /* буфера вершин и индексов */
      NumOfV,      /* количество вершин */
      NumOfF;      /* количество граней */
} as3GOBJ;

/* Матрицы */
extern MATR
  AS3_RndMatrWorld,
  AS3_RndMatrView,
  AS3_RndMatrProj;

/* Параметры проецирования */
extern DBL
  AS3_RndWp, AS3_RndHp,    /* размеры обрасти проецирования */
  AS3_RndProjDist;         /* расстояние до плоскости проекции */

/* Шейдер по умолчанию */
extern UINT AS3_RndProg;

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       as3GOBJ *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL AS3_RndGObjLoad( as3GOBJ *GObj, CHAR *FileName );

/* Функция освобождения памяти из-под геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       as3GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_RndGObjFree( as3GOBJ *GObj );

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       as3GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_RndGObjDraw( as3GOBJ *GObj );

UINT AS3_ShaderLoad(CHAR *FileNamePrefix);

/* Функция уничтодения шейдеров для программы.
* АРНУМЕНТЫ:
*   - номер программы:
*       UINT PrgId;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AS3_ShaderFree(UINT PrgId);

#endif /* __RENDER_H_ */    

/* END OF 'RENDER.H' FILE */