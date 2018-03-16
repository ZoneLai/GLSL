const char* sphere_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in vec3	v_fragPos;
in vec2	v_textcoord;
in vec3	v_fragNoraml;

// 材质属性结构体
struct MaterialAttr {
	sampler2D	diffuseMap;	// 使用纹理对象根据片元位置取不同的材质属性
	sampler2D	specularMap;
	float		shininess;		//镜面高光系数
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
	vec3	ambient		= u_light.ambient * vec3(texture(u_material.diffuseMap, v_textcoord));
	// 漫反射光成分 此时需要光线方向为指向光源
	vec3	lightDir	= normalize(u_light.position - v_fragPos);
	vec3	normal		= normalize(v_fragNoraml);
	float	diffFactor	= max(dot(lightDir, normal), 0.0);
	vec3	diffuse		= diffFactor * u_light.diffuse * vec3(texture(u_material.diffuseMap, v_textcoord));
	// 镜面反射成分 此时需要光线方向为由光源指出
	vec3	reflectDir	= normalize(reflect(-lightDir, normal));
	vec3	viewDir		= normalize(u_viewPos - v_fragPos);
	float	specFactor	= pow(max(dot(reflectDir, viewDir), 0.0), u_material.shininess); // shininess镜面高光系数
	vec3	specular	= specFactor * u_light.specular * vec3(texture(u_material.specularMap, v_textcoord));
	vec3	result		= ambient + diffuse + specular;

	gl_FragColor		= vec4(result, 1.0f);
}
);