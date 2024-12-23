#shader vertex

#version 460 core

layout(location = 0) in vec2 position;

out vec2 texCoords;

void main() {
    texCoords = (position + vec2(1.0)) / 2.0;
    gl_Position = vec4(position, 0.0, 1.0);
}

#shader fragment

#version 460 core

in vec2 texCoords;

uniform sampler2D depthTexture;

out vec4 color;

void main() {
    vec2 depths = texture(depthTexture, texCoords).rg;
    color = vec4(depths.r, depths.g, 0.0, 1.0);
}
