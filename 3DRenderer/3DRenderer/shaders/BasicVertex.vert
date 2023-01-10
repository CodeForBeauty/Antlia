#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_Proj;
uniform vec4 u_CamPos;
uniform mat4 u_View;

void main()
{
	gl_Position = u_Proj * (u_View * position + u_CamPos);
	//gl_Position.x /= gl_Position.w;
	//gl_Position.y /= gl_Position.w;
	//gl_Position.z /= gl_Position.w;
	v_TexCoord = texCoord;
}