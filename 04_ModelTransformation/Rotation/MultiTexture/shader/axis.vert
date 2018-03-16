const char* axis_vert    =   STRINGIFY(
#version 330             \n

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;

out vec3 v_vertColor;

void main()
{
	gl_Position = vec4(a_position, 1.0);
	v_vertColor = a_color;
}
);