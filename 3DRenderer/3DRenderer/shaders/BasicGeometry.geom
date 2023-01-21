#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_CamPos;
out vec3 v_Pos;

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
	vec3 vector0 = data_in[0].pos - data_in[1].pos;
	vec3 vector1 = data_in[2].pos - data_in[1].pos;
	vec3 surfaceNormal = normalize(cross(vector0, vector1)) * 0.1;
	for (int i = 0; i < 3; i++)
	{
		gl_Position = data_in[i].projection * (data_in[i].view * vec4(data_in[i].pos + data_in[i].camPos + surfaceNormal, 1));
		gl_Position.z /= -gl_Position.w;
		v_TexCoord = data_in[i].texCoord;
		v_Normal = data_in[i].normal;
		v_CamPos = data_in[i].camPos;
		v_Pos = data_in[i].pos;
		EmitVertex();
	}
	EndPrimitive();
}