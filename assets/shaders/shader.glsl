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

uniform int special;
uniform float time;

out vec3 fragPosition;
out vec3 fragNormal;

const float M_E = 2.71828182845904523536028747135266250;

uvec4 murmurHash41(uint src) {
    const uint M = 0x5bd1e995u;
    uvec4 h = uvec4(1190494759u, 2147483647u, 3559788179u, 179424673u);
    src *= M; src ^= src>>24u; src *= M;
    h *= M; h ^= src;
    h ^= h>>13u; h *= M; h ^= h>>15u;
    return h;
}

vec4 hash(float src) {
    uvec4 h = murmurHash41(floatBitsToUint(src));
    return uintBitsToFloat(h & 0x007fffffu | 0x3f800000u) - 1.0;
}

float waveF(float x) {
	return pow(M_E, sin(x) - 1);
}

float DwaveF(float x) {
	return pow(M_E, sin(x) - 1) * cos(x);
}

vec4 displacement(vec2 p) {
	float distanceToEdge = min(1.0 - abs(p.x), 1.0 - abs(p.y));
	float strength = clamp(sqrt(distanceToEdge) * 2, 0.0, 1.0);

	float sum = 0.0;
	float derX = 0.0;
	float derY = 0.0;
	float amplitude = strength * 0.2;
	float freq = 1.212;

	for(int i = 0; i < 12; i++) {
		vec4 r = hash(i * 3 + 1) * 3;
		float w = dot(r.xy, vec2(p.x, p.y * 0.7)) * freq + time * r.z + r.w;
		sum += amplitude * waveF(w);
		derX += r.x * amplitude * freq * DwaveF(w);
		derY += r.y * amplitude * freq * DwaveF(w);
		amplitude *= 0.9;
		freq *= 1.278;
	}

	vec3 tangent = vec3(1, derX, 0);
	vec3 binormal = vec3(0, derY, 1);

	return vec4(cross(binormal, tangent), sum);
}

void main() {
	vec3 pos = aPos;
	vec3 norm = aNormal;

	if(special == 2) {
		vec4 disp = displacement(pos.xz);
		pos.y = disp.w;
		norm = disp.xyz;
	}

	fragPosition = vec3(iModel * vec4(pos, 1.0));
	fragNormal = mat3(transpose(inverse(iModel))) * norm;
	gl_Position = projection * view * iModel * vec4(pos, 1.0);
}

#shader fragment

#version 460 core

struct DirectionalLight {
	vec3 direction;
	float padding;
	vec3 color;
	float intensity;
};
struct PointLight {
	vec3 position;
	float padding;
	vec3 color;
	float intensity;
};
#define NUM_LIGHTS 15

in vec3 fragPosition;
in vec3 fragNormal;

layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 projection;
	vec3 cameraPosition;
};

layout(std140, binding = 1) uniform Lights {
	DirectionalLight directionalLight;
	PointLight pointLights[NUM_LIGHTS];
	int pointLightsCount;
};

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
uniform float opacity;
uniform float alpha;
uniform int special;

uniform sampler2D depthTexture;

out vec4 FragColor;

vec3 dirLightDiff(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal, lightDir), 0.0);
	return diff * light.intensity * light.color * diffuse;
}

vec4 dirLightSpec(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	vec3 halfway = normalize(lightDir + viewDir);

	float spec = pow(max(dot(normal, halfway), 0.0), shininess);
	return spec * vec4(light.intensity * light.color * specular, 1.0);
}

vec3 pointLightDiff(PointLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPosition);
    float distance = length(light.position - fragPosition);
	float attenuation = 1.0 / (1.0 + 0.02 * distance * distance / light.intensity);

	float diff = max(dot(normal, lightDir), 0.0);
	return diff * light.intensity * light.color * diffuse * attenuation;
}

vec4 pointLightSpec(PointLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPosition);
	vec3 halfway = normalize(lightDir + viewDir);
    float distance = length(light.position - fragPosition);
	float attenuation = 1.0 / (1.0 + 0.3 * distance * distance / light.intensity);

	float spec = pow(max(dot(normal, halfway), 0.0), shininess);
	return spec * vec4(light.intensity * light.color * specular, 1.0) * attenuation;
}

vec4 waterDepthEffect(float dist, float farDist, float nearDist) {
	float distInWater = max((min(dist, farDist) - nearDist), 0) / 40;
	float f = 1 - pow(2.0, -(2.0 * distInWater));
	vec3 c = vec3(0.2, 0.6, 0.7);
	return vec4(c, f);
}

vec4 lighten(vec4 a, vec4 b) {
	float alp = a.a + (1.0 - a.a) * b.a;
	vec3 c = (a.rgb * a.a + b.rgb * b.a) / alp;
	return vec4(c, alp);
}

vec3 blend(vec3 a, vec3 b, float t) {
	return a * (1 - t) + b * t;
}



void main() {
	vec3 viewDir = normalize(cameraPosition - fragPosition);
	vec3 normal = normalize(fragNormal);

	vec3 ambientLight = ambient * 0.1f * opacity;

	vec3 diffuseLight = dirLightDiff(directionalLight, normal, viewDir);
	for(int i = 0; i < pointLightsCount; i++) {
		diffuseLight += pointLightDiff(pointLights[i], normal, viewDir);
	}

	vec4 res = vec4(ambientLight + diffuseLight, opacity);
	vec4 specularLight = dirLightSpec(directionalLight, normal, viewDir);
	for(int i = 0; i < pointLightsCount; i++) {
		vec4 l = pointLightSpec(pointLights[i], normal, viewDir);
		specularLight.rgb += l.rgb;
		specularLight.a = specularLight.a + l.a - specularLight.a * l.a;
	}
	res = lighten(res, specularLight);

	vec2 ndcCoords = gl_FragCoord.xy / vec2(textureSize(depthTexture, 0));
	vec2 depths = texture(depthTexture, ndcCoords).rg;
	vec4 waterColor = waterDepthEffect(gl_FragCoord.z/gl_FragCoord.w, depths.g, depths.r);

	FragColor = vec4(blend(res.rgb, waterColor.rgb, waterColor.w), res.a * alpha);
}
