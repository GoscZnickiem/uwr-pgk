#shader vertex

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in mat4 iModel;

layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 projection;
	vec3 cameraPosition;
};

void main() {
	gl_Position = projection * view * iModel * vec4(aPos, 1.0);
}

#shader fragment

#version 460 core

uniform int side;

out vec4 depth;

void main() {
	float d = gl_FragCoord.z / gl_FragCoord.w;
	if(side == 0) {
		depth = vec4(d, 0.0, 0.0, 1.0);
	} else {
		depth = vec4(0.0, d, 0.0, 1.0);
	}
}
