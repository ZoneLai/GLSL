const char* skybox_vert    =   STRINGIFY(
#version 330            \n

layout(location = 0) in vec3	a_position;

uniform mat4					u_projectionMatrix;
uniform mat4					u_viewMatrix;
uniform mat4					u_modelMatrix;

out	vec3						v_textcoord;

void main()
{
	gl_Position		= u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0);
	v_textcoord		= a_position;
}
);
