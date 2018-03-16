const char* quad_frag    =   STRINGIFY(
#version 330            \n

precision mediump float;
in vec2 v_textCoord;
uniform sampler2D s_texture;

vec3 inversion()  // ��ɫ
{
	return vec3(1.0 - texture(s_texture, v_textCoord));
}

vec3 grayscale()  // �ҶȻ�
{
	vec4 color		= texture(s_texture, v_textCoord);
	float average	= 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b; // ȡȨ�ؼ���
	return vec3(average, average, average);
}

// sharpenЧ��kernel
float sharpenKernel[9] = float[](
	-1.0, -1.0, -1.0,
	-1.0,  9.0, -1.0,
	-1.0, -1.0, -1.0
);

// ģ��Ч��kernel
float blurKernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

// edge-detectionЧ��kernel
float edgeDetectionKernel[9] = float[](
	1.0, 1.0, 1.0,
	1.0, -8.0, 1.0,
	1.0, 1.0, 1.0
);

// ����Ч��
float embossKernel[9] = float[](
	-2.0, -1.0, 0.0,
	-1.0,  1.0, 1.0,
	 0.0,  1.0, 2.0
);

const float offset = 1.0 / 300;		// 9��λ�õ���������ƫ����
vec3 kernelEffect(float kernel[9])  // ����Kernel�������Ч��
{
   // ȷ��9��λ�õ�ƫ����
   vec2 offsets[9] = vec2[](
        vec2(-offset, offset),  // top-left ���Ϸ�
        vec2(0.0f,    offset),  // top-center ���Ϸ�
        vec2(offset,  offset),  // top-right ���Ϸ�
        vec2(-offset, 0.0f),    // center-left �м����
        vec2(0.0f,    0.0f),    // center-center ����λ��
        vec2(offset,  0.0f),    // center-right �м��ұ�
        vec2(-offset, -offset), // bottom-left �ײ����
        vec2(0.0f,    -offset), // bottom-center �ײ��м�
        vec2(offset,  -offset)  // bottom-right  �ײ��ұ�
    );
	
	// ����9��λ�õ�����
	vec3 sampleText[9];
	for(int i = 0; i < 9; ++i)
	{
		sampleText[i] = vec3(texture(s_texture, v_textCoord.st + offsets[i]));
	}

	// ����Ȩֵ������������ɫ
	vec3 result = vec3(0.0);
	for(int i = 0; i < 9; ++i)
	{
		result += sampleText[i] * kernel[i];
	}
	return result;
}

void main()
{
	gl_FragColor = vec4(kernelEffect(edgeDetectionKernel), 1.0);
	// vec3 color		= kernelEffect(edgeDetectionKernel);
	// float average	= 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b; // ȡȨ�ؼ���
	// gl_FragColor = vec4(average, average, average, 1.0);
}
);