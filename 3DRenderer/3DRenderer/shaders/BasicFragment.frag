#version 330 core

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform int u_UseTex;

out vec4 color;

void main()
{
	color = (texture(u_Texture, v_TexCoord) * u_UseTex) + (u_Color * (1 - u_UseTex));//vec4(0.1, 0.0, 1.0, 1.0);
}