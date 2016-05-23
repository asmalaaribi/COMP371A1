// modified from learnopengl.com/#Getting-started/Shaders


#version 330 core

in vec4 vertexColorOut; 
out vec4 fragmentColorOut;

void main()
{
    fragmentColorOut = vertexColorOut;
} 