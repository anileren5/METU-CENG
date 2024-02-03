#version 330 core

in vec4 color;
in vec4 worldPosition; // Interpolated world coordinates from the vertex shader
out vec4 fragColor;

void main(void)
{
    // Determine the checkerboard pattern based on the world coordinates
    bool isWhiteSquare = (mod(float(int(worldPosition.x) / 20), 2.0) < 1.0) !=
                         (mod(float(int(worldPosition.y) / 20), 2.0) < 1.0);

    // Set the color of this fragment to white or black based on the checkerboard pattern
    fragColor = color;//isWhiteSquare ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
}
