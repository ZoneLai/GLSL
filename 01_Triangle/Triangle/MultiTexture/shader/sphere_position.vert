const char* sphere_vert    =   STRINGIFY(
#version 330             \n

layout(location = 0) in vec3	a_position;

void main()
{
	gl_Position	= vec4(a_position, 1);
}
);
