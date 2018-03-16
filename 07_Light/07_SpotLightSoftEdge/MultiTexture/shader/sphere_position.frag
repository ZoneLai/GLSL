const char* sphere_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in vec3	v_fragPos;
in vec2	v_textcoord;
in vec3	v_fragNoraml;

// 材质属性结构体
struct MaterialAttr {
	sampler2D	diffuseMap;		// 使用纹理对象根据片元位置取不同的材质属性
	sampler2D	specularMap;
	float		shininess;		//镜面高光系数
};

// 光源属性结构体
struct LightAttr {
	vec3	position;	// 聚光灯的位置
	vec3	direction;	// 聚光灯的spot direction
	float	cutoff;		// 聚光灯张角的cos值
	float	outerCutoff;// 聚光灯外沿张角的余弦值

	vec3	ambient;
	vec3	diffuse;
	vec3	specular;

	float	constant;	// 衰减常数
	float	linear;		// 衰减一次系数
	float	quadratic;	// 衰减二次系数
};

uniform MaterialAttr	u_material;
uniform LightAttr		u_light;
uniform vec3			u_viewPos;

void main()
{
	// 环境光成分
	vec3	ambient		= u_light.ambient * vec3(texture(u_material.diffuseMap, v_textcoord));
	vec3	lightDir	= normalize(u_light.position - v_fragPos);
	vec3	normal		= normalize(v_fragNoraml);
	
	// 漫反射光成分 此时需要光线方向为指向光源
	float	diffFactor	= max(dot(lightDir, normal), 0.0);
	vec3	diffuse		= diffFactor * u_light.diffuse * vec3(texture(u_material.diffuseMap, v_textcoord));
	
	// 镜面反射成分 此时需要光线方向为由光源指出
	vec3	reflectDir	= normalize(reflect(-lightDir, normal));
	vec3	viewDir		= normalize(u_viewPos - v_fragPos);
	float	specFactor	= pow(max(dot(reflectDir, viewDir), 0.0), u_material.shininess); // shininess镜面高光系数
	vec3	specular	= specFactor * u_light.specular * vec3(texture(u_material.specularMap, v_textcoord));
	
	// 计算内外张角范围内的强度
	float	theta		= dot(lightDir, normalize(-u_light.direction));// 光线与聚光灯spotDir夹角余弦值
	float	epsilon		= u_light.cutoff - u_light.outerCutoff;
	float	intensity	= clamp((theta - u_light.outerCutoff) / epsilon, 0.0, 1.0); // 引入聚光灯内张角和外张角后的强度值
	diffuse			   *= intensity;
	specular		   *= intensity;

	// 计算衰减因子
	float distance		= length(u_light.position - v_fragPos); // 在世界坐标系中计算距离
	float attenuation	= 1.0f / (u_light.constant + u_light.linear * distance
							+ u_light.quadratic * distance * distance);
	ambient			   *= attenuation;
	diffuse			   *= attenuation;
	specular		   *= attenuation;
	vec3	result		= (ambient + diffuse + specular) * attenuation;
	gl_FragColor		= vec4(result, 1.0f);
}
);