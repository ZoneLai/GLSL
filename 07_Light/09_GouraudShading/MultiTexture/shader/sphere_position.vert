const char* sphere_vert    =   STRINGIFY(
#version 330            \n

layout(location = 0) in vec3	a_position;
layout(location = 1) in vec2	a_textcoord;
layout(location = 2) in vec3	a_normal;

uniform mat4					u_projectionMatrix;
uniform mat4					u_viewMatrix;
uniform mat4					u_modelMatrix;

out	vec3						v_vertexColor;
uniform vec3					u_lightPos;
uniform vec3					u_lightColor;
uniform vec3					u_objectColor;
uniform vec3					u_viewPos;

void main()
{
	gl_Position					= u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0);
	vec3 Position				= vec3(u_modelMatrix * vec4(a_position, 1.0)); // ����������ϵ��ָ��ƬԪλ��
	mat3 normalMatrix			= mat3(transpose(inverse(u_modelMatrix)));
	vec3 Normal					= normalize(normalMatrix * a_normal);			// ���㷨��������ģ�ͱ任��ֵ

	// per-vertex shading
	// ������ɷ�
	float	ambientStrength		= 0.1f;
	vec3	ambient				= ambientStrength * u_lightColor;

	// �������ɷ� ��ʱ��Ҫ���߷���Ϊָ���Դ
	vec3	lightDir			= normalize(u_lightPos - Position);
	float	diffFactor			= max(dot(lightDir, Normal), 0.0f);
	vec3	diffuse				= diffFactor * u_lightColor;
	
	// ���淴��ɷ� ��ʱ��Ҫ���߷���Ϊ�ɹ�Դָ��
	float	specularStrength	= 1.0f;
	vec3	reflectDir			= normalize(reflect(-lightDir, Normal));
	vec3	viewDir				= normalize(u_viewPos - Position);
	float	specFactor			= pow(max(dot(reflectDir, viewDir), 0.0), 32); // 32Ϊ����߹�ϵ��
	vec3	specular			= specularStrength * specFactor * u_lightColor;

	v_vertexColor				= (ambient + diffuse + specular ) * u_objectColor;
}
);
