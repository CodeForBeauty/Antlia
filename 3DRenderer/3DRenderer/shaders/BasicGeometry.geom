#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_CamPos;
out vec3 v_Pos;
out mat3 v_TBN;


in DATA
{
	vec2 texCoord;
	vec3 normal;
	vec3 pos;
	vec3 camPos;
	mat4 projection;
	mat4 view;
} data_in[];

void main()
{
	vec3 edge0 = data_in[1].pos - data_in[0].pos;
	vec3 edge1 = data_in[2].pos - data_in[0].pos;
	vec2 deltaUV0 = data_in[1].texCoord - data_in[0].texCoord;
	vec2 deltaUV1 = data_in[2].texCoord - data_in[0].texCoord;

	float invDet = 1.0 / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

	vec3 tangent = invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1);
	vec3 bitangent = invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1);

	vec3 vector0 = data_in[0].pos - data_in[1].pos;
	vec3 vector1 = data_in[2].pos - data_in[1].pos;
	vec3 surfaceNormal = normalize(cross(vector0, vector1)) * 0.1;

	for (int i = 0; i < 3; i++)
	{
		gl_Position = data_in[i].projection * (data_in[i].view * vec4(data_in[i].pos + data_in[i].camPos, 1));
		gl_Position.z /= -gl_Position.w;
		v_TexCoord = data_in[i].texCoord;
		vec3 T = normalize(cross(bitangent, data_in[i].normal));
		vec3 B = normalize(cross(tangent, data_in[i].normal));
		mat3 TBN = mat3(T, B, data_in[i].normal);
		v_Normal = data_in[i].normal;
		v_TBN = TBN;
		v_CamPos = data_in[i].camPos;
		v_Pos = data_in[i].pos;
		EmitVertex();
	}
	EndPrimitive();
}