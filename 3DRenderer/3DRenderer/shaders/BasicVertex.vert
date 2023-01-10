#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform vec4 u_Pos;
uniform vec4 u_Scale;
uniform mat4 u_Proj;
uniform mat4 u_Rot;

void main()
{
	gl_Position = u_Proj * (position);//u_Proj * (u_Rot * (position * u_Scale) + u_Pos);
	//gl_Position.x /= gl_Position.w;
	//gl_Position.y /= gl_Position.w;
	//gl_Position.z /= gl_Position.w;
	v_TexCoord = texCoord;
}