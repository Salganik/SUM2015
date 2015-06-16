/* FILENAME: ANIM.H
 * PROGRAMMER: AS3
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "render.h"

/* Максимальное количество элементов анимации */
#define AS3_MAX_UNITS 3000

/* Объявление типа объектов анимации "вперед" */
typedef struct tagas3UNIT as3UNIT;

/* Структура хранения контекста анимации */
typedef struct
{
  HWND hWnd;          /* Окно вывода */
  INT W, H;           /* Размер окна вывода */
  HDC hDC;            /* Контекст вывода */
  HGLRC hGLRC;        /* Контекст рендеринга */

  /* Массив элементов анимации и их количество */
  as3UNIT *Units[AS3_MAX_UNITS];
  INT NumOfUnits;

  BOOL IsPause;       /* Проверка на паузу */
  DBL GlobalTime, 
      GlobalDeltaTime, 
      DeltaTime, 
      Time, 
      FPS;
  /* Подсистема ввода */
  BYTE
    Keys[256],       /* Состояние клавиш клавиатуры и мыши */
    KeysOld[256],    /* Состояние клавиш на предыдущем кадре */
    KeysClick[256];  /* Флаги однократного нажатия */
  INT
    MsDeltaX, MsDeltaY, /* Относительные координаты курсора мыши */
    MsX, MsY,        /* Абсолютные координаты курсора мыши */
    MsWheel;         /* Состояние колеса мыши */
  BYTE
    JButs[32],       /* Состояние кнопок джойстика */
    JButsOld[32],    /* Состояние кнопок джойстика на предыдущем кадре */
    JButsClick[32],  /* Флаги однократного нажатия кнопок джойстика */
    JPOV;            /* Переключатель POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV; /* Оси джойстика (-1.0 .. 1.0*/


} as3ANIM;

extern as3ANIM AS3_Anim;
extern INT AS3_GamePadShiftZ;

/* Базовые поля объекта анимации:
 * - размер структуры для наследования
 *     INT Size;
 * - идентификатор объеткта:
 *     INT Id;
 * - указатель на функцию инициализации
 *     VOID (*Init)( as3UNIT *Unit, as3ANIM *Ani );
 * - указатель на функцию деинициализации
 *     VOID (*Close)( as3UNIT *Unit, as3ANIM *Ani );
 * - указатель на функцию обновления межкадровых параметров
 *     VOID (*Response)( as3UNIT *Unit, as3ANIM *Ani );
 * - указатель на функцию построения
 *     VOID (*Render)( as3UNIT *Unit, as3ANIM *Ani );
 */
#define AS3_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( as3UNIT *Unit, as3ANIM *Ani );       \
  VOID (*Close)( as3UNIT *Unit, as3ANIM *Ani );      \
  VOID (*Response)( as3UNIT *Unit, as3ANIM *Ani );   \
  VOID (*Render)( as3UNIT *Unit, as3ANIM *Ani )

/* Базовый тип объекта анимации */
struct tagas3UNIT
{
  AS3_UNIT_BASE_FIELDS;
};

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
BOOL AS3_AnimInit( HWND hWnd );

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimClose( VOID );

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimResize( INT W, INT H );

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimRender( VOID );

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimCopyFrame( VOID );

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       as3UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimAddUnit( as3UNIT *Unit );

/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimFlipFullScreen( VOID );

/* Функция создания объекта анимации.
 * АРГУМЕНТЫ:
 *   - размер структуры объекта анимации:
 *       INT Size;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (as3UNIT *) указатель на созданный объект анимации.
 */
as3UNIT * AS3_AnimUnitCreate( INT Size );

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID AS3_AnimDoExit( VOID );

/* Set pause function.
 * ARGUMENTS: 
 *   - pause flag:
 *       BOOL NewPAuseFlag;
 * RETURNS: none.
 */  
VOID AS3_AnimSetPause( BOOL NewPauseFlag );

extern INT AS3_MouseWheel;


#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */