#version 330 core
layout(location = 0) in vec3 aPos; //vertex position
uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
out vec3 fragColor;
void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
    fragColor = vec3(1.0);
}
