#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 vertexColor;
out float intensity;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPos;

void main() {
    mat4 MVPmatrix = projectionMatrix * viewMatrix * modelMatrix;
    mat4 MVmatrix = viewMatrix * modelMatrix;

    vec3 modelViewVert = vec3(MVmatrix * vec4(aPos, 1.0));
    vec3 modelViewNorm = normalize(mat3(MVmatrix) * aNormal);

    vec3 lightVec = normalize(lightPos - modelViewVert);

    float diffuse = max(dot(modelViewNorm, lightVec), 0.1);

    vertexColor = aColor * diffuse;
    intensity = diffuse; // Passing diffuse intensity for potential use in fragment shader

    gl_Position = MVPmatrix * vec4(aPos, 1.0);
}
