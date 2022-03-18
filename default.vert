#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

uniform float MVP;
out vec3 color;
out vec2 texCoord;

void main()
{
   gl_Position = vec4(MVP * aPos.x + aPos.x, MVP * aPos.y + aPos.y, MVP * aPos.z + aPos.z, 1.0);
   color = aColor;
   texCoord = aTex;
}