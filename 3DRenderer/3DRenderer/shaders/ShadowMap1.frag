#version 330 core

in vec4 fragPos;

uniform vec3 u_Pos;
uniform float u_FarPlane;

void main()
{
	gl_FragDepth = length(fragPos.xyz) / 10;
}