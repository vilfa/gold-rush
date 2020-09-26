#version 330 core

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Light light;
uniform Material material;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightColor;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
	// Ambient lighting component
	vec3 ambient = light.ambient * material.ambient;
	
	// Normalize fragment normal
	vec3 fragNormal = normalize(Normal);

	// Diffuse lighting component
	vec3 lightDirection = normalize(light.position - FragPos);
	float diffuseFactor = max(dot(fragNormal, lightDirection), 0.0); // make sure the dot product is never negative
	vec3 diffuse = light.diffuse * (diffuseFactor * material.diffuse);

	// Specular lighting component
	vec3 viewDirection = normalize(viewPos - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, fragNormal);
	float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess); // higher power = more gloss
	vec3 specular = light.specular * (specularFactor * material.specular);

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.00);
}