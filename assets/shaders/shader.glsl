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

out vec3 fragPosition;
out vec3 fragNormal;

void main() {
	fragPosition = vec3(iModel * vec4(aPos, 1.0));
	fragNormal = mat3(transpose(inverse(iModel))) * aNormal;
	gl_Position = projection * view * iModel * vec4(aPos, 1.0);
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

vec4 water(float dist, float farDist, float nearDist) {
	float distInWater = (min(dist, farDist) - nearDist) / 40;
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
	vec4 waterColor = water(gl_FragCoord.z/gl_FragCoord.w, depths.g, depths.r);

	FragColor = vec4(blend(res.rgb, waterColor.rgb, waterColor.a), res.a * alpha);
}
