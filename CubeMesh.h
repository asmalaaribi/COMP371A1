// Modified from GLM.G-TRUC.net library and glossary  

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;


#include <GL/glew.h> /
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

typedef glm::vec3 Vertex;

class CubeMesh {
public:
	// data
	vector<Vertex> vertices;
	vector<GLuint> indices;
	glm::vec4 color;

	
	// Setting up the Mesh constructor
	CubeMesh(const glm::vec4 &color)
	{

		this->color = color;
		vertices.push_back(Vertex(-0.5f, -0.5f, +0.5f)); //0
		vertices.push_back(Vertex(+0.5f, -0.5f, +0.5f)); //1
		vertices.push_back(Vertex(+0.5f, +0.5f, +0.5f)); //2
		vertices.push_back(Vertex(-0.5f, +0.5f, +0.5f)); //3

		vertices.push_back(Vertex(-0.5f, -0.5f, -0.5f)); //4
		vertices.push_back(Vertex(+0.5f, -0.5f, -0.5f)); //5
		vertices.push_back(Vertex(+0.5f, +0.5f, -0.5f)); //6
		vertices.push_back(Vertex(-0.5f, +0.5f, -0.5f)); //7

		// front of the cube
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);

		// back of the cube
		indices.push_back(5);
		indices.push_back(4);
		indices.push_back(7);

		indices.push_back(7);
		indices.push_back(6);
		indices.push_back(5);

		// left of the cube
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(7);

		indices.push_back(7);
		indices.push_back(4);
		indices.push_back(0);

		// right of the cube
		indices.push_back(1);
		indices.push_back(5);
		indices.push_back(6);

		indices.push_back(6);
		indices.push_back(2);
		indices.push_back(1);

		// top of the cube
		indices.push_back(3);
		indices.push_back(2);
		indices.push_back(6);

		indices.push_back(6);
		indices.push_back(7);
		indices.push_back(3);

		// bottom of the cube
		indices.push_back(0);
		indices.push_back(4);
		indices.push_back(5);

		indices.push_back(5);
		indices.push_back(1);
		indices.push_back(0);

		this->setupMesh();
	}

	// the function that draws the mesh
	void Draw(Shader &shader)
	{

		GLint loc = glGetUniformLocation(shader.Program, "vertexColor");
		glUniform4fv(loc, 1, &color[0]);

		// Draws the mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}

private:
	// declare vertex array object and the two buffers 
	GLuint VAO, VBO, EBO;

	
	// Initialization of the VAO, VBO, and EBO
	void setupMesh()
	{
		// Create the VAO, VBO, and EBO
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		// Bind the vertex array object:
		glBindVertexArray(this->VAO);
		// then we bind the vertex buffer array:
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		// we now load the data into the VBO and allocate memory to it and that's why we need to know the type, size, pointer to data, and usage
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);


		// we bind the element/index buffer array:
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		// we now load the data into the VBO and allocate memory to it and that's why we need to know the type, size, pointer to data, and usage
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Set the vertex attribute pointer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		
		// Unbind the VAO
		glBindVertexArray(0);
	}
};
