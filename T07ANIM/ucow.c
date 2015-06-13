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
  AS3_RndGObjLoad(&Uni->Model, "cube.object");
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
  INT i, j;

  AS3_RndMatrView = MatrView(VecSet(8, 8, 8),
                             VecSet(0, 0, 0),
		             VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  for (i = -2; i < 2; i++)
    for (j = -2; j < 2; j++)
    {
      AS3_RndMatrWorld =
                         MatrMulMatr(MatrMulMatr(MatrMulMatr(
			             MatrTranslate(Ani->JX * 228, Ani->JY * 228, 0),
				     MatrScale(0.1, 0.1, 0.1)),
				     MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
				     MatrTranslate(j * 2.5, 0, i * 2.5 + 228 * Ani->JZ));
      glColor3d(i & 1, j & 1, 1 - ((i & 1) + (j & 1)) / 2);
      AS3_RndGObjDraw(&Uni->Model);
    }
  
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



