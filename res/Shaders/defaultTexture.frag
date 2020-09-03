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

void main() 
{
	//TODO: why is it offsetted this way ?!?!?
	vec3 _uv  = vec3(inUV.x + 0.25, inUV.y, 0);
	float val = texture(textures, _uv).b;
	if (val < 0.1) 
		discard;

	const float AMBIENT = 0.1;
	//float shadow = textureProj(inShadowCoord / inShadowCoord.w, vec2(0, 0));
	//float shadow = filterPCF(inShadowCoord / inShadowCoord.w);
	outColor = vec4(inColors.rgb * (AMBIENT + 1), 1);
}