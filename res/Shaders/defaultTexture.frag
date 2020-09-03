#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 inColors;
layout(location = 1) in vec4 inShadowCoord;
layout(location = 2) in float inShadowDot;
layout(location = 3) in vec3 inUV;

//UBO has binding 0
layout(binding  = 1) uniform sampler2DShadow shadowMap;
layout(binding  = 2) uniform sampler2DArray  textures;

void main() 
{
	//TODO: why is it offsetted this way ?!?!?
	vec3 _uv  = vec3(inUV.x + 0.25, inUV.y, 0);
	float val = texture(textures, _uv).b;
	if (val < 0.1) 
		discard;

	float shadow = 0;
	const float scale = 0.0002;
	for(float y = -1; y <= 1; ++y) {
	for(float x = -1; x <= 1; ++x) {
		vec3 off = vec3(x * scale, y * scale, 0);
		shadow += texture(shadowMap, inShadowCoord.xyz + off).r;
	}}
	shadow = 1 - (shadow / 9);

	const float AMBIENT = 0.1;
	outColor = vec4(inColors.rgb * (AMBIENT + shadow), 1);
}