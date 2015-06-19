/* FILENAME: UCONTRL.C
 * PROGRAMMER: AS3
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <stdio.h>  

#include "anim.h" 
/* полключание звука */
#include <al.h>
#include <alc.h>
#include <alut.h>


#pragma comment(lib, "alut")
#pragma comment(lib, "openal32")
/* смещение по оси Z */
INT AS3_GamePadShiftZ = 0;

/* Тип структуры контроля */
typedef struct tagas3UNIT_CTRL
{
  AS3_UNIT_BASE_FIELDS; 

  UINT ASrc[2];
  INT ABuf[2];
} as3UNIT_CTRL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as3UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       as3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AS3_AnimUnitInit( as3UNIT_CTRL *Uni, as3ANIM *Ani )
{
  INT format;
  UINT size, freq;
  VOID *mem;
  CHAR loop;
  
  /* Инициализация аудио системы */
  alutInit(NULL, 0);
  alGetError();
  /* создаем буфера */
  alGenBuffers(2, Uni->ABuf);

  /* загружаем звук в буфер */
  alutLoadWAVFile("a.wav", &format, &mem, &size, &freq, &loop);
  alBufferData(Uni->ABuf[0], format, mem, size, freq);
  alutUnloadWAV(format, mem, size, freq);

  /* создаем источники звука и параметризируем их */
  alGenSources(2, Uni->ASrc);

  alSourcei(Uni->ASrc[0], AL_BUFFER, Uni->ABuf[0]); /* закрепляем буфер за источником */
  alSourcef(Uni->ASrc[0], AL_PITCH, 1);             /* скорость воспроизведения: 1.0 - обычная*/
  alSourcef(Uni->ASrc[0], AL_GAIN, 0.3);              /* громкость: 1.0 – обычная */
  alSourcei(Uni->ASrc[0], AL_LOOPING, 1);           /* флаг повтора: 0 – нет, 1 – бесконечно */

  alSourcePlay(Uni->ASrc[0]); /* запуск проигрывания */
} /* End of 'AS3_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as3UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       as3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID AS3_AnimUnitClose( as3UNIT_CTRL *Uni, as3ANIM *Ani )
{
 
} /* End of 'AS3_AnimUnitClose' function */

/* Функция обновления межкадровых параметров объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as3UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       as3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       as3UNIT_CTRL *Uni;
 *   - указатель на контекст анимации:
 *       as3ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

/* Функция создания объекта анимации "управление".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (as3UNIT *) указатель на созданный объект анимации.
 */
as3UNIT * AS3_UnitControlCreate( VOID )
{
  as3UNIT_CTRL *Uni;

  if ((Uni = (VOID *)AS3_AnimUnitCreate(sizeof(as3UNIT_CTRL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AS3_AnimUnitInit;
  Uni->Close = (VOID *)AS3_AnimUnitClose;
  Uni->Response = (VOID *)AS3_AnimUnitResponse;
  Uni->Render = (VOID *)AS3_AnimUnitRender;
  return (as3UNIT *)Uni;
} /* End of 'AS3_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */
