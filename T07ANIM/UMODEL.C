/* FILENAME: UMODEL.C
* PROGRAMMER: AS3
* PURPOSE: Model view unit handle module.
* LAST UPDATE: 10.06.2015
*/

#include "anim.h"

/* Тип представления мяча */
typedef struct tagas3UNIT_MODEL
{
  AS3_UNIT_BASE_FIELDS;

  as3GEOM Model; /* Модель для отображения */
  as3GEOM Geom; /* Модель для отображения */
  as3PRIM Pr;
  INT TextId;  /* Id текстуры */
} as3UNIT_MODEL;

/* Функция инициализации объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       as3UNIT_MODEL *Uni;
*   - указатель на контекст анимации:
*       as3ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID AS3_AnimUnitInit(as3UNIT_MODEL *Uni, as3ANIM *Ani)
{
  as3VERTEX V[] =
  {
    { { 0, 0, 0 }, { 0, 0 }, { 0, 0, 1 }, { 1, 1, 1, 1 } },
    { { 1, 0, 0 }, { 5, 0 }, { 0, 0, 1 }, { 1, 0, 1, 1 } },
    { { 0, 1, 0 }, { 0, 5 }, { 0, 0, 1 }, { 1, 1, 0, 1 } },
    { { 1, 1, 0 }, { 5, 5 }, { 0, 0, 1 }, { 1, 1, 0, 1 } },
  };
  INT I[] = { 0, 1, 2, 2, 1, 3 };
  BYTE txt[2][2][3] =
  {
    { { 255, 255, 255 }, { 0, 0, 0 } },
    { { 0, 0, 0 }, { 255, 255, 255 } }
  };

  /* загружаем текстуру */
  Uni->TextId = AS3_TextureLoad("Earth_D.BMP");
  AS3_GeomLoad(&Uni->Model, "Earth.g3d");
  AS3_RndPrimMatrConvert = MatrRotateX(-90);
  //AS3_GeomLoad(&Uni->Geom, "X6.G3D");
} /* End of 'AS3_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       as3UNIT_MODEL *Uni;
*   - указатель на контекст анимации:
*       as3ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID AS3_AnimUnitClose(as3UNIT_MODEL *Uni, as3ANIM *Ani)
{
  AS3_GeomFree(&Uni->Model);
  AS3_GeomFree(&Uni->Geom);
  AS3_PrimFree(&Uni->Pr);
} /* End of 'AS3_AnimUnitClose' function */

/* Функция построения объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       as3UNIT_MODEL *Uni;
*   - указатель на контекст анимации:
*       as3ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID AS3_AnimUnitRender(as3UNIT_MODEL *Uni, as3ANIM *Ani)
{
  INT i, j;
  static INT k = 0;
  if (Ani->JR > 0)
    k += 5;
  if (Ani->JR < -0.0002)
    k -= 5;

  AS3_RndMatrView = MatrView(VecSet(30 + k, 30 + k, 30 + k), VecSet(0, 0, 0), VecSet(0, 1, 0));


  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);
  AS3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotate(3650 * Ani->Time, 0, 1, 0),  //0.004050925925925926
    MatrScale(100, 100, 100)),
    MatrTranslate(100, 10, 10)),
    MatrRotateY(10 * Ani->Time));                                                                           
  AS3_GeomDraw(&Uni->Model);
  //MatrMulMatr(MatrRotateZ(30 * sin(Ani->Time * 3.0)), MatrRotateY(30 * Ani->Time));
  AS3_GeomDraw(&Uni->Geom);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  AS3_PrimDraw(&Uni->Pr);
} /* End of 'AS3_AnimUnitRender' function */

/* Функция создания объекта анимации "модель".
* АРГУМЕНТЫ: Нет.
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (as3UNIT *) указатель на созданный объект анимации.
*/
as3UNIT * AS3_UnitModelCreate(VOID)
{
  as3UNIT_MODEL *Uni;

  if ((Uni = (VOID *)AS3_AnimUnitCreate(sizeof(as3UNIT_MODEL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AS3_AnimUnitInit;
  Uni->Close = (VOID *)AS3_AnimUnitClose;
  Uni->Render = (VOID *)AS3_AnimUnitRender;
  return (as3UNIT *)Uni;
} /* End of 'AS3_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */
