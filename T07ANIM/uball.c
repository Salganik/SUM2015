/* FILENAME: UBALL.C
 * PROGRAMMER: AS3
 * PURPOSE: Bounce cow unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* Тип представления мяча */
typedef struct tagas3UNIT_BALL
{
  AS3_UNIT_BASE_FIELDS;

  VEC Pos;     /* Позиция мяча */
  DWORD Color; /* Цвет мяча */
  DBL
    Amplitude,  /* Амплитуда */
    PhaseShift, /* Сдвиг по фазе */
    ScaleShift; /* Масштаб времени */
} as3UNIT_BALL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as3UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       as3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AS3_AnimUnitInit( as3UNIT_BALL *Uni, as3ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 4300, rand() % 1500, rand() % 8000);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 10;
  Uni->ScaleShift = 5 + rand() / RAND_MAX;
  Uni->Amplitude = 30 + rand()/ RAND_MAX;
} /* End of 'AS3_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as3UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       as3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AS3_AnimUnitClose( as3UNIT_BALL *Uni, as3ANIM *Ani )
{
} /* End of 'AS3_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as3UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       as3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AS3_AnimUnitResponse( as3UNIT_BALL *Uni, as3ANIM *Ani )
{
  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    AS3_AnimDoExit();
  if (GetAsyncKeyState('F') & 0x8000)
    AS3_AnimFlipFullScreen();
} /* End of 'AS3_AnimUnitResponse' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as3UNIT_BALL *Uni;
 *   - указатель на контекст анимации:
 *       as3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AS3_AnimUnitRender( as3UNIT_BALL *Uni, as3ANIM *Ani )
{
  
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * (DBL)clock() / CLOCKS_PER_SEC + Uni->PhaseShift));

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - rand() % 15, Uni->Pos.Y - rand() % 15 - shift, Uni->Pos.X + rand() % 15, Uni->Pos.Y + rand() % 15 - shift);
} /* End of 'AS3_AnimUnitRender' function */

/* Функция создания объекта анимации "мяч".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (as3UNIT *) указатель на созданный объект анимации.
 */
as3UNIT * AS3_UnitBallCreate( VOID )
{
  as3UNIT_BALL *Uni;

  if ((Uni = (VOID *)AS3_AnimUnitCreate(sizeof(as3UNIT_BALL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AS3_AnimUnitInit;
  Uni->Close = (VOID *)AS3_AnimUnitClose;
  Uni->Response = (VOID *)AS3_AnimUnitResponse;
  Uni->Render = (VOID *)AS3_AnimUnitRender;
  return (as3UNIT *)Uni;
} /* End of 'AS3_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */