#shader vertex

#version 460 core

layout (location = 0) in vec2 aPos;

uniform vec2 pos;
uniform float sin;
uniform float cos;
uniform vec2 scale;
uniform vec2 global_scale;

void main() {
	vec2 sPos = scale * aPos;
	float xsin = sPos.x * sin;
	float xcos = sPos.x * cos;
	float ysin = sPos.y * sin;
	float ycos = sPos.y * cos;
	vec2 res = (vec2(xcos - ysin, ycos + xsin) + pos) * global_scale;
	gl_Position = vec4(res, 0.0, 1.0);
}

#shader fragment

#version 460 core

uniform vec3 color;

out vec4 FragColor;

void main() {
	FragColor = vec4(color, 1.0);
}
