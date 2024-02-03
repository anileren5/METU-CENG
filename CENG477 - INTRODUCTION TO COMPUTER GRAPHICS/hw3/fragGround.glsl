#version 330 core

in vec4 color;
in vec4 worldPosition; // Interpolated world coordinates from the vertex shader
out vec4 fragColor;
uniform float cubeZGeneral;

void main(void)
{
    // Get the fractional part of world coordinates with the added cubeZGeneral to z-coordinate only
    vec2 fracCoords = fract(vec2(worldPosition.x, worldPosition.z - cubeZGeneral) * 0.1665); // Adjust the multiplier for your desired pattern size

    // Determine the checkerboard pattern based on the fractional parts
    bool isWhiteSquare = (fracCoords.x < 0.5) != (fracCoords.y < 0.5);

    // Set the color of this fragment to white or black based on the checkerboard pattern
    fragColor = isWhiteSquare ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
}