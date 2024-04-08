#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
out vec3 vertexColor;
out float intensity;
out float mappedDepth;
out float z_std;
out float z_prime;
uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPos;
void main()
{
    z_std = gl_Position.z;
    vec4 pos = viewMatrix * modelMatrix * vec4(aPos, 1.0);
    vec4 lightP = viewMatrix * vec4(lightPos - pos.xyz, 1.0);
    vec3 halfway = normalize(lightP.xyz + normalize(pos.xyz));
    float spec = pow(max(dot(aNormal, halfway), 0.0), 16.0);
    vec3 lightDir = normalize(lightPos - pos.xyz);
    vec3 specular = vec3(1.0) * spec;
    
    vec4 worldPos = modelMatrix * vec4(aPos, 1.0);
    vec3 fragNormal = normalize(mat3(transpose(inverse(modelMatrix))) * aNormal);
    intensity = max(dot(fragNormal, lightDir), 0.0); // Calculate vertex intensity
    vertexColor = aColor * intensity;
    vertexColor += vec3(0.2) + specular;
    gl_Position = projectionMatrix * viewMatrix * worldPos;
    z_prime = gl_Position.z;

}
