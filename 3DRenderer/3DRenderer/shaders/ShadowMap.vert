#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 lightProj;
uniform vec3 u_Pos;

void main()
{
	gl_Position = lightProj * vec4(position + u_Pos, 1.0f);
	gl_Position.z /= -gl_Position.w;
}