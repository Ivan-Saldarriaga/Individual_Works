#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
out vec3 vertexColor;
uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPos;

void main()
{
    mat4 MVPmatrix = projectionMatrix * viewMatrix * modelMatrix;
    vec4 viewPos = viewMatrix * modelMatrix * vec4(aPos, 1.0);
    gl_Position = MVPmatrix * vec4(aPos, 1.0);

    float specularStrength = 0.9;
    vec3 viewDir = normalize(-viewPos.xyz);
    vec3 lightDir = normalize(lightPos - viewPos.xyz);
    vec3 reflectDir = reflect(-lightDir, normalize(aNormal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = vec3(specularStrength) * spec;
    vertexColor = aColor + specular;
}
