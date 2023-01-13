#version 330 core

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_CamPos;
in vec3 v_Pos;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform int u_UseTex;

out vec4 color;

float pointLight(vec3 pos)
{
	vec3 lightVec = pos - v_Pos;
	float dist = length(lightVec);
	float a = 0.05f;
	float b = 0.01f;
	float intens = 1.0f / (a * dist * dist + b * dist + 1.0f);

	float ambient = 0.3f;

	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(lightVec);
	float diffuse = max(dot(v_Normal, lightDir), 0.0f);

	float specularLight = 0.5f;
	vec3 camPos = normalize(v_CamPos - v_Pos);
	vec3 reflectionDir = reflect(-lightDir, v_Normal);
	float specAmount = pow(max(dot(camPos, reflectionDir), 0.0f), 8);
	float specular = specularLight * specAmount;

	return (diffuse * intens + ambient + specular);
}

float directLight(vec3 dir)
{
	float ambient = 0.2f;

	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(dir);
	float diffuse = max(dot(v_Normal, -lightDir), 0.0f);

	float specularLight = 0.5f;
	vec3 camPos = normalize(v_CamPos - v_Pos);
	vec3 reflectionDir = reflect(lightDir, v_Normal);
	float specAmount = pow(max(dot(camPos, reflectionDir), 0.0f), 8);
	float specular = specularLight * specAmount;

	return (diffuse + ambient + specular);
}

float spotLight(vec3 pos, float innerCone = 0.95f, float outerCone = 0.90f)
{
	vec3 lightVec = pos - v_Pos;

	float ambient = 0.1f;

	vec3 normal = normalize(v_Normal);
	vec3 lightDir = normalize(lightVec);
	float diffuse = max(dot(v_Normal, lightDir), 0.0f);

	float specularLight = 0.5f;
	vec3 camPos = normalize(v_CamPos - v_Pos);
	vec3 reflectionDir = reflect(-lightDir, v_Normal);
	float specAmount = pow(max(dot(camPos, reflectionDir), 0.0f), 8);
	float specular = specularLight * specAmount;

	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDir);
	float intens = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	return (diffuse * angle + ambient + specular * angle);
}

void main()
{
	color = ((texture(u_Texture, v_TexCoord) * u_UseTex) + (u_Color * (1 - u_UseTex))) * directLight(vec3(0, -0.5, 0.5));
}