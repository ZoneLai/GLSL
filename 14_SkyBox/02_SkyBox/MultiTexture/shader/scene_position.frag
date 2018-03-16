const char* scene_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in	vec2			v_textcoord;
uniform sampler2D	s_texture;

void main()
{
	gl_FragColor	= texture(s_texture, v_textcoord);
}
);