/**************************************************/
//ΑΜ:4304       Όνομα:TSAPIKOUNI GEORGIA
//ΑΜ:4087       Όνομα:KOTOFOLI XRISTINA
/*************************************************/

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>


// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;


glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}


//****************************************************************

glm::vec3 positionCam= glm::vec3(10.0f, 50.0f, 0.0f);
glm::vec3 frontDirectionCam = glm::vec3(1.0f,0.0f,0.0f);
glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);

const float speedCam = 0.3f;

void camera_function()
{		
		glm::vec3 startCam = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 direction = glm::normalize(positionCam - startCam);
	
		ProjectionMatrix = glm::perspective(glm::radians(100.0f), 1.0f / 1.0f, 0.1f, 100.0f);
		ViewMatrix = glm::lookAt(positionCam, direction, up);

		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
			positionCam += glm::normalize(glm::cross(up, positionCam)) * speedCam;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			positionCam -= glm::normalize(glm::cross(up, positionCam)) * speedCam;

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			positionCam += glm::normalize(glm::cross(direction, frontDirectionCam)) * speedCam;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			positionCam -= glm::normalize(glm::cross(direction, frontDirectionCam)) * speedCam;


		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
			positionCam -= speedCam * direction;
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
			positionCam += speedCam * direction;
		

}


//************************************
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);


	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}


	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1000, 1000, "Ergasia 1B-trapezoeides prisma", NULL, NULL);


	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.3f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	
	GLuint programID = LoadShaders("ProjBVertexShader.vertexshader", "ProjBFragmentShader.fragmentshader");

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	
	//********************

	glm::mat4 Projection = glm::perspective(glm::radians(50.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(10,50,0),
		glm::vec3(0,0,0),
		glm::vec3(0,0,1)
	);
	
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model; 
	

	srand(time(NULL)); 
	int h =rand() % 9 + 2;

	static const GLfloat g_vertex_buffer_data[] = {

		1.0f,3.0f,-h,
		1.0f,3.0f, h,
		-1.0f, 3.0f, -h,
		///////////////////
		1.0f, 3.0f,h,
		-1.0f,3.0f,h,
		-1.0f, 3.0f,-h,
	    //////////////////
		-4.0f,-3.0f, h,
		4.0f,-3.0f,h,
		-4.0f,-3.0f,-h,
		//////////////////
		4.0f, -3.0f,h,
		-4.0f,-3.0f,-h,
		4.0f,-3.0f,-h,
		//////////////////
		-4.0f,-3.0f,h,
		4.0f, -3.0f, h,
		1.0f, 3.0f,h,
		//////////////////
		1.0f,3.0f, h,
		-4.0f,-3.0f, h,
		-1.0f,3.0f,h,
		//////////////////
		-4.0f, -3.0f, -h,
		4.0f,-3.0f, -h,
		1.0f,3.0f, -h,
		//////////////////
		1.0f, 3.0f, -h,
		-1.0f,3.0f,-h,
		-4.0f, -3.0f,-h,
		/////////////////
		1.0f,3.0f,h,
		1.0f, 3.0f, -h,
		4.0f,-3.0f, -h,
		//////////////////
		1.0f, 3.0f,h,
		4.0f, -3.0f,-h,
		4.0f, -3.0f,h,
		//////////////////
		-1.0f, 3.0f, h,
		-1.0f, 3.0f,-h,
		-4.0f, -3.0f, h,
		//////////////////
		-4.0f, -3.0f, h,
		-1.0f, 3.0f, -h,
		-4.0f,-3.0f, -h

	};
	// One color for each vertex. They were generated randomly.
	static const GLfloat g_color_buffer_data[] = {
		0.9f,  0.1f,  0.0f,
		0.9f,  0.1f,  0.0f,
		0.9f,  0.1f,  0.0f,

		0.9f,  0.1f,  0.0f,
		0.9f,  0.1f,  0.0f,
		0.9f,  0.1f,  0.0f,
		///////////////////////
		0.0f,  0.9f,  0.0f,
		0.0f,  0.9f,  0.0f,
		0.0f,  0.9f,  0.0f,
		
		0.0f,  0.9f,  0.0f,
		0.0f,  0.9f,  0.0f,
		0.0f,  0.9f,  0.0f,
		////////////////////////
		0.0f,  0.0f,  0.9f,
		0.0f,  0.0f,  0.9f,
		0.0f,  0.0f,  0.9f,

		0.0f,  0.0f,  0.9f,
		0.0f,  0.0f,  0.9f,
		0.0f,  0.0f,  0.9f,
		///////////////////////
		0.2f,  0.0f,  0.2f,
		0.2f,  0.0f,  0.2f,
		0.2f,  0.0f,  0.2f,

		0.2f,  0.0f,  0.2f,
		0.2f,  0.0f,  0.2f,
		0.2f,  0.0f,  0.2f,
		////////////////////////
		0.0f,  0.7f,  0.9f,
		0.0f,  0.7f,  0.9f,
		0.0f,  0.7f,  0.9f,

		0.0f,  0.7f,  0.9f,
		0.0f,  0.7f,  0.9f,
		0.0f,  0.7f,  0.9f,
		///////////////////////
		0.8f,  0.8f,  0.0f,
		0.8f,  0.8f,  0.0f,
		0.8f,  0.8f,  0.0f,

		0.8f,  0.8f,  0.0f,
		0.8f,  0.8f,  0.0f,
		0.8f,  0.8f,  0.0f,
	};



	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		
		//*************************************************

		camera_function();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {

			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 2.0f));

		}
		else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {

			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.0f, 1.0f, 0.5f));

		}
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		
			
		// **************************************************/

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		
		
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);


	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

