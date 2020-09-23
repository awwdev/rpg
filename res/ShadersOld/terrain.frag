#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec4  inColors;
layout (location = 1) in vec4  inShadowCoord;
layout (location = 2) in float inShadowDot;
layout (location = 3) in vec4  inViewPos;
layout (location = 4) in vec4  inPos;

layout (binding  = 0) uniform sampler2DArrayShadow shadowMap;

layout (set = 0, binding = 1) uniform UBO {
	mat4 camProj;
    mat4 camView;
    mat4 sunView;
	mat4 sunCasc [3];
    vec3 sunDir;
	float cascadeFadeDist0;
	float cascadeFadeDist1;
	float cascadeFadeDist2;
} ubo;

void main() 
{
	vec4 shadowCoords0 = ubo.sunCasc[0] * inPos;
	vec4 shadowCoords1 = ubo.sunCasc[1] * inPos;
	vec4 shadowCoords2 = ubo.sunCasc[2] * inPos;

	float shadow = 0;
	const float scale = 0.0002;
	for(float y = -2; y <= 2; ++y) {
	for(float x = -2; x <= 2; ++x) {
		vec2 off = vec2(x * scale, y * scale);

		vec4 coord0   = vec4(shadowCoords0.xy + off, 0, shadowCoords0.z);
		vec4 coord1   = vec4(shadowCoords1.xy + off, 1, shadowCoords1.z);
		vec4 coord2   = vec4(shadowCoords2.xy + off, 2, shadowCoords2.z);
		float shadow0 = texture(shadowMap, coord0).r;
		float shadow1 = texture(shadowMap, coord1).r;
		float shadow2 = texture(shadowMap, coord2).r;

		float v0 = abs(inViewPos.z) / ubo.cascadeFadeDist0;
		float v1 = abs(inViewPos.z) / ubo.cascadeFadeDist1;
		float v2 = abs(inViewPos.z) / ubo.cascadeFadeDist2;
		float n0 = clamp(v0, 0, 1);
		float n1 = clamp(v1, 0, 1);
		float n2 = clamp(v2, 0, 1);

		//float shadowLerp0 = shadow0 * clamp(1 - abs(4*n - 1), 0, 1);
		//if (n < 0.25) shadowLerp0 = shadow0;
		//float shadowLerp1 = shadow1 * clamp(1 - abs(4*n - 2), 0, 1);
		//float shadowLerp2 = shadow2 * clamp(1 - abs(4*n - 3), 0, 1);

		//float shadowLerp0 = shadow0 * clamp(1 - 250*(pow(n-0.25, 4)), 0, 1);
		//if (n < 0.25) shadowLerp0 = shadow0;
		//float shadowLerp1 = shadow1 * clamp(1 - 250*(pow(n-0.50, 4)), 0, 1);
		//float shadowLerp2 = shadow2 * clamp(1 - 250*(pow(n-0.75, 4)), 0, 1);

		//1-250\left(x-0.25\right)^{4}

		//1-abs(4x-1)
		//1-abs(4x-2)
		//1-abs(4x-3)

		float shadowLerp0 = shadow0 * (1-n0);
		float shadowLerp1 = shadow1 * (1-n1);
		float shadowLerp2 = shadow2 * (1-pow(n2,10));

		shadow += clamp(shadowLerp0 + shadowLerp1 + shadowLerp2, 0, 1);
	}}
	shadow = 1 - (shadow / 25);

	const float AMBIENT = 0.1;
	outColor = vec4(inColors.rgb * (AMBIENT + shadow * inShadowDot), 1);
}

//layout(binding  = 0) uniform sampler2D shadowMap;

//float dist = 1 - texture(shadowMap, coords.st).r;
//if (dist < coords.z)
//	shadow = 1;