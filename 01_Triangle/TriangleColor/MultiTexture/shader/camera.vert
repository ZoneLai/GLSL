const char* camera_play_vert = STRINGIFY(

layout(location = 0) in vec3	a_Position;
layout(location = 1) in vec3	a_Color;
uniform mat4                    u_MvpMatrix;
out vec3                        v_VetexColor;

void main()
{
	gl_Position   = vec4(a_Position, 1.0f);
	v_VetexColor  = a_Color;
}
);