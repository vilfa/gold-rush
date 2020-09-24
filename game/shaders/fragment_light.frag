#version 330 core

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
	// Ambient lighting component
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse lighting component
	vec3 fragNormal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - FragPos);
	float diffuseStrength = max(dot(fragNormal, lightDirection), 0.0); // make sure the dot product is never negative
	vec3 diffuse = diffuseStrength * lightColor;

	float specularStrength = 0.5;
	vec3 viewDirection = normalize(viewPos - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, fragNormal);
	float specularComp = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specular = specularComp * specularStrength * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.00);
}