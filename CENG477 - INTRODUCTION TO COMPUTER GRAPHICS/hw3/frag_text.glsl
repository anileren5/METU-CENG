#version 120

varying vec2 TexCoords;
uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(text, TexCoords).r);
    // Check if the sampled alpha value is below a threshold (e.g., 0.5)
    if (sampled.a < 0.5)
        discard; // Discard fragments with low alpha (make them transparent)
    gl_FragColor = vec4(textColor, 1.0) * sampled;
}