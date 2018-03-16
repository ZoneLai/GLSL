const char* quad_vert    =   STRINGIFY(
#version 330	\n

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_textCoord;

out vec2 v_textCoord;

void main()
{
	gl_Position = vec4(a_position.x, a_position.y, 0.0, 1.0); // 在NDC中不需要变换
	v_textCoord = a_textCoord;
}
);