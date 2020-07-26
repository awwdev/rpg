#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 inColors;
layout(location = 1) in vec4 inShadowCoord;
layout(binding  = 0) uniform sampler2D shadowMap;

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

void main() 
{
    float shadow = textureProj(inShadowCoord / inShadowCoord.w, vec2(0.0));

    outColor = vec4(inColors.rgb * shadow, 1);
}