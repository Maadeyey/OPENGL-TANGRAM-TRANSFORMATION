// Code adapted from www.learnopengl.com, www.glfw.org

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/constants.hpp>

#include "shader.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void do_movement();

// Window dimensions
const GLuint WIDTH = 640, HEIGHT = 640;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];


// Light attributes
glm::vec3 lightPos(1.0f, 0.0f, 1.0f);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main(void)
{
	//++++create a glfw window+++++++++++++++++++++++++++++++++++++++
	GLFWwindow* window;

	if (!glfwInit()) //Initialize the library
		return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);//Make the window's context current

	glfwSetKeyCallback(window, key_callback);// Set the required callback functions

	glfwSetCursorPosCallback(window, mouse_callback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	//++++Initialize GLEW to setup the OpenGL Function pointers+++++++
	glewExperimental = GL_TRUE;
	glewInit();

	//++++Define the viewport dimensions++++++++++++++++++++++++++++
	glViewport(0, 0, HEIGHT, HEIGHT);

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	//++++++++++Set up vertex data (and buffer(s)) and attribute pointers+++++++++
	GLfloat vertices[] = {
		0.5f, 0.9f, 0.3f,  0.0f,  0.0f, -1.0f,
		0.4f, 0.7f, 0.3f,  0.0f,  0.0f, -1.0f,
		0.6f, 0.7f, 0.3,  0.0f,  0.0f, -1.0f,


		0.5f, 0.9f, 0.66f, 0.0f,1.0f, 0.0f,
		0.6f, 0.7f, 0.66f,  0.0f, 1.0f, 0.0f,
		0.4f, 0.7f, 0.66f,  0.0f, 1.0f, 0.0f,

		0.5f, 0.9f, 0.3f,  1.0f,0.5f, 0.0f,
		0.5f, 0.9f, 0.66f, 1.0f,0.5f, 0.0f,
		0.6f, 0.7f, 0.3f,  1.0f,0.5f, 0.0f,


		0.5f, 0.9f, 0.66f, 0.0f,0.0f, 1.0f,
		0.6f, 0.7f, 0.3f,  0.0f, 0.0f, 1.0f,
		0.6f, 0.7f, 0.66f,  0.0f, 0.0f, 1.0f,

		0.5f, 0.9f, 0.66f, 0.90f,0.91f,0.98f,
		0.4f, 0.7f, 0.66f, 0.90f,0.91f,0.98f,
		0.4f, 0.7f, 0.3f,  0.90f,0.91f,0.98f,

		0.5f, 0.9f, 0.66f, 0.439216f,0.858824f,0.576471f,
		0.5f, 0.9f, 0.3f, 0.439216f,0.858824f,0.576471f,
		0.4f, 0.7f, 0.3f,  0.439216f,0.858824f,0.576471f,

		// second triangle prism

		0.4f, 0.7f, 0.3f,  0.0f,  1.0f, 0.0f,
		0.6f, 0.7f, 0.3f, 0.0f,  1.0f, 0.0f,
		0.4f, 0.4f, 0.3f,  0.0f,  1.0f, 0.0f,

		0.4f, 0.7f, 0.66f,  1.0f,0.498039f, 0.0f,
		0.6f, 0.7f, 0.66f,  1.0f,0.498039f, 0.0f,
		0.4f, 0.4f, 0.66f,  1.0f,0.498039f, 0.0f,

		0.4f,0.7f,0.66f,   0.737255f, 0.560784f, 0.560784f,
		0.4f,0.7f,0.3f,   0.737255f, 0.560784f, 0.560784f,
		0.4f,0.4f,0.66f,   0.737255f, 0.560784f, 0.560784f,

		0.4f,0.4f,0.66f,   0.0f, 0.0f, 1.0f,
		0.4f,0.7f,0.3f,   0.0f, 0.0f, 1.0f,
		0.4f,0.4f,0.3f,   0.0f, 0.0f, 1.0f,

		//third prism
		0.6f, 0.7f, 0.3f, 1.0f,  1.0f, 0.0f,
		0.4f, 0.4f, 0.3f, 1.0f,  1.0f, 0.0f,
		0.6f, 0.24f, 0.3f, 1.0f,  1.0f, 0.0f,

		0.6f, 0.7f, 0.66f, 0.258824f,0.435294f,0.258824f,
		0.4f, 0.4f, 0.66f, 0.258824f,0.435294f,0.258824f,
		0.6f, 0.24f,0.66f, 0.258824f,0.435294f,0.258824f,

		0.6f,0.7f,0.3f,   1.0f,0.0f,0.0f,
		0.6f,0.24f,0.3f,   1.0f,0.0f,0.0f,
		0.6f,0.24f,0.66f,   1.0f,0.0f,0.0f,

		0.6f,0.7f,0.66f,   0.6f,0.8f,0.196078f,
		0.6f,0.24f,0.66f,   0.6f,0.8f,0.196078f,
		0.6f,0.7f,0.3f,     0.6f,0.8f,0.196078f,
	};
	

	
	
	GLfloat vertices2[] = {


		//4th prism
		0.6f, 0.24f,0.3f,  0.0f,  0.0f, 1.0f,
		0.4f, 0.4f,0.3f,   0.0f,  0.0f, 1.0f,
		0.4f, -0.06f,0.3f, 0.0f,  0.0f, 1.0f,

		0.6f,0.7f,0.3f,   1.0f,0.0f,0.0f,
		0.6f,0.24f,0.3f,   1.0f,0.0f,0.0f,
		0.6f,0.24f,0.66f,   1.0f,0.0f,0.0f,

		0.6f,0.7f,0.66f,   0.6f,0.8f,0.196078f,
		0.6f,0.24f,0.66f,   0.6f,0.8f,0.196078f,
		0.6f,0.7f,0.3f,     0.6f,0.8f,0.196078f,

		

		0.4f, 0.4f, 0.66f, 0.0f,1.0f,0.0f,
		0.6f, 0.24f,0.66f, 0.0f,1.0f,0.0f,
		0.4f, -0.06,0.66f, 0.0f,1.0f,0.0f,

		

		0.4f, 0.4f, 0.66f, 0.87f, 0.58f, 0.98f,
		0.4f, 0.4f, 0.3f,  0.87f, 0.58f, 0.98f,
		0.4f,-0.06f, 0.66, 0.87f, 0.58f, 0.98f,

		0.4f, -0.06f,0.3f, 0.0f, 0.0f, 1.0f,
		0.4f, 0.4f, 0.3f,  0.0f, 0.0f, 1.0f,
		0.4f,-0.06f,0.66,  0.0f, 0.0f, 1.0f,

		//parrallelogram

		0.4f, 0.16f,0.3f,   0.258824f,0.435294f,0.258824f,
		0.29f, 0.05f,0.3f,  0.258824f,0.435294f,0.258824f,
		0.4f,-0.07f,0.3f,   0.258824f,0.435294f,0.258824f,

		0.29f, 0.05f,0.3f,0.258824f,0.435294f,0.258824f,
		0.4f,-0.07f,0.3f,  0.258824f,0.435294f,0.258824f,
		0.29f,-0.2f,0.3f,  0.258824f,0.435294f,0.258824f,


		0.4f, 0.16f,0.66f,   1.0f, 0.0,1.0f,
		0.29f, 0.05f,0.66f,  1.0f, 0.0,1.0f,
		0.4f,-0.07f,0.66f,   1.0f, 0.0,1.0f,


		0.29f, 0.05f,0.66f,  1.0f, 1.0f,0.0f,
		0.4f,-0.07f,0.66f,   1.0f, 1.0f,0.0f,
		0.29f,-0.2f, 0.66f,  1.0f, 1.0f,0.0f,

		0.4f, 0.16f,0.66f,  0.5f,1.0f,0.5f,
		0.29f,0.05f,0.66f,  0.5f,1.0f,0.5f,
		0.4f,0.16f,0.3f,    0.5f,1.0f,0.5f,

		0.4f, 0.16f,0.3f,   1.0f, 0.0,0.0f,
		0.29f,0.05f,0.3f,  1.0f, 0.0,0.0f,
		0.29f,0.05f,0.66f,  1.0f, 0.0,0.0f,
	};
	GLfloat vertices3[] = {

		0.29f, 0.05f, 0.3f,   0.87f, 0.58f, 0.98f,
		0.29f, -0.2f, 0.66f,  0.87f, 0.58f, 0.98f,
		0.29f,-0.2f, 0.3f,   0.87f, 0.58f, 0.98f,

		0.29f, 0.05f, 0.3f,  0.737255f, 0.560784f, 0.560784f,
		0.29f, -0.2f, 0.66f,  0.737255f, 0.560784f, 0.560784f,
		0.29f, 0.05f, 0.66f,   0.737255f, 0.560784f, 0.560784f,

		//square1

		0.6f,0.23f,0.3f,   0.87f, 0.58f, 0.98f,
		0.5f,0.08f,0.3f,   0.87f, 0.58f, 0.98f,
		0.7f,0.1f, 0.3f,  0.87f, 0.58f, 0.98f,

		0.5f,0.08f,0.3f,   0.87f, 0.58f, 0.98f,
		0.7f,0.1f, 0.3f,   0.87f, 0.58f, 0.98f,
		0.6f,-0.08f,0.3f, 0.87f, 0.58f, 0.98f,


		0.6f,0.24f,0.3f,   0.0f,1.0f, 0.0f,
		0.6f,0.24f,0.66f,   0.0f,1.0f, 0.0f,
		0.7f,0.1f,0.3f,    0.0f,1.0f, 0.0f,

		0.7f,0.1f,0.66f,   1.0f, 0.0,1.0f,
		0.6f,0.24f,0.66f,   1.0f, 0.0,1.0f,
		0.7f,0.1f,0.3f,    1.0f, 0.0,1.0f,

		0.7f,0.1f,0.3f,    0.0f,0.0f, 1.0f,
		0.7f,-0.2f,0.3f,   0.0f,0.0f, 1.0f,
		0.7f,0.1f,0.66f,   0.0f,0.0f, 1.0f,


		0.7f,-0.2f,0.3f,   0.87f, 0.58f, 0.98f,
		0.7f,0.1f,0.66f,   0.87f, 0.58f, 0.98f,
		0.7f,-0.2,0.66f,   0.87f, 0.58f, 0.98f,

		


		//0.5f,0.08f,0.3f,   1.0f,0.0f,0.0f,
		//0.4f,-0.07f, 0.6f, 1.0f,0.0f,0.0f,
		//0.4f,-0.07f,0.3f,  1.0f,0.0f,0.0f,

		//0.5f,0.08f,0.3f,    1.0f,1.0f,1.0f,
		//0.4f,-0.07f, 0.6f,  1.0f,1.0f,1.0f,
		//0.5f,0.09f,0.66f,   1.0f,1.0f,1.0f,
		//triangle
		0.7f,0.1f,0.3f,      0.5f,1.0f,0.5f,
		0.6f,-0.06f,0.3f,    0.5f,1.0f,0.5f,
		0.7f,-0.2f,0.3f,       0.5f,1.0f,0.5f,


		0.6f,0.24f,0.66f,    0.737255f, 0.560784f, 0.560784f,
		0.7f,0.1f,0.66f,     0.737255f, 0.560784f, 0.560784f,
		0.5f, 0.08,0.66f,     0.737255f, 0.560784f, 0.560784f,


		 

		 //sqaure
		0.7f,0.1f,0.66f,       0.737255f, 0.560784f, 0.560784f,
		0.5f, 0.08,0.66f,      0.737255f, 0.560784f, 0.560784f,
		0.6f, -0.06f, 0.66f,   0.737255f, 0.560784f, 0.560784f,


		0.7f,0.1f, 0.66f,      0.5f,1.0f,0.5f,
		0.6f, -0.06f, 0.66f,   0.5f,1.0f,0.5f,
		0.7f,-0.2f,0.66f,       0.5f,1.0f,0.5f,
     };
	
	
	
	GLfloat vertices4[] = {

		0.29f,-0.2f, 0.3f,      0.6f,0.8f,0.196078f,
		0.29f, -0.2f, 0.66f,    0.6f,0.8f,0.196078f,
		0.4f,-0.07f,0.3f,       0.6f,0.8f,0.196078f,

		0.4f,-0.07f,0.66f,     0.0f, 0.0f, 1.0f,
		0.29f, -0.2f, 0.66f,   0.0f, 0.0f, 1.0f,
		0.4f,-0.07f,0.3f,      0.0f, 0.0f, 1.0f,



		0.5f,0.09f,0.66f,     0.690f, 0.878f,0.902f,
		0.7f,-0.2f,0.3f,      0.690f, 0.878f,0.902f,
		0.7f,-0.2f,0.3f,      0.690f, 0.878f,0.902f,


		0.5f, 0.08f, 0.3f,   0.0f,0.0f,0.0f,
		0.7f,-0.2f,0.66f,    0.0f,0.0f,0.0f,
		0.7f,-0.2f,0.3f,      0.0f,0.0f,0.0f,





		0.5f, 0.08f, 0.3f,    1.0f,1.0f,1.0f,
		0.7f,-0.2f,0.66f,      1.0f,1.0f,1.0f,
		0.5f,0.09f,0.66f,       1.0f,1.0f,1.0f,


		0.5f, 0.08f, 0.3f,     0.917647f,0.678431f,0.917647f,
		0.4f,-0.07f,0.66f,      0.917647f,0.678431f,0.917647f,
		0.5f,0.09f,0.66f,        0.917647f,0.678431f,0.917647f,


		0.5f, 0.08f, 0.3f,    1.0f,1.0f,1.0f,
		0.4f,-0.07f,0.66f,      1.0f,1.0f,1.0f,
		0.4f,-0.07f,0.3f,       1.0f,1.0f,1.0f,

		



		//0.5f,0.09f,0.06f,  0.690f, 0.878f,0.902f,
		//0.6f,-0.06f,0.3f,  0.690f, 0.878f,0.902f,
		//0.5f,0.08f,0.3f,   0.690f, 0.878f,0.902f,

		//0.5f,0.08f, 0.3f,  0.678f, 1.0f,0.184f,
		//0.5f,0.09f,0.06f,  0.678f, 1.0f,0.184f,
		//0.6f,-0.06f,0.3f,  0.678f, 1.0f,0.184f,








	};

	


	
	


	

	GLuint VBOs[4], VAOs[4],EBO;
	glGenVertexArrays(4, VAOs);
	glGenBuffers(4, VBOs);
	

    // ================================
    // buffer setup
    // ===============================
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);
    glBindVertexArray(0);


	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAOs[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices4), vertices4, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	

	



	//++++++++++Build and compile shader program+++++++++++++++++++++
	GLuint shaderProgram = initShader("vert.glsl","frag.glsl");

	//++++++++++++++++++++++++++++++++++++++++++++++
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))


	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		do_movement();


		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Use cooresponding shader when setting uniforms/drawing objects
		glUseProgram(shaderProgram);
		GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
		GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
		GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
		glUniform3f(objectColorLoc, 1.0f, 0.0f, 1.0f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);


		// Draw the cube
		// use shader
		glUseProgram(shaderProgram);

		// Create transformations
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		//model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
		GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
		GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
		// Pass them to the shaders


		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// draw object
		glBindVertexArray(VAOs[0]);
		
		//model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 3*12);

		//model = glm::mat4();
		//model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		//model = glm::translate(model, glm::vec3(-0.8f, -0.8f, 0.5f));
		//model = glm::rotate(model, (GLfloat) 3.14/4, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);


		glBindVertexArray(0);

		// draw vertices2
		glBindVertexArray(VAOs[1]);
		
		//model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
		//model = glm::mat4();
		//model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		//model = glm::translate(model, glm::vec3(-0.8f, -0.8f, 0.5f));
		//model = glm::rotate(model, (GLfloat) 3.14/4, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
		glBindVertexArray(0);

		// draw vertices3
		glBindVertexArray(VAOs[2]);
		
		//model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
		//model = glm::mat4();
		//model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		//model = glm::translate(model, glm::vec3(-0.8f, -0.8f, 0.5f));
		//model = glm::rotate(model, (GLfloat) 3.14/4, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
		glBindVertexArray(0);

		// draw vertices4
		glBindVertexArray(VAOs[3]);
		
		//model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
		//model = glm::mat4();
		//model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		//model = glm::translate(model, glm::vec3(-0.8f, -0.8f, 0.5f));
		//model = glm::rotate(model, (GLfloat) 3.14/4, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 3 * 12);
		glBindVertexArray(0);


		

		




		

		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	glfwTerminate();
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)xpos - lastX;
	GLfloat yoffset = lastY - (GLfloat)ypos; // Reversed since y-coordinates go from bottom to left
	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	GLfloat sensitivity = (GLfloat)0.05;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}