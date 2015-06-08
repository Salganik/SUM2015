/* FILENAME: ANIM.H
 * PROGRAMMER: AS3
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

/* ������������ ���������� ��������� �������� */
#define AS3_MAX_UNITS 3000

/* ���������� ���� �������� �������� "������" */
typedef struct tagas3UNIT as3UNIT;

/* ��������� �������� ��������� �������� */
typedef struct
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HBITMAP hBmFrame;   /* ����� ����� */
  HDC hDC;            /* �������� ������ */

  /* ������ ��������� �������� � �� ���������� */
  as3UNIT *Units[AS3_MAX_UNITS]; 
  INT NumOfUnits;
  DBL
    Time,            /* ����� � �������� �� ������ �������� */
    GlobalTime,      /* ����� -"-, �� ��� ����� */
    DeltaTime,       /* ����������� ����� � �������� */
    GlobalDeltaTime, /* ����������� ����� � �������� ��� ����� */
    FPS;             /* ���������� ������ � ������� */
  BOOL
    IsPause;         /* ���� ����� */

} as3ANIM;

/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ������������� ��������:
 *     INT Id;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( as3UNIT *Unit, as3ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( as3UNIT *Unit, as3ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( as3UNIT *Unit, as3ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( as3UNIT *Unit, as3ANIM *Ani );
 */
#define AS3_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( as3UNIT *Unit, as3ANIM *Ani );       \
  VOID (*Close)( as3UNIT *Unit, as3ANIM *Ani );      \
  VOID (*Response)( as3UNIT *Unit, as3ANIM *Ani );   \
  VOID (*Render)( as3UNIT *Unit, as3ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagas3UNIT
{
  AS3_UNIT_BASE_FIELDS;
};

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID AS3_AnimInit( HWND hWnd );

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS3_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID AS3_AnimResize( INT W, INT H );

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS3_AnimRender( VOID );

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS3_AnimCopyFrame( VOID );

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       as3UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID AS3_AnimAddUnit( as3UNIT *Unit );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS3_AnimFlipFullScreen( VOID );

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (as3UNIT *) ��������� �� ��������� ������ ��������.
 */
as3UNIT * AS3_AnimUnitCreate( INT Size );

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID AS3_AnimDoExit( VOID );
VOID AS3_AnimSetPause( BOOL NewPauseFlag );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
