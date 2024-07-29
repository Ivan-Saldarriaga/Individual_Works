#version 330 core

in vec3 fragPos;

out vec4 FragColor;

uniform vec3 cameraPos; // Camera position from vertex shader

void main()
{
    float distance = length(fragPos - cameraPos);
    float alpha = 1 / ( 1 * pow(10.0 * length(gl_PointCoord - vec2(0.5, 0.5)), 2));
    vec3 color;
    if (distance < 2.0)
        color = vec3(1.0, 0.0, 0.0); // Red
    else if (distance < 3.0)
        color = vec3(0.0, 1.0, 0.0); // Green
    else
        color = vec3(0.0, 0.0, 1.0); // Blue

    FragColor = vec4(color, alpha);
}
