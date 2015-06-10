/* FILENAME: RENDER.H
 * PROGRAMMER: AS3
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 09.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* ��� ������������� ������� - ����� ������ � ������������� */
typedef struct tagas3GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} as3GOBJ;

/* ������� */
extern MATR
  AS3_RndMatrWorld,
  AS3_RndMatrView;

/* ��������� ������������� */
extern DBL
  AS3_RndWp, AS3_RndHp,    /* ������� ������� ������������� */
  AS3_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������� �������������� �� ������� ������� ��������� � ����.
 * ���������:
 *   - �������� �����:
 *       VEC P;
 * ������������ ��������:
 *   (POINT) ���������� � �����.
 */
POINT AS3_RndWorldToScreen( VEC P );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       as3GOBJ *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL AS3_RndGObjLoad( as3GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ������ ��-��� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       as3GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID AS3_RndGObjFree( as3GOBJ *GObj );

/* ������� ��������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       as3GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID AS3_RndGObjDraw( as3GOBJ *GObj );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */