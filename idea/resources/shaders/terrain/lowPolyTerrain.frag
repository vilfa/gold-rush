#version 420 core

struct DirectionalLight
{
	/*
	* Directional light imitates the sun. The sun is so 
	* far away that it is omnipresent from a specific
	* direction. 
	*/
	vec3 direction;

	/*
	* The three light components of the Phong lighting model
	*/
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

layout (std140, binding = 1) uniform Camera
{
	vec3 cameraPos;
};

layout (std140, binding = 2) uniform WorldLights
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VS_OUT
{
	vec3 fragPos;
    vec3 fragNormal;
    vec3 fragColor;
} fs_in;

DirectionalLight light;

vec3 CalculateDirectionalPhong(DirectionalLight light, vec3 fragPos, vec3 fragNormal, vec3 fragColor, vec3 cameraPos);

void main()
{
	light.direction = direction;
	light.ambient = ambient;
	light.diffuse = diffuse;
	light.specular = specular;

	vec3 glFragColor = CalculateDirectionalPhong(light, fs_in.fragPos, fs_in.fragNormal, fs_in.fragColor, cameraPos);
    gl_FragColor = vec4(glFragColor, 1.0);
}

vec3 CalculateDirectionalPhong(DirectionalLight light, vec3 fragPos, vec3 fragNormal, vec3 fragColor, vec3 cameraPos)
{
	vec3 sourceDir = normalize(-light.direction);
	vec3 reflectDir = reflect(sourceDir, fragNormal);
	vec3 cameraDir = normalize(cameraPos - fragPos);
	
	float specularComp = dot(reflectDir, cameraDir);

	vec3 ambient = light.ambient * vec3(fragColor);
	vec3 diffuse = light.diffuse * (sourceDir * fragNormal) * vec3(fragColor);
	vec3 specular = light.specular * pow(specularComp, 1) * vec3(fragColor);
	
	return (ambient + diffuse + specular);
}