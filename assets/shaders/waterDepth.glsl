#shader vertex

#version 460 core

out vec4 vcolor;
out vec2 vpos;

void main(void) {
    const vec2 vertices[6] = vec2[6](
        vec2(-0.9, -0.9),
        vec2( 0.9,  0.9),
        vec2( 0.9, -0.9),
        vec2(-0.9, -0.9),
        vec2( 0.9,  0.9),
        vec2(-0.9,  0.9)
    );
    const vec4 colors[] = vec4[6](
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(1.0, 1.0, 0.0, 1.0),
        vec4(1.0, 0.0, 0.0, 1.0),
        vec4(0.0, 1.0, 0.0, 1.0),
        vec4(0.0, 0.0, 1.0, 1.0)
    );

    vcolor = colors[gl_VertexID];
    vpos = vertices[gl_VertexID];
    gl_Position = vec4(vertices[gl_VertexID], 0.5, 1.0); 
}

#shader fragment

#version 460 core

in vec4 vcolor;
in vec2 vpos;
layout(location = 0) out vec2 depth;

void main(void) {
    if(length(vpos) < 0.1) {
        depth = vec2(1.0, 1.0);
    }
}
