#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 lightProj;
uniform vec3 u_CamPos;
uniform mat4 u_Proj;
uniform mat4 u_View;
uniform vec3 u_Pos;

out DATA
{
	vec3 pos;
	vec3 camPos;
	mat4 projection;
	mat4 view;
	vec4 lightPos;
} data_out;

void main()
{
	data_out.pos = position;
	data_out.camPos = u_CamPos;
	data_out.projection = u_Proj;
	data_out.view = u_View;
	data_out.lightPos = lightProj * vec4(position + u_Pos, 1);
}