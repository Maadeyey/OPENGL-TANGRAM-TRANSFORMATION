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

#include "shader.h"

int main(void)
{
	//++++create a glfw window+++++++++++++++++++++++++++++++++++++++
	GLFWwindow* window;

	if (!glfwInit()) //Initialize the library
		return -1;

	window = glfwCreateWindow(640, 640, "OpenGL Window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);//Make the window's context current
	
	//++++Initialize GLEW to setup the OpenGL Function pointers+++++++
	glewExperimental = GL_TRUE;
	glewInit();

	//++++Define the viewport dimensions++++++++++++++++++++++++++++
	glViewport(0, 0, 640, 640);

	//++++++++++Set up vertex data (and buffer(s)) and attribute pointers+++++++++
    GLfloat firstTriangle[] = {
		 //position data,      colour data
		 -0.36f, 0.36f, 0.4f, 1.0f, 0.0f, 0.0f,// Left 
         -0.36f, 0.04f, 0.4f, 1.0f, 0.0f, 0.0f,// Right
         -0.2f,  0.2f,  0.4f, 1.0f, 0.0f, 0.0f, // Top 
    };
    GLfloat secondTriangle[] = {
         -0.04f, 0.36f, 0.4f,  0.0f, 0.0f, 1.0f,// Left
         -0.36f, 0.36f, 0.4f,  0.0f, 0.0f, 1.0f,// Right
         -0.2f, 0.2f, 0.4f,    0.0f, 0.0f, 1.0f,// Top 
    };

	GLfloat thirdTriangle[] = {
		 -0.2f, 0.2f, 0.4f,   0.0f, 1.0f, 0.0f,
		 -0.12f, 0.12f, 0.4f, 0.0f, 1.0f, 0.0f,
		 -0.28f, 0.12f, 0.4f, 0.0f, 1.0f, 0.0f,

		  
	};

	GLfloat square[] = {
		-0.12f, 0.28f, 0.4f,  0.5f, 0.0f, 1.0f,
		-0.2f,  0.2f, 0.4f,  0.5f, 0.0f, 1.0f,
		-0.12f, 0.12, 0.4f,  0.5f, 0.0f, 1.0f,
		-0.04f, 0.2f,0.4f,  0.5f, 0.0f, 1.0f,
		
        
	};
	GLuint indices[] = {

		1, 2, 3,
		//3,1,0,
		//2,1,0,
		 3,1,0


	};

	

	GLfloat fourthtriangle[] = {
		-0.04f, 0.36f, 0.4f,  1.0f, 1.0f, 0.0f,
		-0.12f, 0.28f, 0.4f,  1.0f, 1.0f, 0.0f,
		-0.04f, 0.2f,  0.4f,  1.0f, 1.0f, 0.0f,
	};

	GLfloat fifthtriangle[] = {
		-0.04f, 0.2f,  0.4f,     1.0f, 0.5f, 0.0f,
		-0.2f,  0.04f, 0.4f,     1.0f, 0.5f, 0.0f,
		-0.04f, 0.04f, 0.4f,     1.0f, 0.5f, 0.0f,

	};

	GLfloat parallelogramFirstHalf[] = {
		-0.12f, 0.12f, 0.4f,     0.0f, 1.0f, 1.0f,
		-0.28f,  0.12f, 0.4f,     0.0f, 1.0f, 1.0f,
		-0.36,  0.04f, 0.4f,     0.0f, 1.0f, 1.0f,
		-0.2f, 0.04f, 0.4f,      0.0f, 1.0f, 1.0f,
		
		
	};
	




	GLuint VBOs[8], VAOs[8], EBO;
	glGenVertexArrays(8, VAOs); // the address-of operator (&) is not needed here, as the array name is a pointer type
	glGenBuffers(8, VBOs);
	glGenBuffers(1, &EBO);

    // ================================
    // First Triangle setup
    // ===============================
    glBindVertexArray(VAOs[0]); 
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same, note that the stride is 6*sizeof(GLloat)			
	glEnableVertexAttribArray(0);  // set attribute index of the position attribute to 0 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);
   
    glBindVertexArray(0);
    // ================================
    // Second Triangle setup
    // ===============================
    glBindVertexArray(VAOs[1]);	// Note that we bind to a different VAO now
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// And a different VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);
	
    glBindVertexArray(0);

	// ================================
	// Third Triangle setup
	// ===============================
    glBindVertexArray(VAOs[2]);	// Note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);	// And a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(thirdTriangle), thirdTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);


	// ================================
	// Square setup
	// ===============================
	glBindVertexArray(VAOs[3]);	// Note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);	// And a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));// Color attribute
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	
	

	// ================================
	// FourthTiangle setup
	// ===============================
	glBindVertexArray(VAOs[4]);	// Note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]);	// And a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(fourthtriangle), fourthtriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	//fifthTriangle
	// ===============================
	glBindVertexArray(VAOs[5]);	// Note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[5]);	// And a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(fifthtriangle), fifthtriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Color attribute
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);



	//parallelogramfirsthalf
	// ===============================
	
	glBindVertexArray(VAOs[6]);	// Note that we bind to a different VAO now
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[6]);	// And a different VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(parallelogramFirstHalf), parallelogramFirstHalf, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);	// Vertex attributes stay the same	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));// Color attribute
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	
	






	//++++++++++Build and compile shader program+++++++++++++++++++++
	GLuint shaderProgram = initShader("vert.glsl","frag.glsl");

	//++++++++++++++++++++++++++++++++++++++++++++++
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		GLint transformLoc;

		// Draw the first Triangle
		glm::mat4 transform;
		transform = glm::translate(transform, glm::vec3(0.6f, 0.2f,-0.6f));
		transform = glm::rotate(transform, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.4f));


		//animation
		glm::mat4 iden;
		if ((GLfloat)glfwGetTime() * 0.1f <= 1.0)
			transform = glm::mix(iden, transform, (GLfloat)glfwGetTime() * 0.1f);
		
		// Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Draw the second Triangle
        transform = glm::mat4();
		transform = glm::translate(transform, glm::vec3(0.04f, 0.36f, -0.4f));
		transform = glm::rotate(transform, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 0.2f));


		

		//animation
		if ((GLfloat)glfwGetTime() * 0.1f <= 1.0)
			transform = glm::mix(iden, transform, (GLfloat)glfwGetTime() * 0.1f);

		// Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Draw the third triangle
		transform = glm::mat4();
		transform = glm::translate(transform, glm::vec3(0.60f, 0.435f, -0.4f));
		transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 0.2f));

		//animation
		if ((GLfloat)glfwGetTime() * 0.1f <= 1.0)
			transform = glm::mix(iden, transform, (GLfloat)glfwGetTime() * 0.1f);
		// Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Draw the square
		transform = glm::mat4();
		transform = glm::translate(transform, glm::vec3(0.52f, 0.12f, -0.4f));
		transform = glm::rotate(transform, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.2f));

		//animation
		if ((GLfloat)glfwGetTime() * 0.1f <= 1.0)
			transform = glm::mix(iden, transform, (GLfloat)glfwGetTime() * 0.1f);
		// Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[3]);
		glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		

		

		

      	

		//draw fourthtriangle
	    transform = glm::mat4();
		transform = glm::translate(transform, glm::vec3(0.04f, 0.68f, -0.2f));
		transform = glm::rotate(transform, glm::radians(270.0f), glm::vec3(0.0f, 0.0f, 0.2f));

		
		//animation
		if ((GLfloat)glfwGetTime() * 0.1f <= 1.0)
			transform = glm::mix(iden, transform, (GLfloat)glfwGetTime() * 0.1f);
		

		// Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[4]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);


		//fifthtriangle

		transform = glm::mat4();
		transform = glm::translate(transform, glm::vec3(0.20f, 0.76f, -0.2f));
		transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 0.2f));

		//animation
		
		if((GLfloat)glfwGetTime()* 0.1f <= 1.0)
			transform = glm::mix(iden, transform, (GLfloat)glfwGetTime() * 0.1f);
			

		// Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[5]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//parallelogramfirsthalf

		transform = glm::mat4();

		transform = glm::translate(transform, glm::vec3(0.28f, 0.04f, -0.2f));
		transform = glm::rotate(transform, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 0.2f));


		//animation

		if ((GLfloat)glfwGetTime() * 0.1f <= 1.0)
			transform = glm::mix(iden, transform, (GLfloat)glfwGetTime() * 0.1f);


		// Get matrix's uniform location and set matrix
		transformLoc = glGetUniformLocation(shaderProgram, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAOs[6]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		
		




		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	// Properly de-allocate all resources once they've outlived their purpose
	
};