#shader vertex

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

layout(std140) uniform Camera {
    mat4 view;
    mat4 projection;
};

uniform mat4 model;

out vec3 color;
out vec3 pos;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	color = aColor;
	pos = aPos + 0.5;
}

#shader fragment

#version 460 core

in vec3 color;
in vec3 pos;

uniform float time;

out vec4 FragColor;

//https://iquilezles.org/articles/palettes/
vec3 palette( float t ) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263,0.416,0.557);

    return a + b*cos( 6.28318*(c*t+d) );
}

//https://www.shadertoy.com/view/mtyGWy
void main() {
	float len = length(pos - 1);
    float len0 = len;
    vec3 finalColor = vec3(0.0);

	for (float i = 0.0; i < 2.0; i++) {
        len = len * 0.5 - 1.5;

        float d = len * exp(-len0);

        vec3 col = palette(len0 + i*.4 + time*.4);

        d = sin(d * 8. + time) / 8.;
        d = abs(d);

        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }

	FragColor = vec4(finalColor * 0.3 + color, 1.0);
}
