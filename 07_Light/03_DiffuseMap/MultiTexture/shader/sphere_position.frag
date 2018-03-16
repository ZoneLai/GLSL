const char* sphere_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in vec3	v_fragPos;
in vec2	v_textcoord;
in vec3	v_fragNoraml;

// �������Խṹ��
struct MaterialAttr {
	sampler2D	diffuseMap;	// ʹ������������ƬԪλ��ȡ��ͬ�Ĳ�������
	sampler2D	specularMap;
	float		shininess;		//����߹�ϵ��
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
	vec3	ambient		= u_light.ambient * vec3(texture(u_material.diffuseMap, v_textcoord));
	// �������ɷ� ��ʱ��Ҫ���߷���Ϊָ���Դ
	vec3	lightDir	= normalize(u_light.position - v_fragPos);
	vec3	normal		= normalize(v_fragNoraml);
	float	diffFactor	= max(dot(lightDir, normal), 0.0);
	vec3	diffuse		= diffFactor * u_light.diffuse * vec3(texture(u_material.diffuseMap, v_textcoord));
	// ���淴��ɷ� ��ʱ��Ҫ���߷���Ϊ�ɹ�Դָ��
	vec3	reflectDir	= normalize(reflect(-lightDir, normal));
	vec3	viewDir		= normalize(u_viewPos - v_fragPos);
	float	specFactor	= pow(max(dot(reflectDir, viewDir), 0.0), u_material.shininess); // shininess����߹�ϵ��
	vec3	specular	= specFactor * u_light.specular * vec3(texture(u_material.specularMap, v_textcoord));
	vec3	result		= ambient + diffuse + specular;

	gl_FragColor		= vec4(result, 1.0f);
}
);