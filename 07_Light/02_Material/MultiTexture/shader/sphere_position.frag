const char* sphere_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in vec3	v_fragPos;
in vec2	v_textcoord;
in vec3	v_fragNoraml;

// �������Խṹ��
struct MaterialAttr {
	vec3	ambient;	// ������
	vec3	diffuse;	// �������
	vec3	specular;   // �����
	float	shininess;	//����߹�ϵ��
};

// ��Դ���Խṹ��
struct LightAttr {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform MaterialAttr	u_material;
uniform LightAttr		u_light;
uniform vec3			u_viewPos;

void main()
{
	// ������ɷ�
	vec3	ambient		= u_light.ambient * u_material.ambient;

	// �������ɷ� ��ʱ��Ҫ���߷���Ϊָ���Դ
	vec3	lightDir	= normalize(u_light.position - v_fragPos);
	vec3	normal		= normalize(v_fragNoraml);
	float	diffFactor	= max(dot(lightDir, normal), 0.0);
	vec3	diffuse		= diffFactor * u_light.diffuse * u_material.diffuse;

	// ���淴��ɷ� ��ʱ��Ҫ���߷���Ϊ�ɹ�Դָ��
	vec3	reflectDir	= normalize(reflect(-lightDir, normal));
	vec3	viewDir		= normalize(u_viewPos - v_fragPos);
	float	specFactor	= pow(max(dot(reflectDir, viewDir), 0.0), u_material.shininess); // shininess����߹�ϵ��
	vec3	specular	= specFactor * u_light.specular * u_material.specular;
	vec3	result		= ambient + diffuse + specular;

	gl_FragColor		= vec4(result, 1.0f);
}
);