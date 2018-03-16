const char* camera_play_frag = STRINGIFY(

precision mediump float;
in vec3 v_VetexColor;

void main()
{
	gl_FragColor = vec4(v_VetexColor, 1.0);
}
);