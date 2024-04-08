#version 330 core

in vec3 vertexColor;
in float z_std;
in float z_prime;
out vec4 FragColor;

float near = 0.1;
float far = 10.0;

// Function prototype
float LinearizeDepth(float depth);

void main()
{
    //compare values
    float depthDifference = abs(z_std - z_prime);
    vec3 color = vec3(depthDifference);
    FragColor = vec4(color, 1.0);
    // Calculate linearized depth
    float depth = LinearizeDepth(gl_FragCoord.z) / far;

    // Output depth value as color
    FragColor = vec4(vec3(depth), 1.0);
}

// Function definition
float LinearizeDepth(float depth){
   float z = depth * 2.0 - 1.0;
   return (2.0 * near * far) / (far + near - z * (far - near));

}
