#shader vertex

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in mat4 iModel;

layout(std140) uniform Camera {
    mat4 view;
    mat4 projection;
};

out vec4 color;

void main() {
	gl_Position = projection * view * iModel * vec4(aPos, 1.0);
	color = vec4(aNormal, 1.0);
}

#shader fragment

#version 460 core

in vec4 color;

out vec4 FragColor;

void main() {
	FragColor = color;
}
