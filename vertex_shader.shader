// modified from learnopengl.com/#Getting-started/Shaders


#version 330 core
in  vec3 position; // The position variable has attribute position 0  
uniform vec4 vertexColor; 
out vec4 vertexColorOut;
uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(position, 1.0); // See how we directly give a vec3 to vec4's constructor
    vertexColorOut = vertexColor;
}
