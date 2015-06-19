/* Filename: TEST.FRAG: */

// верси€ €зыка шейдера
#version 330

// врем€
uniform float Time;
uniform float PartNo;

// выходные параметры - цвет рисовани€
out vec4 OutColor;

// входные параметры от вершинного шейдера
in vec4 DrawColor;
in vec3 DrawPosition;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

/* “екстура */
uniform sampler2D DrawTexture;

void main( void )
{
  vec4 tc = texture2D(DrawTexture, DrawTexCoord);
  OutColor = tc * DrawColor * dot(normalize(DrawNormal), normalize(vec3(1, 1, 1)));
}


