#version 330 core

in vec2 v_TexCoord;
out vec4 color;
uniform sampler2D u_ScreenTex;


const float offset_x = 1.0f / 850;
const float offset_y = 1.0f / 850;

vec2 offsets[9] = vec2[]
(
	vec2(-offset_x,  offset_y),	vec2(0.0f,  offset_y), vec2(offset_x, offset_y),
	vec2(-offset_x,  0.0f),		vec2(0.0f,  0.0f),	   vec2(offset_x, 0.0f),
	vec2(-offset_x, -offset_y), vec2(0.0f, -offset_y), vec2(offset_x, -offset_y)
);

float kernel[9] = float[]
(
	1,  1, 1,
	1, -8, 1,
	1,  1, 1
);

void main()
{
	vec3 col = vec3(0.0f);
	color = texture(u_ScreenTex, v_TexCoord);//vec4(v_TexCoord, 0, 0);//
	for (int i = 0; i < 9; i++)
		col += vec3(texture(u_ScreenTex, v_TexCoord.st + offsets[i])) * kernel[i];
}