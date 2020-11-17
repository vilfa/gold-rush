#version 330 core
//out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform float near;
uniform float far;

float LinearizeDepth(float depth)
{
    float ndc = depth * 2.0 - 1.0; // Back to normalized device coordinates
    float linearDepth = ((2.0 * near * far) / (far + near - ndc * (far - near)));
    return linearDepth;
}

void main()
{    
//    FragColor = texture(texture1, TexCoords);
//    FragColor = texture(texture1, TexCoords);
    float fragColorDepth = gl_FragCoord.z;
    float depth = LinearizeDepth(fragColorDepth) / far;
//    gl_FragColor = texture(texture1, TexCoords);
    gl_FragColor = vec4(vec3(depth), 1.0);
}