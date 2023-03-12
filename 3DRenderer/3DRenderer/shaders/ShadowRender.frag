#version 330 core

in vec3 v_Normal;
in vec3 v_CamPos;
in vec3 v_Pos;
in vec4 v_LightPos;

uniform sampler2D u_ShadowMap;

out vec4 color;

void main()
{
	float shadow = 1.0f;
	vec3 lightCoords = v_LightPos.xyz / v_LightPos.w;

	if (lightCoords.z <= 1.0f)
	{
		lightCoords = (lightCoords + 1.0f) / 2.0f;

		float closestDepth = texture(u_ShadowMap, lightCoords.xy).r;
		float currentDepth = lightCoords.z;

		float bias = 0.05f;
		if (currentDepth > closestDepth + bias)
			shadow = 0.0f;
	}

	//color.y = lightCoords.z;

	color = vec4(shadow, shadow, shadow, 1.0f);
	
}