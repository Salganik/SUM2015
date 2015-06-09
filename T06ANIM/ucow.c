/* FILENAME: UCOW.C
 * PROGRAMMER: AS3
 * PURPOSE: Cow unit handle module.
 * LAST UPDATE: 09.06.2015
 */

#include <string.h> 
#include "anim.h"
#include "vec.h"
#include "obj3d.h"

/* COW image type */
typedef struct tagas3UNIT_COW
{
  AS3_UNIT_BASE_FIELDS;
    
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
  ObjLoad("cow.object");
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
} /* End of 'AS3_AnimUnitClose' function */

static VOID AS3_AnimUnitResponse( as3UNIT_COW *Uni, as3ANIM *Ani )
{
  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    AS3_AnimDoExit();
  if (GetAsyncKeyState('F') & 0x8000)
    AS3_AnimFlipFullScreen();
} /* End of 'AS3_AnimUnitResponse' function */

static VOID AS3_AnimUnitRender( as3UNIT_COW *Uni, as3ANIM *Ani )
{
  SetDCBrushColor(Ani->hDC, Uni->Color);
  ObjDraw(Ani->hDC, Uni->Pos.X, Uni->Pos.Y);  
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

