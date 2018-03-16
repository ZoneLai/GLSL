const char* sphere_vert    =   STRINGIFY(
#version 330             \n

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_color;
out vec3 v_vertexColor;

void main()
{
	gl_Position		= vec4(a_position, 1);
	v_vertexColor	= a_color;
}
);
