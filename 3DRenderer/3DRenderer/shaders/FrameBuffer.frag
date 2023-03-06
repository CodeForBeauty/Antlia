#version 330 core

in vec2 v_TexCoord;

out vec4 color;

uniform sampler2D u_ScreenTex;


void main()
{
	color = texture(u_ScreenTex, v_TexCoord);//vec4(v_TexCoord, 0, 0);//
}