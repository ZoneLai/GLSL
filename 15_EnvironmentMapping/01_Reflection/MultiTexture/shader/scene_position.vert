const char* scene_vert    =   STRINGIFY(
#version 330            \n

layout(location = 0) in vec3	a_position;
layout(location = 1) in vec3	a_normal;

uniform mat4					u_projectionMatrix;
uniform mat4					u_viewMatrix;
uniform mat4					u_modelMatrix;

out	vec3						v_fragNormal;
out vec3						v_fragPos;

void main()
{
	gl_Position			= u_projectionMatrix * u_viewMatrix *  u_modelMatrix * vec4(a_position, 1.0);
	v_fragPos			= vec3(u_modelMatrix * vec4(a_position, 1.0));	// 在世界坐标系中指定
	mat3 normalMatrix	= mat3(transpose(inverse(u_modelMatrix)));
	v_fragNormal		= normalMatrix * a_normal;						// 计算法向量经过模型变换后值
}
);
