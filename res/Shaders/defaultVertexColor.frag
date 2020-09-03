#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 inColors;
layout(location = 1) in vec4 inShadowCoord;
layout(location = 2) in float inShadowDot;

layout(binding  = 1) uniform sampler2D shadowMap;

void main() 
{
	//float shadow = filterPCF(inShadowCoord / inShadowCoord.w);
	//float shadow = textureProj(inShadowCoord / inShadowCoord.w, vec2(0, 0));

	const float AMBIENT = 0.1;
	outColor = vec4(inColors.rgb * (AMBIENT + 1 * inShadowDot), 1);
}