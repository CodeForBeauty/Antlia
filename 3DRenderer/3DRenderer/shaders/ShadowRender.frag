#version 330 core

in vec3 v_Normal;
in vec3 v_CamPos;
in vec3 v_Pos;
in vec4 v_LightPos;
in vec4 v_ScreenPos;

uniform sampler2D u_ShadowMap;
uniform sampler2D u_ShadowRender;
uniform bool u_HasPrevious;

out vec4 color;

void main()
{
	float shadow = 1.0f;
	vec3 lightCoords = v_LightPos.xyz / v_LightPos.w;
	vec2 screenSpace = (v_ScreenPos.xy / v_ScreenPos.w) * 0.5f + 0.5f;

	float previous = 1.0f;

	if (lightCoords.z <= 1.0f)
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;

		float closestDepth = texture(u_ShadowMap, lightCoords.xy).r;
		if (u_HasPrevious)
			previous = texture(u_ShadowRender, screenSpace).r;
		float currentDepth = lightCoords.z;

		float bias = 0.05f;
		if (currentDepth > closestDepth + bias)
			shadow = 0.0f + previous / 2;
		if (previous < 0.9)
			shadow = previous / 2 + shadow / 2;
		//shadow -= 1 - previous;
		//shadow = previous + 0.1;
	}

	//color.y = lightCoords.z;

	color = vec4(shadow, shadow, shadow, 1.0f);
	//color = vec4((v_ScreenPos.xy / v_ScreenPos.w) * 0.5f + 0.5f, 0.0f, 1.0f);
}