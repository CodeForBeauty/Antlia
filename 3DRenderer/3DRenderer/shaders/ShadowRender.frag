#version 330 core

in vec3 v_Normal;
in vec3 v_CamPos;
in vec3 v_Pos;
in vec4 v_LightPos;
in vec4 v_ScreenPos;

uniform sampler2D u_ShadowMap;
uniform sampler2D u_ShadowRender;
uniform bool u_HasPrevious;
uniform float u_Bias;

out vec4 color;

void main()
{
	float shadow = 1.0f;
	vec3 lightCoords = v_LightPos.xyz / v_LightPos.w;
	lightCoords.z /= -v_LightPos.w;
	vec2 screenSpace = (v_ScreenPos.xy / v_ScreenPos.w) * 0.5f + 0.5f;

	float previous = 1.0f;

	lightCoords = (lightCoords + 1.0f) / 2.0f;

	if (lightCoords.z <= 1.0f)
	{

		float closestDepth = texture(u_ShadowMap, lightCoords.xy).r;
		if (u_HasPrevious)
			previous = texture(u_ShadowRender, screenSpace).r;
		float currentDepth = lightCoords.z;

		int sampleRadius = 1;
		vec2 pixelSize = 1.0f / textureSize(u_ShadowMap, 0);
		float isShadow = 0.0f;
		for (int y = -sampleRadius; y <= sampleRadius; y++)
		{
			for (int x = -sampleRadius; x <= sampleRadius; x++)
			{
				shadow = 1.0f;
				closestDepth = texture(u_ShadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				currentDepth = lightCoords.z;
				if (currentDepth > closestDepth + u_Bias)
					shadow = 0.0f + previous / 2;
				if (previous < 0.9)
					shadow = (previous + shadow) / 2;
				shadow -= (1 - previous) * 0.5;
				shadow = 1 - shadow;
				isShadow += shadow;
			}
		}
		isShadow /= pow((sampleRadius * 2 + 1), 2);

		shadow = (1 - isShadow);
	}

	color = vec4(shadow, shadow, shadow, 1.0f);
}