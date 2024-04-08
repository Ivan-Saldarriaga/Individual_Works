#version 330 core
in vec3 vertexColor; // Input varying variable for specular intensity
out vec4 FragColor;

void main()
{
    // Set the fragment color based on the specular intensity
    //vec3 specularColor = vec3(1.0); // White color for specular highlights
    FragColor = vec4(vertexColor, 1.0);
}
