#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 inColors;
layout(location = 1) in vec4 inShadowCoord;

layout(binding  = 1) uniform sampler2D shadowMap;

////https://github.com/SaschaWillems/Vulkan/tree/master/data/shaders/glsl/shadowmapping

float textureProj(vec4 shadowCoord, vec2 off)
{
	float shadow = 0.1f;
	if ( shadowCoord.z > -1.0 && shadowCoord.z < 1.0 ) 
	{
		float dist = texture( shadowMap, shadowCoord.st + off ).r;
		if ( shadowCoord.w > 0.0 && dist < shadowCoord.z ) 
		{
			shadow = 1.0f;
		}
	}
	return shadow;
}

float filterPCF(vec4 sc)
{
	ivec2 texDim = textureSize(shadowMap, 0);
	float scale = 1.5;
	float dx = scale * 1.0 / float(texDim.x);
	float dy = scale * 1.0 / float(texDim.y);

	float shadowFactor = 0.0;
	int count = 0;
	int range = 1;
	
	for (int x = -range; x <= range; x++)
	{
		for (int y = -range; y <= range; y++)
		{
			shadowFactor += textureProj(sc, vec2(dx*x, dy*y));
			count++;
		}
	
	}
	return shadowFactor / count;
}

void main() 
{
    float shadow = filterPCF(inShadowCoord / inShadowCoord.w);

    outColor = vec4(inColors.rgb * shadow, 1);
}