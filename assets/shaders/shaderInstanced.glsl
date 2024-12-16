#shader vertex

#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in mat4 iModel;

layout(std140) uniform Camera {
    mat4 view;
    mat4 projection;
};

uniform float boardSize;
uniform vec3 boardCenter;

flat out vec3 color;
out vec2 uv;

void main() {
	gl_Position = projection * view * iModel * vec4(aPos, 1.0);
	color = aColor * (iModel[3].xyz - boardCenter + boardSize/2) / boardSize * 2.0 + 0.3;
	uv = aPos.xy;
}

#shader fragment

#version 430 core

flat in vec3 color;
in vec2 uv;

uniform float time;

out vec4 FragColor;

//https://www.shadertoy.com/view/Mfcyzs
vec2 GetGradient(vec2 intPos, float t) {
	float rand = fract(sin(dot(intPos, vec2(12.9898, 78.233))) * 43758.5453);;
	float angle = 6.283185 * rand + 4.0 * t * rand;
	return vec2(cos(angle), sin(angle));
}


float Pseudo3dNoise(vec3 pos) {
	vec2 i = floor(pos.xy);
	vec2 f = pos.xy - i;
	vec2 blend = f * f * (3.0 - 2.0 * f);
	float noiseVal = 
		mix(
			mix(
				dot(GetGradient(i + vec2(0, 0), pos.z), f - vec2(0, 0)),
				dot(GetGradient(i + vec2(1, 0), pos.z), f - vec2(1, 0)),
				blend.x),
			mix(
				dot(GetGradient(i + vec2(0, 1), pos.z), f - vec2(0, 1)),
				dot(GetGradient(i + vec2(1, 1), pos.z), f - vec2(1, 1)),
				blend.x),
			blend.y
		);

	return noiseVal;
}

void main() {
	const float SMOOTHSTEP_WIDTH = 0.03;
	const float BAND_WIDTH = 0.25;
	const float LOWER_BAND_START = 0.5 - BAND_WIDTH;
	const float UPPER_BAND_START = 0.5 + BAND_WIDTH;
	const float NUM_BANDS = 10.0;
	const float SIZE = 1.0;

	float t = time * 0.025;

	float noiseVal = 0.5 + 0.5 * Pseudo3dNoise(vec3(uv * SIZE, t));

	float waves = 1.0 - 
		smoothstep(LOWER_BAND_START - SMOOTHSTEP_WIDTH, LOWER_BAND_START + SMOOTHSTEP_WIDTH, fract((abs(noiseVal) * NUM_BANDS) + t)) *
		smoothstep(UPPER_BAND_START + SMOOTHSTEP_WIDTH, UPPER_BAND_START - SMOOTHSTEP_WIDTH, fract((abs(noiseVal) * NUM_BANDS) + t));


	vec3 w = vec3(waves);

	FragColor = vec4(w * 0.04 + color, 1.0);
}
