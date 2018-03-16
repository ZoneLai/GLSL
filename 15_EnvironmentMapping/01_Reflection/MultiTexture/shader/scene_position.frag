const char* scene_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in vec3				v_fragNormal;
in vec3				v_fragPos;
uniform samplerCube	s_texture;	// ��������
uniform vec3		u_cameraPos;

void main()
{
	vec3 viewDir	= normalize(v_fragPos - u_cameraPos);	// ע�����������ӹ۲���λ��ָ��
	vec3 reflectDir = reflect(viewDir, normalize(v_fragNormal));
	gl_FragColor	= texture(s_texture, reflectDir);		// ʹ�÷�������������������
}
);