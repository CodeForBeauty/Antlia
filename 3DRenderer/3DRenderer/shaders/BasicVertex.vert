#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_CamPos;
out vec3 v_Pos;

uniform mat4 u_Proj;
uniform vec4 u_CamPos;
uniform mat4 u_View;

void main()
{
	gl_Position = u_Proj * (u_View * position + u_CamPos);
	v_Pos = vec3(position);
	//gl_Position.x /= gl_Position.w;
	//gl_Position.y /= gl_Position.w;
	//gl_Position.z /= gl_Position.w;
	v_TexCoord = texCoord;
	v_Normal = normal;
	v_CamPos = vec3(u_CamPos);
}