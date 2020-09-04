#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec4  inColors;
layout (location = 1) in vec4  inShadowCoord;
layout (location = 2) in float inShadowDot;
layout (location = 3) in vec4  inViewPos;
layout (location = 4) in vec3  inCascadeSplits;
layout (location = 5) in mat4  inSunView;
layout (location = 12) in vec4  inPos;

layout (binding  = 0) uniform sampler2DArrayShadow shadowMap;

const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0 
);

void main() 
{
	const float splits[3] =
	{
		-1,
		-10,
		-20,
	};

	uint cascadeIndex = 0;
	for(uint i = 0; i < 3; ++i) {
		if(inViewPos.z > splits[i]) {	
			++cascadeIndex;
		}
	}

	

	float S = 0.002 + (cascadeIndex * 0.01);
    float D = 0.00001f; 
    float Z = 0.01f;
    mat4 sunProj = mat4( 
        S, 0, 0, 0,
        0, S, 0, 0,
        0, 0, D, 0,
        0, 0, Z, 1 
    );

	vec4 shadowCoords = (biasMat * sunProj * inSunView) * inPos;


	

	float shadow = 0;
	const float scale = 0.0002;
	for(float y = -2; y <= 2; ++y) {
	for(float x = -2; x <= 2; ++x) {
		vec2 off = vec2(x * scale, y * scale);
		vec4 coord = vec4(shadowCoords.xy + off, cascadeIndex, shadowCoords.z);
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