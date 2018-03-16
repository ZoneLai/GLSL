const char* sphere_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in vec3 v_vetexColor;

void main()
{
	gl_FragColor = vec4(v_vetexColor, 1.0);
}
);