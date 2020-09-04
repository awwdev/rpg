#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec4  inColors;
layout (location = 1) in vec4  inShadowCoord;
layout (location = 2) in float inShadowDot;

layout (binding  = 0) uniform sampler2DArrayShadow shadowMap;

void main() 
{
	float shadow = 0;
	const float scale = 0.0002;
	for(float y = -2; y <= 2; ++y) {
	for(float x = -2; x <= 2; ++x) {
		vec2 off = vec2(x * scale, y * scale);
		vec4 coord = vec4(inShadowCoord.xy + off, 0, inShadowCoord.z);
		shadow += texture(shadowMap, coord).r;
	}}
	shadow = 1 - (shadow / 25);

	const float AMBIENT = 0.1;
	outColor = vec4(inColors.rgb * (AMBIENT + shadow * inShadowDot), 1);
}

//layout(binding  = 0) uniform sampler2D shadowMap;

//float dist = 1 - texture(shadowMap, coords.st).r;
//if (dist < coords.z)
//	shadow = 1;