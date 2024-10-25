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

out vec4 FragColor;

void main() {
	FragColor = vec4(color, 1.0);
}
