// modified from learnopengl.com

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include "CubeMesh.h"
#include "camera.h"

// we will use this to set the scaling values 
#define STEP_SCALE 0.05f
#define STEP_ANGLE 0.001f
#define STEP_ANGLE_PER_KEY  0.1f

// needed rendering modes 
enum
{
	RENDER_TRIANGLE,
	RENDER_LINE,
	RENDER_POINT
};

// create the shader object and set it to null
Shader *shader = NULL;

// create the two cubes 
CubeMesh *SPINNER = NULL;
CubeMesh *REVOLVER = NULL;

GLint WIDTH = 800; 
GLint HEIGHT= 800;

float scaleFactorSPINNER = 1.0f;
float scaleFactorREVOLVER = 0.5f;
bool shiftPressed = false;

// set the rotation around the x-axis and z-axis to zero, which will be modifiable through the controls function
float rx = 0.0f;
float rz = 0.0f;

int renderingMode = RENDER_TRIANGLE;	

// create the camera object 
Camera camera;
int mouseLeftPressed = false;


// set control handles to scale up/down, change render mode, and rotate the cubes
void controls(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, GL_TRUE);

		else if (key == GLFW_KEY_R && shiftPressed)	// R
			scaleFactorREVOLVER += STEP_SCALE;

		else if (key == GLFW_KEY_R)	// r
			scaleFactorREVOLVER -= STEP_SCALE;

		else if (key == GLFW_KEY_S && shiftPressed)	// S
			scaleFactorSPINNER += STEP_SCALE;

		else if (key == GLFW_KEY_S)	// s
			scaleFactorSPINNER -= STEP_SCALE;

		else if (key == GLFW_KEY_LEFT_SHIFT)
			shiftPressed = true;

		else if (key == GLFW_KEY_LEFT)
			rz += STEP_ANGLE_PER_KEY;

		else if (key == GLFW_KEY_RIGHT)
			rz -= STEP_ANGLE_PER_KEY;

		else if (key == GLFW_KEY_UP)
			rx += STEP_ANGLE_PER_KEY;

		else if (key == GLFW_KEY_DOWN)
			rx -= STEP_ANGLE_PER_KEY;

		else if (key == GLFW_KEY_P)
			renderingMode = RENDER_POINT;

		else if (key == GLFW_KEY_W)
			renderingMode = RENDER_LINE;

		else if (key == GLFW_KEY_T)
			renderingMode = RENDER_TRIANGLE;

	}

	else
	{
		if (key == GLFW_KEY_LEFT_SHIFT)
			shiftPressed = false;
	}
}

// Allows us to know if the left mouse button is presses
void mouseButton(GLFWwindow *window, int button, int action, int mod)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			mouseLeftPressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			mouseLeftPressed = false;
		}
	}
}

// allows the user to move camera if the left button is presses
void mouseMove(GLFWwindow *window, double x, double y)
{
	static double lastY = y;
	if (mouseLeftPressed)
	{
		double delta = y - lastY;
		camera.ProcessMouseMovement(0, delta);
	}
	lastY = y;
}


// creates the window
GLFWwindow* initWindow(const int resX, const int resY)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
									 // Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(resX, resY, "TEST", NULL, NULL);

	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, controls);

	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)(mouseButton));
	glfwSetCursorPosCallback(window, mouseMove);

	glewExperimental = GL_TRUE;
	glewInit();


	glEnable(GL_DEPTH_TEST); // enable the Depth Testing
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	shader = new Shader("vertex_shader.shader", "fragment_shader.shader");
	SPINNER = new CubeMesh(glm::vec4(0, 1, 0, 1));
	REVOLVER = new CubeMesh(glm::vec4(1, 0, 0, 1));
	return window;
}



void drawCubes()
{
	shader->Use();

	// this sets the rendering modes based on points lines and triangles
	switch (renderingMode)
	{
		case RENDER_TRIANGLE: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); glPointSize(1); break;
		case RENDER_LINE: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  glPointSize(1); break;
		default: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);  glPointSize(4); break;
	}

	glm::mat4x4 projection = glm::perspective(45.0, (double)WIDTH / (double)HEIGHT, 1.0, 100.0);
	static float ry = 0.0;
	ry += STEP_ANGLE;

	glm::mat4x4 view = camera.GetViewMatrix();

	glm::mat4x4 rotX = glm::rotate(glm::mat4(), rx, glm::vec3(1, 0, 0));
	glm::mat4x4 rotY = glm::rotate(glm::mat4(), ry, glm::vec3(0, 1, 0));
	glm::mat4x4 rotminusY = glm::rotate(glm::mat4(), -ry, glm::vec3(0, 1, 0));
	glm::mat4x4 rotZ = glm::rotate(glm::mat4(), rz, glm::vec3(0, 0, 1));

	//drawing the first cube 
	glm::mat4x4 translate = glm::translate(glm::mat4x4(), glm::vec3(0, 0, -4));
	glm::mat4x4 scale = glm::scale(glm::mat4(), glm::vec3(scaleFactorSPINNER, scaleFactorSPINNER, scaleFactorSPINNER));
	glm::mat4x4 mvp = projection * view * translate * rotZ * rotX * rotY * scale;
	GLint loc = glGetUniformLocation(shader->Program, "mvp");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]);
	SPINNER->Draw(*shader);

	//drawing the second cube around SPINNER and using the same rotation angle
	glm::mat4x4 tright = glm::translate(glm::mat4x4(), glm::vec3(2, 0, 0));
	scale = glm::scale(glm::mat4(), glm::vec3(scaleFactorREVOLVER, scaleFactorREVOLVER, scaleFactorREVOLVER));
	mvp = projection * view * translate * rotZ * rotX * rotY  * tright * rotminusY * scale;
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mvp[0][0]);
	REVOLVER->Draw(*shader);
}

void display(GLFWwindow* window)
{

	// game loop 
	while (!glfwWindowShouldClose(window))
	{

		glfwGetWindowSize(window, &WIDTH, &HEIGHT);
		glViewport(0, 0, WIDTH, HEIGHT);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawCubes();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

int main(int argc, char** argv)
{
	GLFWwindow* window = initWindow(WIDTH, HEIGHT);
	if (NULL != window)
		display(window);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
