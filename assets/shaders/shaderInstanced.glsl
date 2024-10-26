#shader vertex

#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 iPos;
layout (location = 3) in vec2 iAngle;
layout (location = 4) in vec2 iScale;

uniform vec2 global_scale;
uniform float global_special;

out vec3 color;

void main() {
	vec2 sPos = iScale * aPos;
	float xsin = sPos.x * iAngle[0];
	float xcos = sPos.x * iAngle[1];
	float ysin = sPos.y * iAngle[0];
	float ycos = sPos.y * iAngle[1];
	vec2 res = (vec2(xcos - ysin, ycos + xsin) + iPos) * global_scale;

	// Warping Effect
	float distanceToCenter = length(res);
	vec2 directionToCenter = normalize(-res);
	vec2 radialDistortion = directionToCenter * pow(distanceToCenter, 1.2) * global_special;

	vec2 waveDistortion = vec2(
		sin(res.y * 10.0 + global_special * 20.0),
		cos(res.x * 10.0 + global_special * 20.0)
	) * 0.1 * global_special;

	float angle = global_special * 5;
	res = mat2(cos(angle), sin(angle), -sin(angle), cos(angle)) * res;

	res += radialDistortion + waveDistortion;

	gl_Position = vec4(res, 0.0, 1.0);
	color = aColor;
}

#shader fragment

#version 460 core

in vec3 color;

uniform vec2 playerPos1;
uniform vec2 playerPos2;
uniform vec2 playerPos3;
uniform vec2 global_scale;
uniform vec2 global_resolution;
uniform vec2 global_special;

out vec4 FragColor;

vec3 lerp(vec3 a, vec3 b, float t) {
	return (1 - t) * a + t * b;
}

float distanceToSegment(vec2 p, vec2 v1, vec2 v2) {
    vec2 lineVec = v2 - v1;
    float lenSquared = dot(lineVec, lineVec);
    float t = clamp(dot(p - v1, lineVec) / lenSquared, 0.0, 1.0);
    vec2 projection = v1 + t * lineVec;
    return length(p - projection);
}

void main() {
	vec2 fragPosNDC = (gl_FragCoord.xy / global_resolution) * 2.0 - 1.0;

	vec2 p1 = playerPos1 * global_scale;
	vec2 p2 = playerPos2 * global_scale;
	vec2 p3 = playerPos3 * global_scale;

	float d1 = distanceToSegment(fragPosNDC, p1, p2);
	float d2 = distanceToSegment(fragPosNDC, p2, p3);
	float d3 = distanceToSegment(fragPosNDC, p3, p1);

	float minDistance = min(d1, min(d2, d3));

	float edgeEffect = smoothstep(0.1, 0.5, 0.005 / (minDistance * minDistance + 0.01));
	float t = clamp(edgeEffect, 0.0, 1.0);

	vec3 altColor = vec3(1.0, 0.6, 0.0);

	FragColor = vec4(lerp(color, altColor, t), 1.0);
}
