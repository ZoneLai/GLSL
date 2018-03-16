const char* skybox_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in	vec3			v_textcoord;
uniform samplerCube	s_texture;

void main()
{
	gl_FragColor	= texture(s_texture, v_textcoord);
}
);