const char* sphere_vert    =   STRINGIFY(
#version 330             \n

layout(location = 0) in vec3	a_position;
layout(location = 1) in vec3	a_color;
layout(location = 2) in vec2	a_textcoord;
uniform mat4					u_mvpMatrix;
out	vec3						v_vertexColor;
out	vec2						v_textcoord2;

void main()
{
	v_textcoord2	= a_textcoord;
	v_vertexColor	= a_color;
	gl_Position		= u_mvpMatrix * vec4(a_position, 1);
}
);
