#version 330 core

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_CamPos;
in vec3 v_Pos;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform int u_UseTex;

uniform float u_Specular;
uniform sampler2D u_SpecularTexture;
uniform int u_UseSpecTex;

uniform float u_Metalic;
uniform sampler2D u_MetalicTexture;
uniform int u_UseMetalTex;

uniform float u_Roughness;
uniform sampler2D u_RoughnessTexture;
uniform int u_UseRoughTex;

uniform sampler2D u_NormalTexture;
uniform int u_UseNormTex;


uniform vec4 u_PointLightColor[8];
uniform vec4 u_PointLightPos[8];

uniform vec4 u_DirectLightColor[4];
uniform vec3 u_DirectLightDir[4];

uniform vec4 u_SpotLightColor[8];
uniform vec4 u_SpotLightPos[8];
uniform vec3 u_SpotLightDir[8];
uniform vec2 u_SpotLightAngle[8];

out vec4 color;


vec3 pointLight(vec3 lightColor, vec3 lightVec, float intensity, float distance)
{
	float dist = length(lightVec);
	float a = 0.1 / distance;
	float b = 0.1;
	float intens = intensity / (a * (dist * dist) + b * dist + 1.0f);

	vec3 lightDir = normalize(lightVec);
	vec3 normal = normalize(v_Normal);
	float diffuse = max(dot(normal, lightDir), 0.0);

	vec3 camPos = normalize(v_CamPos - v_Pos);
	vec3 reflectionDir = reflect(-lightDir, v_Normal);
	float specAmount = pow(max(dot(camPos, reflectionDir), 0.0), 16);
	
	return (diffuse + specAmount * u_Specular * (texture(u_SpecularTexture, v_TexCoord).r * u_UseSpecTex)) * intens * lightColor;
}

vec3 directLight(vec3 lightColor, vec3 lightVec, float intensity)
{
	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(lightVec);
	float diffuse = max(dot(v_Normal, -lightDir), 0.0f);

	vec3 camPos = normalize(v_CamPos - v_Pos);
	vec3 reflectionDir = reflect(lightDir, v_Normal);
	float specAmount = pow(max(dot(camPos, reflectionDir), 0.0f), 16);
	float specular = u_Specular * specAmount;

	return (diffuse + specular * texture(u_SpecularTexture, v_TexCoord).r * u_UseSpecTex) * intensity * lightColor;
}

vec3 spotLight(vec3 lightColor, vec3 lightVec, vec3 dir, float distance, float intensity, float innerCone, float outerCone)
{
	float dist = length(lightVec);
	float a = 0.1 / distance;
	float intens = intensity / (a * (dist * dist) + 1.0f);

	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(lightVec);
	float diffuse = max(dot(v_Normal, lightDir), 0.0f);

	vec3 camPos = normalize(v_CamPos - v_Pos);
	vec3 reflectionDir = reflect(-lightDir, v_Normal);
	float specAmount = pow(max(dot(camPos, reflectionDir), 0.0f), 16);
	float specular = u_Specular * specAmount;

	float angle = dot(dir, -lightDir);
	float cone = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
	//float cone = clamp((1 - angle) / (1 - (innerCone - outerCone)), 0.0f, 1.0f);

	return (diffuse + specular * texture(u_SpecularTexture, v_TexCoord).r * u_UseSpecTex) * cone * lightColor;
}

void main()
{
	int lightCount = 1;
	vec3 totalLight = vec3(0, 0, 0);
	for (int i = 0; i < 8; i++)
	{
		if (u_PointLightPos[i].w > 0)
		{
			vec3 lightVec = vec3(u_PointLightPos[i]) - v_Pos;
			totalLight += pointLight(vec3(u_PointLightColor[i]), lightVec, u_PointLightColor[i].w, u_PointLightPos[i].w);
			lightCount++;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (u_DirectLightColor[i].w > 0)
		{
			totalLight += directLight(vec3(u_DirectLightColor[i]), u_DirectLightDir[i], u_DirectLightColor[i].w);
			lightCount++;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (u_SpotLightColor[i].w > 0)
		{
			vec3 lightVec = vec3(u_SpotLightPos[i]) - v_Pos;
			totalLight += spotLight(vec3(u_SpotLightColor[i]), lightVec, u_SpotLightDir[i], u_SpotLightPos[i].w, 
									u_SpotLightColor[i].w, u_SpotLightAngle[i].x, u_SpotLightAngle[i].y);
			lightCount++;
		}
	}
	
	color = ((texture(u_Texture, v_TexCoord) * u_UseTex) + (u_Color * (1 - u_UseTex))) * vec4(totalLight / lightCount + 0.2, 1);//spotLight(vec3(1, 0, 0), lightVec, vec3(0, -1, 0), 5, 1);
}