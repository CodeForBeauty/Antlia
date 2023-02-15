#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in vec3 normal;

uniform vec3 u_CamPos;
uniform mat4 u_Proj;
uniform mat4 u_View;
uniform bool isSmooth;

out DATA
{
	vec2 texCoord;
	vec3 normal;
	vec3 pos;
	vec3 camPos;
	mat4 projection;
	mat4 view;
	bool isSmooth;
} data_out;

void main()
{
	//gl_Position = u_Proj * (u_View * vec4(position + u_CamPos, 1));
	//gl_Position = vec4(position, 1);
	data_out.texCoord = texCoord;
	data_out.normal = normal;
	data_out.pos = position;
	data_out.camPos = u_CamPos;
	data_out.projection = u_Proj;
	data_out.view = u_View;
	data_out.isSmooth = isSmooth;
	//v_Pos = position;
	//gl_Position.x /= gl_Position.w;
	//gl_Position.y /= gl_Position.w;
	//gl_Position.z /= -gl_Position.w;
	//v_TexCoord = texCoord;
	//v_Normal = normal;
	//v_CamPos = u_CamPos;
}