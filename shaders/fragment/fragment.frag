#version 330 core

out vec4 FragColor;
in vec3 ourColor;

// uniform vec4 ourColor; GLOBAL VARIABLE BETWEEN SHADER PROGRAMS

void main()
{
    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    
    //SOLUTION QUESTION 3:
    //VERTEX SHADER -> out vec3 vecPos;
    //FRAGMENT SHADER -> in vec3 vecPos;
    //FragColor = vec4(vecPos, 1.0); 


    FragColor = vec4(ourColor, 1.0);
}