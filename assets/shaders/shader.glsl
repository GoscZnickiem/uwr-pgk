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
	vec3 color;
	float intensity;
};
struct PointLight {
	vec3 position;
	vec3 color;
	float intensity;
};
#define NUM_LIGHTS 3

in vec3 fragPosition;
in vec3 fragNormal;

layout(std140, binding = 0) uniform Camera {
    mat4 view;
    mat4 projection;
	vec3 cameraPosition;
};

layout(std140, binding = 1) uniform Lights {
	vec3 test;
	// DirectionalLight directionalLight;
	// PointLight pointLights[NUM_LIGHTS];
};

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;
uniform float opacity;

out vec4 FragColor;

vec3 dirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	vec3 halfway = normalize(lightDir + viewDir);

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuseLight = diff * light.color * light.intensity;

	float spec = pow(max(dot(normal, halfway), 0.0), shininess);
	vec3 specularLight = spec * light.color * light.intensity;

	return diffuseLight + specularLight;
}

vec3 pointLight(PointLight light, vec3 fragPos, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (1.0 + light.intensity * 5.0 * distance * distance);

    vec3 halfway = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseLight = diff * light.color * light.intensity * attenuation;

    float spec = pow(max(dot(normal, halfway), 0.0), shininess);
    vec3 specularLight = spec * light.color * light.intensity * attenuation;

    return diffuseLight + specularLight;
}

void main() {
	// vec3 viewDir = normalize(cameraPosition - fragPosition);
	// vec3 normal = normalize(fragNormal);
	//
	// vec3 color = ambient * 0.1f;
	// color += dirLight(directionalLight, normal, viewDir);
	// for(int i = 0; i < NUM_LIGHTS; i++) {
	// 	color += pointLight(pointLights[i], fragPosition, normal, viewDir);
	// }

	FragColor = vec4(test + vec3(0.1, 0.1, 0.1), opacity);
}
