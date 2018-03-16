const char* scene_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in vec3				v_fragNormal;
in vec3				v_fragPos;
uniform samplerCube	s_texture;	// 环境纹理
uniform vec3		u_cameraPos;

void main()
{
	vec3 viewDir	= normalize(v_fragPos - u_cameraPos);	// 注意这里向量从观察者位置指出
	vec3 reflectDir = reflect(viewDir, normalize(v_fragNormal));
	gl_FragColor	= texture(s_texture, reflectDir);		// 使用反射向量采样环境纹理
}
);