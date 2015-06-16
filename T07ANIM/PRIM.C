/* FILENAME: PRIM.C
* PROGRAMMER: AS3
* PURPOSE: Primitive handle module.
* LAST UPDATE: 13.06.2015
*/

#include "anim.h"
#include "render.h"

/* Матрица изменения примитива при создании */
MATR AS3_RndPrimMatrConvert = AS3_UNIT_MATR;

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
*   (BOOL) TRUE при успехе, иначе FALSE.
*/
VOID AS3_PrimCreate(as3PRIM *Prim, as3PRIM_TYPE Type,
                    INT NoofV, INT NoofI, as3VERTEX *Vertices, INT *Indices)
{
  Prim->Type = Type;
  Prim->NumOfI = NoofI;
  /* Создаем буфера OpenGL */
  glGenVertexArrays(1, &Prim->VA);
  glGenBuffers(1, &Prim->VBuf);
  glGenBuffers(1, &Prim->IBuf);

  /* делаем активным массив вершин */
  glBindVertexArray(Prim->VA);
  /* делаем активным буфер вершин */
  glBindBuffer(GL_ARRAY_BUFFER, Prim->VBuf);
  /* сливаем данные */
  glBufferData(GL_ARRAY_BUFFER, sizeof(as3VERTEX) * NoofV, Vertices, GL_STATIC_DRAW);
  /* делаем активным буфер индексов */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->IBuf);
  /* сливаем данные */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Indices, GL_STATIC_DRAW);

  /* задаем порядок данных */
  /*                    layout,
   *                       количество компонент,
   *                          тип,
   *                                    надо ли нормировать,
   *                                           размер в байтах одного элемента буфера,
   *                                                           смещение в байтах до начала данных */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(as3VERTEX), (VOID *)0); /* позиция */
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(as3VERTEX), (VOID *)sizeof(VEC)); /* текстура */
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(as3VERTEX), (VOID *)(sizeof(VEC) + sizeof(as3UV))); /* нормаль */
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(as3VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(as3UV))); /* цвет */

  /* включаем нужные аттрибуты (layout) */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  /* отключаемся от массива вершин */
  glBindVertexArray(0);
} /* End of 'AS3_PrimCreate' function */

/* Функция удаления примитива.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       as3PRIM *Prim;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AS3_PrimFree(as3PRIM *Prim)
{
  /* делаем активным массив вершин */
  glBindVertexArray(Prim->VA);
  /* "отцепляем" буфера */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Prim->VBuf);
  glDeleteBuffers(1, &Prim->IBuf);
  /* делаем неактивным массив вершин */
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Prim->VA);

  memset(Prim, 0, sizeof(as3PRIM));
} /* End of 'AS3_PrimFree' function */

/* Функция отрисовки примитива.
* АРГУМЕНТЫ:
*   - указатель на примитив:
*       as3PRIM *Prim;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
VOID AS3_PrimDraw( as3PRIM *Prim )
{
  INT loc;
  MATR M;

  AS3_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(AS3_RndMatrWorld, AS3_RndMatrView), AS3_RndMatrProj);

  /* оставлено для отлдадки, если нет шейдеров */
  glLoadMatrixf(AS3_RndMatrWorldViewProj.A[0]);

  /* рисуем треугольники */
  glBindVertexArray(Prim->VA);
  glUseProgram(AS3_RndProg);

  loc = glGetUniformLocation(AS3_RndProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AS3_RndMatrWorld.A[0]);
  loc = glGetUniformLocation(AS3_RndProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AS3_RndMatrView.A[0]);
  loc = glGetUniformLocation(AS3_RndProg, "MatrProj");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AS3_RndMatrProj.A[0]);
  loc = glGetUniformLocation(AS3_RndProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, AS3_RndMatrWorldViewProj.A[0]);

  M = MatrTranspose(MatrInverse(MatrMulMatr(AS3_RndMatrWorld, AS3_RndMatrView)));
  loc = glGetUniformLocation(AS3_RndProg, "MatrWVInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrMulMatr(AS3_RndMatrWorld, AS3_RndMatrView);
  loc = glGetUniformLocation(AS3_RndProg, "MatrWV");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  loc = glGetUniformLocation(AS3_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, AS3_Anim.Time);

  glPrimitiveRestartIndex(0xFFFFFFFF);
  if (Prim->Type != AS3_PRIM_TRIMESH)
    glDrawElements(GL_TRIANGLE_STRIP, Prim->NumOfI, GL_UNSIGNED_INT, NULL);
  else
    glDrawElements(GL_TRIANGLES, Prim->NumOfI, GL_UNSIGNED_INT, NULL); 

  glUseProgram(0);
  glBindVertexArray(0);
} /* End of 'AS3_PrimDraw' function */

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
BOOL AS3_PrimCreatePlane(as3PRIM *Prim, VEC Du, VEC Dv, INT N, INT M)
{
} /* End of 'AS3_PrimCreatePlane' function */

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
BOOL AS3_PrimCreateSphere(as3PRIM *Prim, VEC C, FLT R, INT N, INT M)
{
} /* End of 'AS3_PrimCreateSphere' function */

/* END OF 'PRIM.C' FILE */
