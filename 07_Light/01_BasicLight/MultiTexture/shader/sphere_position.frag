const char* sphere_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in vec3	v_fragPos;
in vec2	v_textcoord;
in vec3	v_fragNoraml;

uniform vec3 u_lightPos;
uniform vec3 u_lightColor;
uniform vec3 u_objectColor;
uniform vec3 u_viewPos;

void main()
{
	// 环境光成分
	float	ambientStrength		= 0.3f;
	vec3	ambient				= ambientStrength * u_lightColor;

	// 漫反射光成分 此时需要光线方向为指向光源
	vec3	lightDir			= normalize(u_lightPos - v_fragPos);
	vec3	normal				= normalize(v_fragNoraml);
	float	diffFactor			= max(dot(lightDir, normal), 0.0);
	vec3	diffuse				= diffFactor * u_lightColor;

	// 镜面反射成分 此时需要光线方向为由光源指出
	float	specularStrength	= 0.5f;
	vec3	reflectDir			= normalize(reflect(-lightDir, normal));
	vec3	viewDir				= normalize(u_viewPos - v_fragPos);
	float	specFactor			= pow(max(dot(reflectDir, viewDir), 0.0), 32); // 32为镜面高光系数
	vec3	specular			= specularStrength * specFactor * u_lightColor;
	vec3	result				= (ambient + diffuse + specular ) * u_objectColor;

	gl_FragColor				= vec4(result, 1.0f);
}
);