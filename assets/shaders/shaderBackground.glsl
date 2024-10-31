#shader vertex

#version 460 core

uniform vec2 global_scale;

out vec2 pos;

void main() {
	const vec2 vertices[6] = vec2[6](
		vec2(-0.9, -0.9),
		vec2( 0.9,  0.9),
		vec2( 0.9, -0.9),
		vec2(-0.9, -0.9),
		vec2( 0.9,  0.9),
		vec2(-0.9,  0.9)
	);

	gl_Position = vec4(vertices[gl_VertexID] * global_scale, 0.5, 1.0);
	pos = gl_Position.xy;
}

#shader fragment

#version 460 core

in vec2 pos;

uniform float time;
uniform float global_special;

out vec4 FragColor;

//https://iquilezles.org/articles/palettes/
vec3 palette( float t ) {
    vec3 a = vec3(0.718, 0.718, 0.718);
    vec3 b = vec3(0.258, 0.108, 0.108);
    vec3 c = vec3(1.0, 1.0, 0.5);
    vec3 d = vec3(0.608, -0.252, -0.112);

    return a + b*cos( 6.28318*(c*t+d) );
}

void main() {
	vec2 uv = pos + vec2(1.0, 1.0);

	for(float i = 1.0; i < 10.0; i++) {
		uv.y += 0.03 * cos(uv.x * i * i + time) * sin(uv.y * i + time);
		uv.x += 0.03 * sin(uv.y * i * i + time);
	}

	vec3 color = palette(uv.y + uv.x + time * 0.2 + global_special * 6);

	FragColor = vec4(color, 0.3) * 0.7;
}
