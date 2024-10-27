#shader vertex

#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

uniform vec2 pos;
uniform float sinv;
uniform float cosv;
uniform vec2 scale;
uniform vec2 global_scale;
uniform float global_special;

out vec3 color;

void main() {
	vec2 sPos = scale * aPos;
	float xsin = sPos.x * sinv;
	float xcos = sPos.x * cosv;
	float ysin = sPos.y * sinv;
	float ycos = sPos.y * cosv;
	vec2 res = (vec2(xcos - ysin, ycos + xsin) + pos) * global_scale;

	float angle = global_special * 5;
	res = mat2(cos(angle), sin(angle), -sin(angle), cos(angle)) * res;

	// Warping Effect
	float distanceToCenter = length(res);
	vec2 directionToCenter = normalize(-res);
	vec2 radialDistortion = directionToCenter * pow(distanceToCenter, 1.2) * global_special;

	vec2 waveDistortion = vec2(
		sin(res.y * 10.0 + global_special * 20.0),
		cos(res.x * 10.0 + global_special * 20.0)
	) * 0.1 * global_special;

	res += radialDistortion + waveDistortion;

	gl_Position = vec4(res, 0.0, 1.0);
	color = aColor;
}

#shader fragment

#version 460 core

in vec3 color;

out vec4 FragColor;

void main() {
	FragColor = vec4(color, 1.0);
}
