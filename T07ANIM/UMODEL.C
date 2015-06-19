/* FILENAME: UMODEL.C
* PROGRAMMER: AS3
* PURPOSE: Model view unit handle module.
* LAST UPDATE: 10.06.2015
*/
/* полключание звука */
#include <al.h>
#include <alc.h>
#include <alut.h>


#pragma comment(lib, "alut")
#pragma comment(lib, "openal32")

#include "anim.h"

/* Тип представления мяча */
typedef struct tagas3UNIT_MODEL
{
  AS3_UNIT_BASE_FIELDS;

  as3GEOM Model; /* Модель для отображения */
  as3GEOM Geom[11]; /* Модель для отображения */
  as3PRIM Pr;
  INT TextId;
  INT TextId1;
  INT TextId2;
  INT TextId3;
  INT TextId4;/* Id текстуры */
  INT TextId5;
  INT TextId6;
  INT TextId61;
  INT TextId7;
  INT TextId8;
  INT TextId9;

}as3UNIT_MODEL;

/* Функция инициализации объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       as3UNIT_MODEL *Uni;
*   - указатель на контекст анимации:
*       as3ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID AS3_AnimUnitInit(as3UNIT_MODEL *Uni, as3ANIM *Ani)
{
  Uni->TextId = AS3_TextureLoad("Earth_D.BMP");
  AS3_GeomLoad(&Uni->Geom[0], "Earth.g3d");
  Uni->TextId1 = AS3_TextureLoad("sun.BMP");
  AS3_GeomLoad(&Uni->Geom[1], "Sun.g3d");
  Uni->TextId2 = AS3_TextureLoad("mercury.BMP");
  AS3_GeomLoad(&Uni->Geom[2], "mercury.g3d");
  Uni->TextId3 = AS3_TextureLoad("venus.BMP");
  AS3_GeomLoad(&Uni->Geom[3], "venus.g3d");
  Uni->TextId4 = AS3_TextureLoad("mars.BMP");
  AS3_GeomLoad(&Uni->Geom[4], "mars.g3d");
  Uni->TextId5 = AS3_TextureLoad("jupiter.BMP");
  AS3_GeomLoad(&Uni->Geom[5], "jupiter.g3d");
  Uni->TextId6 = AS3_TextureLoad("saturn.BMP");
  AS3_GeomLoad(&Uni->Geom[6], "saturn.g3d");
  Uni->TextId61 = AS3_TextureLoad("ring.BMP");
  AS3_GeomLoad(&Uni->Geom[7], "plate.g3d");
  Uni->TextId7 = AS3_TextureLoad("Uranus.BMP");
  AS3_GeomLoad(&Uni->Geom[8], "uranus.g3d");
  Uni->TextId8 = AS3_TextureLoad("neptun.BMP");
  AS3_GeomLoad(&Uni->Geom[9], "neptun.g3d");
  Uni->TextId9 = AS3_TextureLoad("night.BMP");
  AS3_GeomLoad(&Uni->Geom[10], "Earth.g3d");
} /* End of 'AS3_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       as3UNIT_MODEL *Uni;
*   - указатель на контекст анимации:
*       as3ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID AS3_AnimUnitClose(as3UNIT_MODEL *Uni, as3ANIM *Ani)
{
  AS3_GeomFree(&Uni->Model);
  AS3_GeomFree(&Uni->Geom[0]);
  AS3_GeomFree(&Uni->Geom[1]);
  AS3_GeomFree(&Uni->Geom[2]);
  AS3_GeomFree(&Uni->Geom[3]);
  AS3_GeomFree(&Uni->Geom[4]);
  AS3_GeomFree(&Uni->Geom[5]);
  AS3_GeomFree(&Uni->Geom[6]);
  AS3_GeomFree(&Uni->Geom[7]); 
  AS3_GeomFree(&Uni->Geom[8]);  
  AS3_GeomFree(&Uni->Geom[9]);  
  AS3_GeomFree(&Uni->Geom[10]);  
  AS3_PrimFree(&Uni->Pr);
} /* End of 'AS3_AnimUnitClose' function */

/* Функция построения объекта анимации.
* АРГУМЕНТЫ:
*   - указатель на "себя" - сам объект анимации:
*       as3UNIT_MODEL *Uni;
*   - указатель на контекст анимации:
*       as3ANIM *Ani;
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
*/
static VOID AS3_AnimUnitRender(as3UNIT_MODEL *Uni, as3ANIM *Ani)
{
  static INT i = 0, j = 0, k = 0;
  if (Ani->JR > 0)
    k += 105;
  if (Ani->JR < -0.0002)
    k -= 105;
  if (Ani->JZ > 0)
    i += 105;
  if (Ani->JZ < -0.0002)
    i -= 105;
  if (Ani->JY > 0)
    j += 1000;
  AS3_RndMatrView = MatrView(VecSet(600 + k, 600 + k, 600 + k), VecSet(i, 0, i), VecSet(0, 1, 0));


  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  if (Ani->KeysClick['S'])
    j = 1;
  glEnable(GL_DEPTH_TEST);
  AS3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotate(0.00001141552511 * 365 * j * Ani->Time, 0, 1, 0),  //Земля
    MatrScale(100, 100, 100)),
    MatrTranslate(0, -50, 1200)),
    MatrRotateY(0.00001141552511 * j * Ani->Time));                                                                           
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  AS3_GeomDraw(&Uni->Geom[0]);
  AS3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotate(0, 0, 1, 0),  //Солнце
    MatrScale(800, 800, 800)),
    MatrTranslate(0, 0, 0)),
    MatrRotateY(0));                                                                           
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId1); 
  AS3_GeomDraw(&Uni->Geom[1]);
  AS3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotate(0.6 * 0.0004 * j *  Ani->Time, 0, 1, 0),  //Меркурий
    MatrScale(30, 30, 30)),
    MatrTranslate(0, 0, 800)),
    MatrRotateY(0.0004 * j * Ani->Time));                                                                           
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId2); 
  AS3_GeomDraw(&Uni->Geom[2]);
  AS3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotate(-1.1 * 0.0001 * j * Ani->Time, 0, 1, 0),  //Венера
    MatrScale(100, 100, 100)),
    MatrTranslate(0, 0, 1000)),
    MatrRotateY(0.0001 * j * Ani->Time));                                                                           
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId3); 
  AS3_GeomDraw(&Uni->Geom[3]);
  AS3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotate(669 * 0.000006 * j * Ani->Time, 0, 1, 0),  //Марс
    MatrScale(90, 90, 90)),
    MatrTranslate(0, 0, 1500)),
    MatrRotateY(0.000006 * j * Ani->Time));                                                                           
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId4); 
  AS3_GeomDraw(&Uni->Geom[4]);
  AS3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotate(10683 * 0.0000009 * j * Ani->Time, 0, 1, 0),  //Юпитер
    MatrScale(500, 500, 500)),
    MatrTranslate(0, 0, 1800)),
    MatrRotateY(0.0000009 * j * Ani->Time));                                                                           
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId5); 
  AS3_GeomDraw(&Uni->Geom[5]);
  AS3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotate(21170 * 0.0000004 * j * Ani->Time, 0, 1, 0),  //Сатурн
    MatrScale(400, 400, 400)),
    MatrTranslate(0, 0, 2500)),
    MatrRotateY(0.0000004 * j * Ani->Time));                                                                           
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId6); 
  AS3_GeomDraw(&Uni->Geom[6]);
/* 
AS3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotate(100 * Ani->Time, 0, 1, 0),  //0.004050925925925926
    MatrScale(10, 8, 10)),
    MatrTranslate(0, 0, 2500)),
    MatrRotateX(-90));                                                                           
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId61); 
  AS3_GeomDraw(&Uni->Geom[7]);
*/ 
 AS3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotate(43800 * 0.0000001 * j * Ani->Time, 0, 1, 0),  //Уран
    MatrScale(350, 350, 350)),
    MatrTranslate(0, 0, 3000)),
    MatrRotateY(0.0000001 * j * Ani->Time));                                                                           
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId7); 
  AS3_GeomDraw(&Uni->Geom[8]);  
  AS3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotate(86036 * 0.00000007 * j * Ani->Time, 0, 1, 0),  //Нептун
    MatrScale(350, 350, 350)),
    MatrTranslate(0, 0, 3500)),
    MatrRotateY(0.00000007 * j * Ani->Time));                                                                           
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId8); 
  AS3_GeomDraw(&Uni->Geom[9]);
  AS3_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
    MatrRotate(0, 0, 1, 0),  // nebo
    MatrScale(20000, 20000, 20000)),
    MatrTranslate(0, 0, 0)),
    MatrRotateY(0));                                                                           
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId9); 
  AS3_GeomDraw(&Uni->Geom[10]);
  
} /* End of 'AS3_AnimUnitRender' function */

/* Функция создания объекта анимации "модель".
* АРГУМЕНТЫ: Нет.
* ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
*   (as3UNIT *) указатель на созданный объект анимации.
*/
as3UNIT * AS3_UnitModelCreate(VOID)
{
  as3UNIT_MODEL *Uni;

  if ((Uni = (VOID *)AS3_AnimUnitCreate(sizeof(as3UNIT_MODEL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)AS3_AnimUnitInit;
  Uni->Close = (VOID *)AS3_AnimUnitClose;
  Uni->Render = (VOID *)AS3_AnimUnitRender;
  return (as3UNIT *)Uni;
} /* End of 'AS3_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */
