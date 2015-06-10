/* FILENAME: UCOW.C
 * PROGRAMMER: AS3
 * PURPOSE: Cow unit handle module.
 * LAST UPDATE: 09.06.2015
 */

#include <string.h> 
#include "anim.h"
#include "vec.h"
#include "render.h"

/* COW image type */
typedef struct tagas3UNIT_COW
{
  AS3_UNIT_BASE_FIELDS;
  as3GOBJ Model;
  DWORD Color; /* Cow color */
  VEC Pos;     /* Cow position */  
} as3UNIT_COW;

/* Animation object initialization function.
 * ARGUMENTS:
 *   - self-pointer - animation object itself:
 *       as3UNIT_COW *Uni;
 *   - animation context pointer:
 *       as3ANIM *Ani;
 * RETURNS: none.
 */
static VOID AS3_AnimUnitInit( as3UNIT_COW *Uni, as3ANIM *Ani )
{
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);  
  Uni->Pos = VecSet(Ani->W + rand() / 228, Ani->H + rand() / 228, 0); 
  AS3_RndGObjLoad(&Uni->Model, "cow.object");
} /* End of 'AS3_AnimUnitInit' function */

/* Animation object deinitialization function.
 * ARGUMENTS:
 *   - óêàçàòåëü íà "ñåáÿ" - ñàì îáúåêò àíèìàöèè:
 *       as3UNIT_COW *Uni;
 *   - animation context pointer:
 *       as3ANIM *Ani;
 * RETURNS: none.
 */
static VOID AS3_AnimUnitClose( as3UNIT_COW *Uni, as3ANIM *Ani )
{
  AS3_RndGObjFree(&Uni->Model);
} /* End of 'AS3_AnimUnitClose' function */

static VOID AS3_AnimUnitResponse( as3UNIT_COW *Uni, as3ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    AS3_AnimDoExit();
  if (Ani->KeysClick['F'])
    AS3_AnimFlipFullScreen();
} /* End of 'AS3_AnimUnitResponse' function */

static VOID AS3_AnimUnitRender( as3UNIT_COW *Uni, as3ANIM *Ani )
{
  SetDCBrushColor(Ani->hDC, Uni->Color);
  SetDCPenColor(Ani->hDC, RGB(255, 0, 0));

  AS3_RndMatrView = MatrView(VecSet(32, 12, 12), VecSet(0, 0, 0), VecSet(0, 1, 0));
  AS3_RndMatrWorld = MatrMulMatr(MatrRotateZ(30 * sin(Ani->Time * 8)), MatrRotateY(30 * Ani->Time));

  AS3_RndGObjDraw(&Uni->Model); 
} /* End of 'AS3_AnimUnitRender' function */

as3UNIT * AS3_UnitCowCreate( VOID )
{
  as3UNIT_COW *Uni;            

  if ((Uni = (VOID *)AS3_AnimUnitCreate(sizeof(as3UNIT_COW))) == NULL)
    return NULL;
                                
  Uni->Init = (VOID *)AS3_AnimUnitInit;
  Uni->Close = (VOID *)AS3_AnimUnitClose;
  Uni->Response = (VOID *)AS3_AnimUnitResponse;
  Uni->Render = (VOID *)AS3_AnimUnitRender;
  return (as3UNIT *)Uni;
} /* End of 'AS3_UnitCowCreate' function */

/* END OF 'UCOW.C' FILE */

