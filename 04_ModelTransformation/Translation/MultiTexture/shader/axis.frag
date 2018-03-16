const char* axis_frag    =   STRINGIFY(
#version 330             \n

in vec3 v_vertColor;

void main()
{
	gl_FragColor = vec4(v_vertColor, 1.0);
}
);