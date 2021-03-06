/* FILENAME: UCONTRL.C
 * PROGRAMMER: AS3
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <stdio.h>  

#include "anim.h" 
/* ����������� ����� */
#include <al.h>
#include <alc.h>
#include <alut.h>


#pragma comment(lib, "alut")
#pragma comment(lib, "openal32")
/* �������� �� ��� Z */
INT AS3_GamePadShiftZ = 0;

/* ��� ��������� �������� */
typedef struct tagas3UNIT_CTRL
{
  AS3_UNIT_BASE_FIELDS; 

  UINT ASrc[2];
  INT ABuf[2];
} as3UNIT_CTRL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as3UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       as3ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AS3_AnimUnitInit( as3UNIT_CTRL *Uni, as3ANIM *Ani )
{
  INT format;
  UINT size, freq;
  VOID *mem;
  CHAR loop;
  
  /* ������������� ����� ������� */
  alutInit(NULL, 0);
  alGetError();
  /* ������� ������ */
  alGenBuffers(2, Uni->ABuf);

  /* ��������� ���� � ����� */
  alutLoadWAVFile("a.wav", &format, &mem, &size, &freq, &loop);
  alBufferData(Uni->ABuf[0], format, mem, size, freq);
  alutUnloadWAV(format, mem, size, freq);

  /* ������� ��������� ����� � ��������������� �� */
  alGenSources(2, Uni->ASrc);

  alSourcei(Uni->ASrc[0], AL_BUFFER, Uni->ABuf[0]); /* ���������� ����� �� ���������� */
  alSourcef(Uni->ASrc[0], AL_PITCH, 1);             /* �������� ���������������: 1.0 - �������*/
  alSourcef(Uni->ASrc[0], AL_GAIN, 0.3);              /* ���������: 1.0 � ������� */
  alSourcei(Uni->ASrc[0], AL_LOOPING, 1);           /* ���� �������: 0 � ���, 1 � ���������� */

  alSourcePlay(Uni->ASrc[0]); /* ������ ������������ */
} /* End of 'AS3_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as3UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       as3ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AS3_AnimUnitClose( as3UNIT_CTRL *Uni, as3ANIM *Ani )
{
 
} /* End of 'AS3_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as3UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       as3ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AS3_AnimUnitResponse( as3UNIT_CTRL *Uni, as3ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    AS3_AnimDoExit();
  if (Ani->KeysClick['F'])
    AS3_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    AS3_AnimSetPause(!Ani->IsPause);  
  
  if (Ani->JButsClick[7])
    AS3_AnimFlipFullScreen();
  if (Ani->JButsClick[1])
    AS3_AnimSetPause(!Ani->IsPause);
  if (Ani->JButs[11])
    AS3_AnimDoExit();
  if (Ani->JButs[10])
    AS3_GamePadShiftZ++;
  if (Ani->JButs[2])
    AS3_GamePadShiftZ--; 
  if (Ani->KeysClick['S'])
    alSourcePlay(Uni->ASrc[0]);
  if (Ani->KeysClick['M'])
    alSourceStopv(1, Uni->ASrc); 
} /* End of 'AS3_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       as3UNIT_CTRL *Uni;
 *   - ��������� �� �������� ��������:
 *       as3ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID AS3_AnimUnitRender( as3UNIT_CTRL *Uni, as3ANIM *Ani )
{
  static DBL count = 30;
  static CHAR Buf[1000];

  count += Ani->GlobalDeltaTime;
  if (count > 1)
  {
    count = 0;
    sprintf(Buf, "FPS: %.3f", Ani->FPS);
    SetWindowText(Ani->hWnd, Buf);
  }

} /* End of 'AS3_AnimUnitRender' function */

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (as3UNIT *) ��������� �� ��������� ������ ��������.
 */
as3UNIT * AS3_UnitControlCreate( VOID )
{
  as3UNIT_CTRL *Uni;

  if ((Uni = (VOID *)AS3_AnimUnitCreate(sizeof(as3UNIT_CTRL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)AS3_AnimUnitInit;
  Uni->Close = (VOID *)AS3_AnimUnitClose;
  Uni->Response = (VOID *)AS3_AnimUnitResponse;
  Uni->Render = (VOID *)AS3_AnimUnitRender;
  return (as3UNIT *)Uni;
} /* End of 'AS3_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */
