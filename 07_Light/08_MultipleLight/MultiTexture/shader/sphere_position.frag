const char* sphere_frag    =   STRINGIFY(
#version 330				\n
#define POINT_LIGHT_NUM 4	\n

precision mediump float;
in vec3	v_fragPos;
in vec2	v_textcoord;
in vec3	v_fragNoraml;

// �������Խṹ��
struct MaterialAttr {
	sampler2D	diffuseMap;		// ʹ������������ƬԪλ��ȡ��ͬ�Ĳ�������
	sampler2D	specularMap;
	float		shininess;		//����߹�ϵ��
};

// �����Դ���Խṹ��
struct DirLightAttr {
	vec3 direction;	// �����Դ
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// ���Դ���Խṹ��
struct PointLightAttr {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;		// ˥������
	float linear;		// ˥��һ��ϵ��
	float quadratic;	// ˥������ϵ��
};

// �۹�ƹ�Դ���Խṹ��
struct SpotLightAttr {
	vec3	position;		// �۹�Ƶ�λ��
	vec3	direction;		// �۹�Ƶ�spot direction
	float	cutoff;			// �۹���ڲ��Žǵ�����ֵ
	float	outerCutoff;	// �۹�������Žǵ�����ֵ
	vec3	ambient;
	vec3	diffuse;
	vec3	specular;

	float	constant;		// ˥������
	float	linear;			// ˥��һ��ϵ��
	float	quadratic;		// ˥������ϵ��
};

uniform MaterialAttr	u_material;
uniform DirLightAttr	u_dirLight;
uniform PointLightAttr	u_pointLights[POINT_LIGHT_NUM]; // ������Դ����
uniform SpotLightAttr	u_spotLight;
uniform vec3			u_viewPos;

// �����ԴЧ���ĺ�������
vec3 calculateDirLight(DirLightAttr light, vec3 fragNormal, vec3 fragPos, vec3 viewPos);
vec3 calculatePointLight(PointLightAttr light, vec3 fragNormal, vec3 fragPos, vec3 viewPos);
vec3 calculateSpotLight(SpotLightAttr light, vec3 fragNormal, vec3 fragPos, vec3 viewPos);

void main()
{
	vec3 result		= calculateDirLight(u_dirLight, v_fragNoraml, v_fragPos, u_viewPos);
	for(int i = 0; i < POINT_LIGHT_NUM; ++i) {
		result += calculatePointLight(u_pointLights[i], v_fragNoraml, v_fragPos, u_viewPos);
	}
	result		   += calculateSpotLight(u_spotLight, v_fragNoraml, v_fragPos, u_viewPos);
	gl_FragColor	= vec4(result, 1.0f);
}

// �����Դ���㷽��
vec3 calculateDirLight(DirLightAttr light, vec3 fragNormal, vec3 fragPos, vec3 viewPos)
{
	// ������ɷ�
	vec3	ambient		= light.ambient * vec3(texture(u_material.diffuseMap, v_textcoord));

	// �������ɷ� ��ʱ��Ҫ���߷���Ϊָ���Դ
	vec3	normal		= normalize(fragNormal);
	vec3	lightDir	= normalize(-light.direction);	// ��ת�����Դ�ķ���
	float	diffFactor	= max(dot(lightDir, normal), 0.0);
	vec3	diffuse		= diffFactor * light.diffuse * vec3(texture(u_material.diffuseMap, v_textcoord));

	// ���淴��ɷ� ��ʱ��Ҫ���߷���Ϊ�ɹ�Դָ��
	float	specularStrength = 0.5f;
	vec3	reflectDir	= normalize(reflect(-lightDir, normal));
	vec3	viewDir		= normalize(viewPos - fragPos);
	float	specFactor	= pow(max(dot(reflectDir, viewDir), 0.0), u_material.shininess);
	vec3	specular	= specFactor * light.specular * vec3(texture(u_material.specularMap, v_textcoord));

	vec3	result		= ambient + diffuse + specular;
	return result;
}

// ���Դ���㷽��
vec3 calculatePointLight(PointLightAttr light, vec3 fragNormal, vec3 fragPos,vec3 viewPos)
{
	// ������ɷ�
	vec3	ambient		= light.ambient * vec3(texture(u_material.diffuseMap, v_textcoord));

	// �������ɷ� ��ʱ��Ҫ���߷���Ϊָ���Դ
	vec3	lightDir	= normalize(light.position - fragPos);
	vec3	normal		= normalize(fragNormal);
	float	diffFactor	= max(dot(lightDir, normal), 0.0);
	vec3	diffuse		= diffFactor * light.diffuse * vec3(texture(u_material.diffuseMap, v_textcoord));

	// ���淴��ɷ� ��ʱ��Ҫ���߷���Ϊ�ɹ�Դָ��
	float	specularStrength = 0.5f;
	vec3	reflectDir	= normalize(reflect(-lightDir, normal));
	vec3	viewDir		= normalize(viewPos - fragPos);
	float	specFactor	= pow(max(dot(reflectDir, viewDir), 0.0), u_material.shininess);
	vec3	specular	= specFactor * light.specular * vec3(texture(u_material.specularMap, v_textcoord));

	// ����˥������
	float distance		= length(light.position - fragPos); // ����������ϵ�м������
	float attenuation	= 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	vec3	result		= (ambient + diffuse + specular) * attenuation;
	return result;
}

// �۹�Ƽ��㷽��
vec3 calculateSpotLight(SpotLightAttr light,vec3 fragNormal, vec3 fragPos, vec3 viewPos)
{
	// ������ɷ�
	vec3	ambient		= light.ambient * vec3(texture(u_material.diffuseMap, v_textcoord));

	vec3	lightDir	= normalize(light.position - fragPos);
	vec3	normal		= normalize(fragNormal);

	// �������ɷ� ��ʱ��Ҫ���߷���Ϊָ���Դ
	
	float	diffFactor	= max(dot(lightDir, normal), 0.0);
	vec3	diffuse		= diffFactor * light.diffuse * vec3(texture(u_material.diffuseMap, v_textcoord));

	// ���淴��ɷ� ��ʱ��Ҫ���߷���Ϊ�ɹ�Դָ��
	float	specularStrength = 0.5f;
	vec3	reflectDir	= normalize(reflect(-lightDir, normal));
	vec3	viewDir		= normalize(viewPos - fragPos);
	float	specFactor	= pow(max(dot(reflectDir, viewDir), 0.0), u_material.shininess);
	vec3	specular	= specFactor * light.specular * vec3(texture(u_material.specularMap, v_textcoord));
	
	// ���������ŽǷ�Χ�ڵ�ǿ��
	float theta			= dot(lightDir, normalize(-light.direction));// ������۹��spotDir�н�����ֵ
	float epsilon		= light.cutoff - light.outerCutoff;
	float intensity		= clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0); // ����۹�����ŽǺ����ŽǺ��ǿ��ֵ
	diffuse			   *= intensity;
	specular		   *= intensity;

	// ����˥������
	float distance		= length(light.position - fragPos); // ����������ϵ�м������
	float attenuation	= 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	ambient			   *= attenuation;
	diffuse			   *= attenuation;
	specular		   *= attenuation;

	vec3 result			= ambient + diffuse + specular;
	return result;
}
);