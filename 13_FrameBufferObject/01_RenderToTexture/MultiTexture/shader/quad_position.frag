const char* quad_frag    =   STRINGIFY(
#version 330 core	\n

precision mediump float;
in vec2 v_textCoord;
uniform sampler2D s_texture;

void main()
{
	gl_FragColor = texture(s_texture, v_textCoord);
}
);