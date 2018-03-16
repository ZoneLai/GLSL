const char* skybox_vert    =   STRINGIFY(
#version 330            \n

layout(location = 0) in vec3	a_position;

uniform mat4					u_projectionMatrix;
uniform mat4					u_viewMatrix;
uniform mat4					u_modelMatrix;

out	vec3						v_textcoord;

void main()
{
	vec4 pos		= u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0);
	gl_Position		= pos.xyww;		// 此处让z=w 则对应的深度值变为depth = w / w = 1.0
	v_textcoord		= a_position;	// 当立方体中央处于原点时 立方体上位置即等价于向量
}
);
