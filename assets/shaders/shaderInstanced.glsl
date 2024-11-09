#shader vertex

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in mat4 iModel;

layout(std140) uniform Camera {
    mat4 view;
    mat4 projection;
};

out vec3 color;

void main() {
	gl_Position = projection * view * iModel * vec4(aPos, 1.0);
	color = aColor;
}

#shader fragment

#version 460 core

in vec3 color;

out vec4 FragColor;

void main() {
	FragColor = vec4(color, 1.0);
}
