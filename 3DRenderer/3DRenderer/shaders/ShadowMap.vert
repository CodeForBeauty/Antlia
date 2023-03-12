#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 lightProj;

void main()
{
	//gl_Position = vec4(position, 1.0f) * lightProj;
	//gl_Position.z = 0.0f;//= -gl_Position.w;
	gl_Position = lightProj * vec4(position, 1.0f);
	gl_Position.z /= gl_Position.w;
}