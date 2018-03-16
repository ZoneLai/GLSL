const char* lamp_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;

void main()
{
	gl_FragColor	= vec4(0.0f, 0.4f, 0.1f, 1.0f);
}
);