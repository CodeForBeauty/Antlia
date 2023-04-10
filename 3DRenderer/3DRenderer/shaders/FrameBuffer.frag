#version 330 core

in vec2 v_TexCoord;
out vec4 color;
uniform sampler2D u_ScreenTex;


const float off_x = 1.0f / 1024;
const float off_y = 1.0f / 1024;

vec2 offsets[25] = vec2[]
(
	vec2(-2, -2),	vec2(-1, -2),  vec2(0, -2),  vec2(1, -2),  vec2(2, -2),
	vec2(-2, -1),	vec2(-1, -1),  vec2(0, -1),  vec2(1, -1),  vec2(2, -1),
	vec2(-2,  0),	vec2(-1,  0),  vec2(0,  0),	 vec2(1,  0),  vec2(2,  0),
	vec2(-2,  1),	vec2(-1,  1),  vec2(0,  1),  vec2(1,  1),  vec2(2,  1),
	vec2(-2,  2),	vec2(-1,  2),  vec2(0,  2),  vec2(1,  2),  vec2(2,  2)
);

float kernel[25] = float[]
(
	1f/256, 1f/128, 1f/64, 1f/128, 1f/256,
	1f/128, 1f/32,  1f/16, 1f/32,  1f/128,
	1f/64,  1f/16,  1f/4,  1f/16,  1f/64,
	1f/128, 1f/32,  1f/16, 1f/32,  1f/128,
	1f/256, 1f/128, 1f/64, 1f/128, 1f/256
);

void main()
{
	vec3 col = vec3(0.0f);
	vec2 pixelSize = 1.0f / textureSize(u_ScreenTex, 0);
	color = texture(u_ScreenTex, v_TexCoord);
	for (int i = 0; i < 25; i++)
	{
		vec4 texColor = texture(u_ScreenTex, v_TexCoord.xy + pixelSize * offsets[i]);
		texColor = mix(texColor * 1.5f, mix(vec4(0.5, 0.5, 0.5, 1.0), texColor, 5f), 0.5f);
		col += vec3(texColor) * kernel[i];
	}
	color += vec4(col, 1.0f)*0.1;
}