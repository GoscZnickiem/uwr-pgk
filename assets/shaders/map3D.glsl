#shader vertex

#version 430 core

layout(location = 0) in int height;

uniform mat4 projection;
uniform mat4 view;
uniform ivec2 position;

out vec4 color;

const int side = 1201;
const float radius = 6378;

void main() {
	float x = (gl_VertexID % side) / float(side - 1) + float(position.x);
	float y = (side - gl_VertexID / side) / float(side - 1) + float(position.y);
	float ht = float(height << 16 >> 16) * 0.001;

	float sinx = sin(radians(x));
	float cosx = cos(radians(x));
	float siny = sin(radians(y));
	float cosy = cos(radians(y));
	vec3 pos = (radius + ht) * vec3(cosy * cosx, siny, -1 * cosy * sinx);

    gl_Position = projection * view * vec4(pos, 1.0);

	vec3 colorVec;
	if      (ht <= 0  )   colorVec = vec3(0.,       0.,        0.6);			//blue
	else if (ht <= 0.5)   colorVec = vec3(0.,       ht*2,      0.);			//->green
	else if (ht <= 1)     colorVec = vec3(ht*2-1,   1.,        0.);			//->yellow
	else if (ht <= 2)     colorVec = vec3(1.,       2.-ht,     0.);			//->red
	else                 colorVec = vec3(1.,       ht/2-1 ,ht/2-1);			//->white
	color = vec4(colorVec, 1.0);
}

#shader fragment

#version 430 core

in vec4 color;

out vec4 c;

void main() {
    c = color;
}
