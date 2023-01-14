#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_CamPos;
out vec3 v_Pos;

uniform vec3 u_CamPos;
uniform mat4 u_Proj;
uniform mat4 u_View;

void main()
{
	gl_Position = u_Proj * (u_View * vec4(position + u_CamPos, 1));
	v_Pos = position;
	//gl_Position.x /= gl_Position.w;
	//gl_Position.y /= gl_Position.w;
	gl_Position.z /= -gl_Position.w;
	v_TexCoord = texCoord;
	v_Normal = normal;
	v_CamPos = u_CamPos;
}