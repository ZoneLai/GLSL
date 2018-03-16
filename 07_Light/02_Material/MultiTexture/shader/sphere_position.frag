const char* sphere_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in vec3	v_fragPos;
in vec2	v_textcoord;
in vec3	v_fragNoraml;

// 材质属性结构体
struct MaterialAttr {
	vec3	ambient;	// 环境光
	vec3	diffuse;	// 漫反射光
	vec3	specular;   // 镜面光
	float	shininess;	//镜面高光系数
};

// 光源属性结构体
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
	// 环境光成分
	vec3	ambient		= u_light.ambient * u_material.ambient;

	// 漫反射光成分 此时需要光线方向为指向光源
	vec3	lightDir	= normalize(u_light.position - v_fragPos);
	vec3	normal		= normalize(v_fragNoraml);
	float	diffFactor	= max(dot(lightDir, normal), 0.0);
	vec3	diffuse		= diffFactor * u_light.diffuse * u_material.diffuse;

	// 镜面反射成分 此时需要光线方向为由光源指出
	vec3	reflectDir	= normalize(reflect(-lightDir, normal));
	vec3	viewDir		= normalize(u_viewPos - v_fragPos);
	float	specFactor	= pow(max(dot(reflectDir, viewDir), 0.0), u_material.shininess); // shininess镜面高光系数
	vec3	specular	= specFactor * u_light.specular * u_material.specular;
	vec3	result		= ambient + diffuse + specular;

	gl_FragColor		= vec4(result, 1.0f);
}
);