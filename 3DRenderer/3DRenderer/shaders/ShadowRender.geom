#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 v_CamPos;
out vec3 v_Pos;
out vec4 v_LightPos;


in DATA
{
	vec3 pos;
	vec3 camPos;
	mat4 projection;
	mat4 view;
	vec4 lightPos;
} data_in[];

void main()
{
	for (int i = 0; i < 3; i++)
	{
		gl_Position = data_in[i].projection * (data_in[i].view * vec4(data_in[i].pos + data_in[i].camPos, 1));
		gl_Position.z /=  -gl_Position.w;
		//gl_Position.z = -gl_Position.z;
		v_CamPos = data_in[i].camPos;
		v_Pos = data_in[i].pos;
		v_LightPos = data_in[i].lightPos;
		EmitVertex();
	}
	EndPrimitive();
}