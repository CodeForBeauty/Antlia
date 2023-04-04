#version 330 core

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_CamPos;
in vec3 v_Pos;
in mat3 v_TBN;
in vec4 v_ScreenPos;

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

uniform sampler2D u_ShadowDir;
uniform sampler2D u_ShadowSpot;
uniform sampler2D u_ShadowPoint;

out vec4 color;

const float Pi = 3.14159265359;
const float gamma = 2.2f;


float distributionGGX(float nDotH, float roughness)
{
	float a = max(roughness * roughness, 0.01);
	float a2 = a * a;
	float denom = nDotH * nDotH * (a2 - 1.0) + 1.0;
	denom = Pi * denom * denom;
	return a2 / max(denom, 0.000001);
}

float geometrySmith(float nDotV, float nDotL, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	float ggx1 = nDotV / (nDotV * (1.0 - k) + k);
	float ggx2 = nDotL / (nDotL * (1.0 - k) + k);
	return ggx1 * ggx2;
}

vec3 freshnelSchlick(float hDotV, vec3 baseReflectivity)
{
	return baseReflectivity + (1.0 - baseReflectivity) * pow(1.0 - hDotV, 5.0);
}


vec3 pointLight(vec3 lightColor, vec3 lightVec, float intensity, float distance,
				vec3 albedo, float specular, float roughness, float metalic,
				vec3 normal, vec3 vector, vec3 reflectivity, float shadow)
{
	vec3 lightDir = normalize(lightVec);
	vec3 H = normalize(vector + lightDir);

	float dist = length(lightVec);
	float a = 0.1 / distance;
	float b = 0.1;
	float intens = intensity / (a * (dist * dist) + b * dist + 1.0f);
	vec3 radiance = lightColor * intens;

	float reflectDir = max(dot(normal, vector), 0.0000001);
	float lightReflect = max(dot(normal, lightDir), 0.0000001);
	float HdotV = max(dot(H, vector), 0.0);
	float NdotH = max(dot(normal, H), 0.0);

	float distrib = distributionGGX(NdotH, roughness);
	float geometry = geometrySmith(reflectDir, lightReflect, roughness);
	vec3 freshnel = freshnelSchlick(HdotV, reflectivity);

	vec3 spec = distrib * geometry * freshnel;
	spec /= 4.0 * reflectDir * lightReflect;
	spec *= specular;

	vec3 KD = vec3(1.0) - freshnel;
	KD *= 1.0 - metalic;

	return (KD * albedo / Pi + spec * shadow) * radiance * lightReflect * shadow;
}

vec3 directLight(vec3 lightColor, vec3 lightVec, float intensity,
				vec3 albedo, float specular, float roughness, float metalic,
				vec3 normal, vec3 vector, vec3 reflectivity, float shadow)
{
	vec3 lightDir = normalize(lightVec);
	vec3 H = normalize(vector + lightDir);

	float reflectDir = max(dot(normal, vector), 0.0000001);
	float lightReflect = max(dot(normal, lightDir), 0.0000001);
	float HdotV = max(dot(H, vector), 0.0);
	float NdotH = max(dot(normal, H), 0.0);

	float distrib = distributionGGX(NdotH, roughness);
	float geometry = geometrySmith(reflectDir, lightReflect, roughness);
	vec3 freshnel = freshnelSchlick(HdotV, reflectivity);

	vec3 spec = distrib * geometry * freshnel;
	spec /= 4.0 * reflectDir * lightReflect;
	spec *= specular;

	vec3 KD = vec3(1.0) - freshnel;
	KD *= 1.0 - metalic;

	return (KD * albedo / Pi + spec * shadow) * (intensity * lightColor) * lightReflect * shadow;
}

vec3 spotLight(vec3 lightColor, vec3 lightVec, vec3 dir, float distance, float intensity, float innerCone, float outerCone,
				vec3 albedo, float specular, float roughness, float metalic,
				vec3 normal, vec3 vector, vec3 reflectivity, float shadow)
{
	float dist = length(lightVec);
	float a = 0.1 / distance;
	float b = 0.1;
	float intens = intensity / (a * (dist * dist) + b * dist + 1.0f);
	//float intens = intensity / (a * (dist * dist) + 1.0f);
	vec3 radiance = lightColor * intens;

	vec3 lightDir = normalize(lightVec);
	vec3 H = normalize(vector + lightDir);

	float reflectDir = max(dot(normal, vector), 0.0000001);
	float lightReflect = max(dot(normal, -lightDir), 0.0000001);
	float HdotV = max(dot(H, vector), 0.0);
	float NdotH = max(dot(normal, H), 0.0);

	float distrib = distributionGGX(NdotH, roughness);
	float geometry = geometrySmith(reflectDir, lightReflect, roughness);
	vec3 freshnel = freshnelSchlick(HdotV, reflectivity);

	vec3 spec = distrib * geometry * freshnel;
	spec /= 4.0 * reflectDir * lightReflect;
	spec *= specular;

	vec3 KD = vec3(1.0) - freshnel;
	KD *= 1.0 - metalic;

	float angle = dot(dir, lightDir);
	float cone = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (KD * albedo / Pi + spec * shadow) * cone * radiance * lightReflect * shadow;
}

void main()
{
	vec4 albedo = (texture(u_Texture, v_TexCoord) * u_UseTex) + (u_Color * (1 - u_UseTex));
	float specular = (texture(u_SpecularTexture, v_TexCoord).r * u_UseSpecTex) + (u_Specular * (1 - u_UseSpecTex));
	float metalic = (texture(u_MetalicTexture, v_TexCoord).r * u_UseMetalTex) + (u_Metalic * (1 - u_UseMetalTex));
	float roughness = (texture(u_RoughnessTexture, v_TexCoord).r * u_UseRoughTex) + (u_Roughness * (1 - u_UseRoughTex));
	//float normal = (texture(u_NormalTexture, v_TexCoord) * u_UseSpecTex) + (u_Specular * (1 - u_UseSpecTex))
	vec3 normal;
	if (u_UseNormTex == 1)
	{
		normal = vec3(texture(u_NormalTexture, v_TexCoord) * 2 - 1);
		normal = v_TBN * normal;
	}
	else
		normal = normalize(v_Normal);

	vec3 vector = normalize(v_Pos - v_CamPos);

	vec3 reflectivity = mix(vec3(0.04), vec3(albedo), metalic);

	int lightCount = 1;
	vec3 totalLight = vec3(0, 0, 0);
	float shadow1 = texture(u_ShadowDir, (v_ScreenPos.xy / v_ScreenPos.w + 1.0f) / 2.0f).r;
	float shadow2 = texture(u_ShadowSpot, (v_ScreenPos.xy / v_ScreenPos.w + 1.0f) / 2.0f).r;
	float shadow3 = texture(u_ShadowPoint, (v_ScreenPos.xy / v_ScreenPos.w + 1.0f) / 2.0f).r;
	for (int i = 0; i < 8; i++)
	{
		if (u_PointLightPos[i].w > 0)
		{
			vec3 lightVec = vec3(u_PointLightPos[i]) - v_Pos;
			totalLight += pointLight(vec3(u_PointLightColor[i]), lightVec, u_PointLightColor[i].w, u_PointLightPos[i].w,
									vec3(albedo), specular, roughness, metalic, normal, vector, reflectivity, shadow3);
			lightCount++;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (u_DirectLightColor[i].w > 0)
		{
			totalLight += directLight(vec3(u_DirectLightColor[i]), -u_DirectLightDir[i], u_DirectLightColor[i].w,
									vec3(albedo), specular, roughness, metalic, normal, vector, reflectivity, shadow1);
			lightCount++;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (u_SpotLightColor[i].w > 0)
		{
			vec3 lightVec = vec3(u_SpotLightPos[i]) - v_Pos;
			totalLight += spotLight(vec3(u_SpotLightColor[i]), lightVec, u_SpotLightDir[i], u_SpotLightPos[i].w, 
									u_SpotLightColor[i].w, u_SpotLightAngle[i].x, u_SpotLightAngle[i].y,
									vec3(albedo), specular, roughness, metalic, normal, vector, reflectivity, shadow2);
			lightCount++;
		}
	}

	vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0) * albedo;
	
	//shadow.x = 1.0f;

	color = ambient + vec4(totalLight, 1.0);
	color.rgb = pow(color.rgb, vec3(1 / gamma));
	//color = vec4(1.0f);
}