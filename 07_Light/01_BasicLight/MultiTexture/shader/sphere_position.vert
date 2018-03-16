const char* sphere_vert    =   STRINGIFY(
#version 330            \n

layout(location = 0) in vec3	a_position;
layout(location = 1) in vec2	a_textcoord;
layout(location = 2) in vec3	a_normal;

uniform mat4					u_projectionMatrix;
uniform mat4					u_viewMatrix;
uniform mat4					u_modelMatrix;

out vec3						v_fragPos;
out	vec2						v_textcoord;
out	vec3						v_fragNoraml;

void main()
{
	gl_Position			= u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1);
	// 在世界坐标系中指定片元位置
	v_fragPos			= vec3(u_modelMatrix * vec4(a_position, 1.0));
	v_textcoord			= a_textcoord;
	mat3 normalMatrix	= mat3(transpose(inverse(u_modelMatrix)));
	v_fragNoraml		= normalMatrix * a_normal;
}
);
