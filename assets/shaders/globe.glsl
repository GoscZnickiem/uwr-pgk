#shader vertex

#version 430 core

out vec4 screenPos;

void main() {
	const vec2 vertices[6] = vec2[6](
		vec2(-1, -1),
		vec2( 1, -1),
		vec2( 1,  1),
		vec2(-1, -1),
		vec2( 1,  1),
		vec2(-1,  1)
	);
	screenPos = vec4(vertices[gl_VertexID], 0.0, 1.0);
    gl_Position = screenPos;
}

#shader fragment

#version 430 core

in vec4 screenPos;

uniform mat4 camInverse;
uniform vec3 camPos;
uniform vec2 resolution;

const float earthRadius = 6378;

out vec4 fragColor;

void main() {
    vec4 worldRay = camInverse * screenPos;
    worldRay /= worldRay.w;

    vec3 rayDir = normalize(worldRay.xyz - camPos);

    float a = dot(rayDir, rayDir);
    float b = 2.0 * dot(camPos, rayDir);
    float c = dot(camPos, camPos) - earthRadius * earthRadius;

    float delta = b * b - 4.0 * a * c;
    if(delta < 0.0) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    float t = (-b - sqrt(delta)) / (2.0 * a);
	// float t = 2 * c / (-b + sqrt(delta));
	if(t < 0) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
	}
    vec3 hitPoint = camPos + t * rayDir;

    float latitude = degrees(asin(hitPoint.y / earthRadius));
    float longitude = degrees(atan(hitPoint.z, hitPoint.x));

	float latProximity = abs(fract(latitude * 0.2) - 0.5);
	float lonProximity = abs(fract(longitude * 0.2) - 0.5);

	float latLine = 1.0 - smoothstep(0.0, 0.01, latProximity);
	float lonLine = 1.0 - smoothstep(0.0, 0.01, lonProximity);
	float colorBonus = (latLine + lonLine) / 2;

	float fade = smoothstep(0.0, 1.0, clamp((length(camPos) - 16 - earthRadius) / 20.0, 0.0, 1.0));

	fragColor = fade * (vec4(0.0, 0.0, 0.6, 1.0) + vec4(1.0, 1.0, 1.0, 1.0) * colorBonus);
}
