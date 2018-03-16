const char* scene_vert    =   STRINGIFY(
#version 330            \n

layout(location = 0) in vec3	a_position;
layout(location = 1) in vec2	a_textcoord;

uniform mat4					u_projectionMatrix;
uniform mat4					u_viewMatrix;
uniform mat4					u_modelMatrix;

out	vec2						v_textcoord;

void main()
{
	gl_Position		= u_projectionMatrix * u_viewMatrix *  u_modelMatrix * vec4(a_position, 1.0);
	v_textcoord		= a_textcoord;
}
);
