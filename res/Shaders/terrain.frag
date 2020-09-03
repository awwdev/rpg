#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 inColors;
layout(location = 1) in vec4 inShadowCoord;
layout(location = 2) in float inShadowDot;

//layout(binding  = 0) uniform sampler2D shadowMap;
layout(binding  = 0) uniform sampler2DShadow shadowMap;

void main() 
{
	//sampler2DShadow
	float shadow = 0;
	const float scale = 0.0002;
	for(float y = -2; y <= 2; ++y) {
	for(float x = -2; x <= 2; ++x) {
		vec3 off = vec3(x * scale, y * scale, 0);
		shadow += texture(shadowMap, inShadowCoord.xyz + off).r;
	}}
	shadow = 1 - (shadow / 25);

	//sampler2DShadow

	//float dist = 1 - texture(shadowMap, coords.st).r;
	//if (dist < coords.z)
	//	shadow = 1;

	const float AMBIENT = 0.1;
	outColor = vec4(inColors.rgb * (AMBIENT + shadow * inShadowDot), 1);
}