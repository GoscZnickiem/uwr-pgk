#shader vertex

#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 iPos;
layout (location = 3) in vec2 iAngle;
layout (location = 4) in vec2 iScale;

uniform vec2 global_scale;

out vec3 color;

void main() {
	vec2 sPos = iScale * aPos;
	float xsin = sPos.x * iAngle[0];
	float xcos = sPos.x * iAngle[1];
	float ysin = sPos.y * iAngle[0];
	float ycos = sPos.y * iAngle[1];
	vec2 res = (vec2(xcos - ysin, ycos + xsin) + iPos) * global_scale;
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

out vec4 FragColor;

vec3 lerp(vec3 a, vec3 b, float t) {
	return (1 - t) * a + t * b;
}

void main() {
	vec2 fragPosNDC = (gl_FragCoord.xy / global_resolution) * 2.0 - 1.0;

	float d1 = distance(playerPos1 * global_scale, fragPosNDC);
	float d2 = 1.5 * distance(playerPos2 * global_scale, fragPosNDC);
	float d3 = distance(playerPos3 * global_scale, fragPosNDC);
	float m = d1 + d2 + d3;
	float t = clamp(1/(m * m * m) * 0.15, 0.0, 1.0);

	vec3 altColor = vec3(1.0, 0.6, 0.0);

	FragColor = vec4(lerp(color, altColor, t), 1.0);
}
