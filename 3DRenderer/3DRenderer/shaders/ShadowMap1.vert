#version 330 core

layout(location = 0) in vec3 position;

uniform vec3 u_LightPos;

void main()
{
	gl_Position = vec4(position + u_LightPos, 1.0f);
}