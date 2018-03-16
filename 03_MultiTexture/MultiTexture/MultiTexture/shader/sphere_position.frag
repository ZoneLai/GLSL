const char* sphere_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in	vec3			v_vertexColor;
in	vec2			v_textcoord2;
uniform sampler2D	s_texture1;
uniform sampler2D	s_texture2;
uniform float		u_mixValue;

void main()
{
	// vec4 v_color1	= texture(s_texture1, v_textcoord2) * vec4(v_vertexColor, 1.0f);	// ÑÕÉ«
	vec4 v_color1	= texture(s_texture1, v_textcoord2);
    vec4 v_color2	= texture(s_texture2, v_textcoord2);
	gl_FragColor	= mix(v_color1, v_color2, u_mixValue);
}
);