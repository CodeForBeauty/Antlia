#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;


void main()
{
	gl_Position = vec4(position.x,  position.y, 1.0f, 1.0f);
	v_TexCoord = texCoord;
}