#version 410 core

layout(location = 0) out vec4 FragColor;
in vec2 v_TexCoord;
flat in float v_TexIndex;

uniform vec4 u_Color;
uniform sampler2D u_Textures[16];

void main()
{
    int index = int(v_TexIndex);
   vec4 texColor = texture(u_Textures[index], v_TexCoord);
   //vec4 texColor = texture(u_TextureArray, vec3(v_TexCoord, v_TexIndex));
   if(u_Color.w == 0.0f)
   {
      FragColor = texColor;
   }else{
      FragColor = u_Color * texColor;
   }
}
