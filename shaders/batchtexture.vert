#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float aTexIndex;

out vec2 v_TexCoord;
flat out int v_TexIndex;
uniform mat4 u_MVP;
void main()
{
   gl_Position = u_MVP * vec4(aPos, 0.0f, 1.0f);
   v_TexCoord = aTexCoord;
   v_TexIndex = int(aTexIndex);
}