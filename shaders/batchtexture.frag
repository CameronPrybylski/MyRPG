#version 410 core

layout(location = 0) out vec4 FragColor;
in vec2 v_TexCoord;
flat in int v_TexIndex;

uniform vec4 u_Color;
uniform sampler2D u_Textures[16];

void main()
{
   vec4 texColor = texture(u_Textures[v_TexIndex], v_TexCoord);
   if(u_Color.w == 0.0f)
   {
      FragColor = texColor;
   }else{
      FragColor = u_Color * texColor;
   }
}
