#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 inColors;
layout(location = 1) in vec4 inShadowCoord;
layout(binding  = 0) uniform sampler2D shadowMap;

void main() 
{
    float shadow = 1.0;
	if (inShadowCoord.z > -1.0 && inShadowCoord.z < 1.0) 
	{
		float dist = texture(shadowMap, inShadowCoord.st).r;
		if (inShadowCoord.w > 0.0 && dist < inShadowCoord.z) 
		{
			shadow = 0.1f;
		}
	}

    outColor = vec4(inColors.rgb * shadow, 1);
}