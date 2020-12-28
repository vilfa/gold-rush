#version 330 core

in vec2 TextureCoordinates;

uniform sampler2D screenTexture;

void main()
{
	// EFFECTS
//	gl_FragColor = texture(screenTexture, TextureCoordinates);

	//--- Inversion ---//
//	gl_FragColor = vec4(vec3(1.0 - texture(screenTexture, TextureCoordinates)), 1.0);

	//--- Grayscale ---//
//	vec4 tex = texture(screenTexture, TextureCoordinates);
////	float avg = (tex.r + tex.g + tex.b) / 3.0;
//	float avg = tex.r * 0.2126 + tex.g * 0.7152 + tex.b * 0.0722; // The human eye is most sensitive to green and least sensitive to blue.
//	gl_FragColor = vec4(vec3(avg), 1.0);

	//--- Sharpen, Blur, Edge-enhance (using kernels) ---//
    const float offset = 1.0 / 300.0;

	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    // Sharpen kernel
//    float kernel[9] = float[](
//        -1, -1, -1,
//        -1,  9, -1,
//        -1, -1, -1
//    );

    // Blur kernel
//    float kernel[9] = float[](
//        1.0 / 16, 2.0 / 16, 1.0 / 16,
//        2.0 / 16, 4.0 / 16, 2.0 / 16,
//        1.0 / 16, 2.0 / 16, 1.0 / 16  
//    );

    // Edge-enance kernel
    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TextureCoordinates.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    gl_FragColor = vec4(col, 1.0);
}