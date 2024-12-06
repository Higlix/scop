#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

void main()
{
    // QUESTION 1 SOLUTION: vec4(-aPos.x, -aPos.y, aPos.z, 1.0);

    // QUESTION 2 SOLUTION: 
    // uniform float offset;
    // vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);
    // IN OpenGL Code -> shader.setFloat("offset", 0.1f);
    
    ourColor = aColor;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}