#version 420 core

struct PointLight
{
	/*
	* Point light imitates a light bulb.
	*/
	vec3 position;

	/*
	* Point light cutoff angle in radians.
	*/
	float cutoff;

	/*
	* Attenuation implementation. Properties: lower distance/higher intensity ->
	* higher distance/lower intensity. Choose the proper factors (K) to suit the
	* light properties. The equations:
	* L(d) = 1 / A(d) -> Luminosity
	* A(d) = K_c + K_l * d + K_q * d**2 -> Attenuation
	*/
	float constant;
	float linear;
	float quadratic;

	/*
	* The three light components of the Phong lighting model
	*/
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material 
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

//-------------//

layout (std140, binding = 1) uniform Camera
{
	vec3 cameraPos;
};

in VS_OUT
{
	vec3 fragPos;
	vec3 fragNormal;
	vec2 texCoords;
} fs_in;

uniform PointLight pointLight;
uniform Material material;

vec3 CalculatePointLightPhong(PointLight light, vec3 fragNormal, vec3 viewDir);
vec3 CalculatePointLightBlinnPhong(PointLight light, vec3 fragNormal, vec3 lightDir, vec3 viewDir, vec3 halfwayDir);

void main()
{
	// Normalize fragment normal
	vec3 fragNormal = normalize(fs_in.fragNormal);

	// Calculate direction vectors
	vec3 viewDir = normalize(cameraPos - fs_in.fragPos);
	vec3 lightDir = normalize(pointLight.position - fs_in.fragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);

	/*--- Lighting ---*/
	gl_FragColor = vec4(CalculatePointLightBlinnPhong(pointLight, fragNormal, lightDir, viewDir, halfwayDir), 
						1.0);
}

vec3 CalculatePointLightPhong(PointLight light, vec3 fragNormal, vec3 lightDir, vec3 viewDir)
{
	float diffuseFactor = max(dot(fragNormal, lightDir), 0.0); // Calculate diffuse factor, make sure the dot product is never negative

	vec3 reflectDir = reflect(-lightDir, fragNormal); // Reflect the negative light direction vector over the fragment normal
	float specularFactor  = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess); // Corr. between look dir. and reflection.

	// Calculate attenuation factor
	float distanceFromSource = length(light.position - fs_in.fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distanceFromSource + light.quadratic * distanceFromSource);

	// Calculate the three Phong components
	vec3 ambient  = attenuation * light.ambient					  * vec3(texture(material.diffuse, fs_in.texCoords));
	vec3 diffuse  = attenuation * light.diffuse  * diffuseFactor  * vec3(texture(material.diffuse, fs_in.texCoords));
	vec3 specular = attenuation * light.specular * specularFactor * vec3(texture(material.specular, fs_in.texCoords));

	return (ambient + diffuse + specular);
}

vec3 CalculatePointLightBlinnPhong(PointLight light, vec3 fragNormal, vec3 lightDir, vec3 viewDir, vec3 halfwayDir)
{
	float diffuseFactor = max(dot(fragNormal, halfwayDir), 0.0); // Calculate diffuse factor, make sure the dot product is never negative

	vec3 reflectDir = reflect(-lightDir, fragNormal); // Reflect the negative light direction vector over the fragment normal
	float specularFactor  = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess); // Specular is the "correlation" between reflect and view direction (more direct look = stronger refleciton)

	// Calculate attenuation factor
	float distanceFromSource = length(light.position - fs_in.fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distanceFromSource + light.quadratic * distanceFromSource);

	// Calculate the three Blinn-Phong components
	vec3 ambient  = attenuation * light.ambient					  * vec3(texture(material.diffuse, fs_in.texCoords));
	vec3 diffuse  = attenuation * light.diffuse  * diffuseFactor  * vec3(texture(material.diffuse, fs_in.texCoords));
	vec3 specular = attenuation * light.specular * specularFactor * vec3(texture(material.specular, fs_in.texCoords));

	return (ambient + diffuse + specular);
}