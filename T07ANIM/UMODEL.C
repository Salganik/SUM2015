/* FILENAME: UMODEL.C
 * PROGRAMMER: AS3
 * PURPOSE: Model unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"


/* MODEL image type */
typedef struct tagas3UNIT_MODEL
{
  AS3_UNIT_BASE_FIELDS;

  as3GOBJ Model;
} as3UNIT_MODEL;

/* Animation object initialization function.
 * ARGUMENTS:
 *   - self-pointer - animation object itself:
 *       as3UNIT_MODEL *Uni;
 *   - animation context pointer:
 *       as3ANIM *Ani;
 * RETURNS: none.
 */
static VOID AS3_AnimUnitInit( as3UNIT_MODEL *Uni, as3ANIM *Ani )
{  
  AS3_RndGObjLoad(&Uni->Model, "cow.object");
} /* End of 'AS3_AnimUnitInit' function */

/* Animation object deinitialization function.
 * ARGUMENTS:
 *   - указатель на "себя" - сам объект анимации:
 *       as3UNIT_MODEL *Uni;
 *   - animation context pointer:
 *       as3ANIM *Ani;
 * RETURNS: none.
 */
static VOID AS3_AnimUnitClose( as3UNIT_MODEL *Uni, as3ANIM *Ani )
{
	AS3_RndGObjFree(&Uni->Model);
} /* End of 'AS3_AnimUnitClose' function */

static VOID AS3_AnimUnitResponse(as3UNIT_MODEL *Uni, as3ANIM *Ani)
{}
/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as3UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       as3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AS3_AnimUnitRender( as3UNIT_MODEL *Uni, as3ANIM *Ani )
{  
  static INT i = 0, j = 0, k = 0;
  static BOOL Mode = TRUE;
  if (Ani->JR > 0)
    i += 5;
  if (Ani->JR < -0.0002)
    i -= 5;
  if (Ani->JZ > 0)
    j += 5;
  if (Ani->JZ < -0.0002)
    j -= 5;
  if (Ani->JButsClick[6])
    i = j = 0;
  if (Ani->JX < -0.0002)
    k -= 5;
  if (Ani->JX > 0)
    k += 5;
  AS3_RndMatrView = MatrView(VecSet(30 + i + sin(Ani->Time * 10), 30 + i + sin(Ani->Time * 10) + i, 30 + i + sin(Ani->Time * 10)), VecSet(k, j, 0), VecSet(0, 1, 0));


  if (Ani->JButsClick[7])
    Mode=!Mode;
  if (Ani->JButsClick[8])
    i = j = k = 0;
  if (Mode == TRUE)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);  
  AS3_RndMatrWorld = MatrMulMatr(MatrRotateZ(10 * Ani->Time * 10), MatrRotateY(20 * Ani->Time)), MatrRotateX(sin(Ani->Time * 18));
  AS3_RndGObjDraw(&Uni->Model); 
  
} /* End of 'AS3_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (as3UNIT *) указатель на созданный объект анимации.
 */
as3UNIT * AS3_UnitModelCreate( VOID )
{
  as3UNIT_MODEL *Uni;            

  if ((Uni = (VOID *)AS3_AnimUnitCreate(sizeof(as3UNIT_MODEL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AS3_AnimUnitInit;
  Uni->Close = (VOID *)AS3_AnimUnitClose;
	Uni->Response = (VOID *)AS3_AnimUnitResponse;
  Uni->Render = (VOID *)AS3_AnimUnitRender;
  return (as3UNIT *)Uni;
} /* End of 'AS3_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */
