#version 410 core

layout(location = 0) out vec4 FragColor;
//out vec4 FragColor;
in vec2 v_TexCoord;
//in vec4 v_ColorCoord;

uniform vec4 u_Color;
//uniform sampler2D u_Texture;

void main()
{
   //vec4 texColor = texture(u_Texture, v_TexCoord);
   FragColor = u_Color;// * texColor;
   // + vec4(0.0f, 0.0f, 0.5f, 0.5f);
   //FragColor = vec4(1,0,0,1);
}
