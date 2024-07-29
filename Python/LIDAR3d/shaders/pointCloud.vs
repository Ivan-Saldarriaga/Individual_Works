#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 fragPos;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 cameraPos; // New uniform for camera position
uniform int rain;
uniform float rainFactor;

void main()
{
    vec4 worldPos = modelMatrix * vec4(aPos, 1.0);
    float dist = length(cameraPos - worldPos.xyz);
    if(rain == 1){
        //dist += rainFactor;
    }
    float maxSize = 10.0; // Maximum point size
    float minSize = 0.1;   // Minimum point size
    float scaleFactor = 2.0; // Adjust this value to control size scaling
    
    // Inverse relationship: closer points are larger
    gl_PointSize = clamp(maxSize / pow(dist, scaleFactor), minSize, maxSize);
    fragPos = aPos; // Pass position to fragment shader
    gl_Position = projectionMatrix * viewMatrix * worldPos;
}
