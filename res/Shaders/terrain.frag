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
	mat4 test;
} ubo;

layout(push_constant) uniform Push {
    mat4 projection;
    mat4 view;
    mat4 sunView;
    vec3 sunDir;
} push;

const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0 
);

void main() 
{
	float D = 0.00001f; 
    float Z = 0.01f;

	float S = 0.005;
    mat4 sunProj = mat4( 
        S, 0, 0, 0,
        0, S, 0, 0,
        0, 0, D, 0,
        0, 0, Z, 1 
    );
	vec4 shadowCoords0 = (biasMat * sunProj * push.sunView) * inPos;

	S = 0.050;
    sunProj = mat4( 
        S, 0, 0, 0,
        0, S, 0, 0,
        0, 0, D, 0,
        0, 0, Z, 1 
    );
	vec4 shadowCoords1 = (biasMat * sunProj * push.sunView) * inPos;

	float shadow = 0;
	const float scale = 0.0002;
	for(float y = -2; y <= 2; ++y) {
	for(float x = -2; x <= 2; ++x) {
		vec2 off = vec2(x * scale, y * scale);

		vec4 coord0   = vec4(shadowCoords0.xy + off, 0, shadowCoords0.z);
		vec4 coord1   = vec4(shadowCoords1.xy + off, 1, shadowCoords1.z);
		float shadow0 = texture(shadowMap, coord0).r;
		float shadow1 = texture(shadowMap, coord1).r;

		float v0 = abs(inViewPos.z) / 50;
		float n0 = clamp(v0, 0, 1);

		float lerp0 = n0 * shadow0 + (1-n0) * shadow1; 
		shadow += lerp0;
	}}
	shadow = 1 - (shadow / 25);

	const float AMBIENT = 0.1;
	//outColor = vec4(inColors.rgb * (AMBIENT + shadow * inShadowDot), 1);
	outColor = vec4(ubo.test[0][0], ubo.test[0][1], ubo.test[0][2], 1);
}

//layout(binding  = 0) uniform sampler2D shadowMap;

//float dist = 1 - texture(shadowMap, coords.st).r;
//if (dist < coords.z)
//	shadow = 1;