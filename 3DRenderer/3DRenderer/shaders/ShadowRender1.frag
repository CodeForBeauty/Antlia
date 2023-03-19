#version 330 core

in vec3 v_Normal;
in vec3 v_CamPos;
in vec3 v_Pos;
in vec4 v_LightPos;
in vec4 v_ScreenPos;

uniform samplerCube u_ShadowMap;
uniform sampler2D u_ShadowRender;
uniform bool u_HasPrevious;
uniform float u_Bias;
uniform vec3 u_Pos;
uniform float u_FarPlane;

out vec4 color;

void main()
{
	float shadow = 0.0f;
	vec3 lightUV = v_Pos - u_Pos;
	float currentDepth = length(v_Pos - u_Pos);
	vec2 screenSpace = (v_ScreenPos.xy / v_ScreenPos.w) * 0.5f + 0.5f;
	
	float previous = 1.0f;

	if (u_HasPrevious)
		previous = texture(u_ShadowRender, screenSpace).r;

	int sampleRadius = 2;
	float pixelSize = 1.0f / 2048.0f;
	for (int z = -sampleRadius; z < sampleRadius; z++)
	{
		for (int y = -sampleRadius; y < sampleRadius; y++)
		{
			for (int x = -sampleRadius; x < sampleRadius; x++)
			{
				float closestDepth = texture(u_ShadowMap, lightUV + vec3(x, y, z) * pixelSize).r;
				closestDepth *= 10;
				if (currentDepth < closestDepth + 0.05f)
					shadow += 1.0f + previous / 2;
				shadow += previous;
			}
		}
	}
	shadow /= pow((sampleRadius * 2 + 1), 3);
	//shadow = 1 - shadow;
	color = vec4(shadow, shadow, shadow, 1.0f);
	//color = vec4(currentDepth/10);//vec4(fragToLight, 1.0f);//texture(u_ShadowMap, fragToLight);
	//color = (currentDepth/10) - texture(u_ShadowMap, -lightUV);
	//color = vec4(texture(u_ShadowMap, lightUV).r);
	//color = vec4(1.0, 1.0, 1.0, 1.0);
	//if (currentDepth < texture(u_ShadowMap, -fragToLight).r)
	//	color = vec4(0.3, 0.3, 0.3, 1.0);
	//color = vec4(v_ScreenPos.xyz / v_ScreenPos.w * 0.5f + 0.5f, 1.0f);
	//color = vec4(fragToLight, 1.0f);
	//color.w = 1.0f;
	//color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}