#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 inColors;
layout(location = 1) in vec4 inShadowCoord;
layout(location = 2) in float inShadowDot;
layout(location = 3) in vec3 inUV;

//UBO has binding 0
layout(binding  = 1) uniform sampler2D shadowMap;
layout(binding  = 2) uniform sampler2DArray textures;

////https://github.com/SaschaWillems/Vulkan/tree/master/data/shaders/glsl/shadowmapping
const float AMBIENT = 0.1;

float textureProj(vec4 shadowCoord, vec2 off)
{
	if (shadowCoord.z > -1.0 && shadowCoord.z < 1.0) 
	{
		float dist = texture( shadowMap, shadowCoord.st + off ).r;
		if ( shadowCoord.w > 0.0 && dist < shadowCoord.z ) {
			return 1.0f;
		}
	}
	return 0.0f;
}

float filterPCF(vec4 sc)
{
	ivec2 texDim = textureSize(shadowMap, 0);
	float scale = 0.1;
	float dx = scale * 1.0 / float(texDim.x);
	float dy = scale * 1.0 / float(texDim.y);

	float shadowFactor = 0.0;
	int count = 0;
	int range = 12;
	
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

	//TODO: why is it offsetted this way ?!?!?
	vec3 _uv  = vec3(inUV.x + 0.25, inUV.y, 0);
	float val = texture(textures, _uv).b;
	if (val < 0.1) 
		discard;

	outColor = vec4(inColors.rgb * (AMBIENT + shadow), 1);
}