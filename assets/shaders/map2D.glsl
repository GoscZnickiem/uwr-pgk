#shader vertex

#version 430 core

layout(location = 0) in int height;

uniform vec2 cameraPos;
uniform vec2 scale;
uniform ivec2 position;
uniform uint side;
uniform uint lod;

out vec4 color;

void main() {
	uint x = (gl_VertexID % side - (side - 1)/2) * lod + position.x * (side - 1);
	uint y = ((side - 1)/2 - gl_VertexID / side) * lod + position.y * (side - 1);

	vec2 pos = vec2(x, y) / lod * scale - cameraPos;

    gl_Position = vec4(pos, 0.0, 1.0);

	float ht = float(height << 16 >> 16);
	vec3 colorVec;
	if      (ht < 0  )   colorVec = vec3(0.,       0.,        1.);			//blue
	else if (ht < 500)   colorVec = vec3(0.,       ht/500,    0.);			//->green
	else if (ht < 1000)  colorVec = vec3(ht/500-1, 1.,        0.);			//->yellow
	else if (ht < 2000)  colorVec = vec3(1.,       2.-ht/1000,0.);			//->red
	else                 colorVec = vec3(1.,       ht/2000-1 ,ht/2000-1);  //->white
	color = vec4(colorVec, 1.0);
}

#shader fragment

#version 430 core

in vec4 color;

out vec4 c;

void main() {
    c = color;
}
