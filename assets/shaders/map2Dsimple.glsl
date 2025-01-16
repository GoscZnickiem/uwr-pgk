#shader vertex

#version 430 core

layout(location = 0) in int height;

uniform vec2 cameraPos;
uniform vec2 scale;
uniform ivec2 position;

out vec4 color;

const int side = 1201;

void main() {
	float x = (gl_VertexID % side) / float(side - 1) + float(position.x);
	float y = (side - gl_VertexID / side) / float(side - 1) + float(position.y);
	
	float scaleX = cos(radians(cameraPos.y));

	vec2 pos = (vec2(scaleX * (x - cameraPos.x), y - cameraPos.y)) * scale;

    gl_Position = vec4(pos, 0.0, 1.0);

	float ht = float(height);
	vec3 colorVec;
	if      (ht <= 0  )   colorVec = vec3(0.,       0.,        1.);			//blue
	else if (ht <= 500)   colorVec = vec3(0.,       ht/500,    0.);			//->green
	else if (ht <= 1000)  colorVec = vec3(ht/500-1, 1.,        0.);			//->yellow
	else if (ht <= 2000)  colorVec = vec3(1.,       2.-ht/1000,0.);			//->red
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
