/*********************************************************
FILE : submit.cpp (csci3260 2016-2017 Assignment 1)
*********************************************************/
/*********************************************************
Student Information
Student ID: 1155032539
Student Name: NG Ting Yuk
*********************************************************/


#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
using namespace std;

GLint programID;

//GLuint VertexArrayID;
//GLuint VertexArrayID2;
//
//
// This will identify our vertex buffer
//GLuint vertexbuffer;
//GLuint vertexbuffer2;
//
//GLuint colorbuffer;
//GLuint colorbuffer2;

GLuint vao[3];
GLuint vbo[6];

GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat scale = 1.0f;
GLfloat rotate_x = 0.0f;


glm::mat4 MVP;

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	string temp = readShaderCode("VertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void keyboard(unsigned char key, int xx, int yy)
{
	//TODO:
	if(key == 'a') { // If they ¡¥a¡¦ key was pressed  
		//printf("x:%f\n", x);
		x += 0.05f / scale;
		if (x > (2.0f - 0.1*scale)) x = (GLfloat) (2.0f - 0.1*scale);
	}
	if (key == 'd') { // If they ¡¥d¡¦ key was pressed  
		//printf("x:%f\n", x);
		x -= 0.05f / scale;
		if (x < (-2.0f + 0.1*scale)) x = (GLfloat) (-2.0f + 0.1*scale);
	}
	if (key == 'w') { // If they ¡¥w¡¦ key was pressed  
		//printf("y:%f\n", y);
		y -= 0.05f / scale;
		if (y < (-2.0f + 0.1*scale)) y = (GLfloat) (-2.0f + 0.1*scale);
	}
	if (key == 's') { // If they ¡¥s¡¦ key was pressed  
		//printf("y:%f\n", y);
		y += 0.05f / scale;
		if (y > (2.0f - 0.1*scale)) y = (GLfloat) (2.0f - 0.1*scale);
	}
	if (key == 'f') { // If they ¡¥f¡¦ key was pressed  
		//printf("scale:%f\n", scale);
		if (scale < 3) scale += 0.08f;
	}
	if (key == 'r') { // If they ¡¥r¡¦ key was pressed  
		//printf("scale:%f\n", scale);
		if (scale > 0.5) scale -= 0.08f;
	}

	if (key == 'g') { // If they ¡¥g¡¦ key was pressed  
		//printf("rotate_x:%f\n", rotate_x);
		rotate_x -= 0.08f;
	}
}

void sendDataToOpenGL()
{
	//TODO:
	//create solid objects here and bind to VAO & VBO

	//glGenVertexArrays(1, &VertexArrayID);
	//glGenVertexArrays(1, &VertexArrayID2);

	//glBindVertexArray(VertexArrayID);

	const GLfloat cube[] =
	{
		-0.1f,-0.1f,-0.1f, // cube 1 start
		-0.1f,-0.1f, 0.1f,
		-0.1f, 0.1f, 0.1f,
		0.1f, 0.1f,-0.1f,
		-0.1f,-0.1f,-0.1f,
		-0.1f, 0.1f,-0.1f,
		0.1f,-0.1f, 0.1f,
		-0.1f,-0.1f,-0.1f,
		0.1f,-0.1f,-0.1f,
		0.1f, 0.1f,-0.1f,
		0.1f,-0.1f,-0.1f,
		-0.1f,-0.1f,-0.1f,
		-0.1f,-0.1f,-0.1f,
		-0.1f, 0.1f, 0.1f,
		-0.1f, 0.1f,-0.1f,
		0.1f,-0.1f, 0.1f,
		-0.1f,-0.1f, 0.1f,
		-0.1f,-0.1f,-0.1f,
		-0.1f, 0.1f, 0.1f,
		-0.1f,-0.1f, 0.1f,
		0.1f,-0.1f, 0.1f,
		0.1f, 0.1f, 0.1f,
		0.1f,-0.1f,-0.1f,
		0.1f, 0.1f,-0.1f,
		0.1f,-0.1f,-0.1f,
		0.1f, 0.1f, 0.1f,
		0.1f,-0.1f, 0.1f,
		0.1f, 0.1f, 0.1f,
		0.1f, 0.1f,-0.1f,
		-0.1f, 0.1f,-0.1f,
		0.1f, 0.1f, 0.1f,
		-0.1f, 0.1f,-0.1f,
		-0.1f, 0.1f, 0.1f,
		0.1f, 0.1f, 0.1f,
		-0.1f, 0.1f, 0.1f,
		0.1f,-0.1f, 0.1f, // cube 1 end

	};

	const GLfloat plane[] =
	{
			-2.0f, -2.0f, 0.0f, //plane start
			2.0f, -2.0f, 0.0f,
			-2.0f, 2.0f, 0.0f,

			2.0f, 2.0f, 0.0f,
			2.0f, -2.0f, 0.0f,
			-2.0f, 2.0f, 0.0f, //plane end
	};

	const GLfloat pyramid[] =
	{
		-1.5f, -1.5f, 0.0f,
		-0.5f, -1.5f, 0.0f,
		-1.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-1.0f, -1.0f, 1.0f,
	};

	GLushort indices[] =
	{
		0, 1, 4,
		0, 2, 4,
		0, 1, 2,
		3, 1, 2,
		3, 1, 4,
		3, 2, 4,

	};

	GLuint indexBufferID;

	const GLfloat cube_color[] =
	{
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f,
	};
	const GLfloat plane_color[] = 
	{
		0.8f,  0.8f,  0.8f,
		0.8f,  0.8f,  0.8f,
		0.8f,  0.8f,  0.8f,
		0.8f,  0.8f,  0.8f,
		0.8f,  0.8f,  0.8f,
		0.8f,  0.8f,  0.8f,
	};

	const GLfloat pyramid_color[] =
	{
		0.869f , 0.115f , 0.970f,
		0.812f , 0.298f , 0.568f,
		0.703f , 0.770f , 0.142f,
		0.513f , 0.849f , 0.947f,
		0.213f , 0.889f , 0.421f,
	};

	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glGenVertexArrays(1, &vao[1]);
	glBindVertexArray(vao[1]);

	glGenBuffers(1, &vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glGenBuffers(1, &vbo[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane_color), plane_color, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glGenVertexArrays(1, &vao[2]);
	glBindVertexArray(vao[2]);

	glGenBuffers(1, &vbo[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid) , pyramid, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	glGenBuffers(1, &vbo[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_color), pyramid_color, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(indices), indices, GL_STATIC_DRAW);

}

void paintGL(void)
{
	//TODO:
	//render your objects and control the transformation here

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	

	glClearColor(0.0f, 0.0f, 0.2f, 0.0f); // navy blue background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	
	glm::mat4 modelTransformMatrix;
	GLint modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");;

	glBindVertexArray(vao[0]);
	glm::mat4 modelTransformMatrix1 = glm::translate(glm::mat4(), glm::vec3(x, y, scale/10));
	glm::mat4 modelTransformMatrix2 = glm::scale(glm::mat4(), glm::vec3(scale, scale, scale));
	modelTransformMatrix = modelTransformMatrix1*modelTransformMatrix2;

	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // Object: Small Cube

	glBindVertexArray(vao[1]);
	modelTransformMatrix = glm::mat4(1.0);
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 6); // Object: Plane

	glBindVertexArray(vao[2]);

	modelTransformMatrix1 = glm::translate(glm::mat4(), glm::vec3(1.0f, 1.0f, 0.0f));
	modelTransformMatrix2 = glm::rotate(glm::mat4(), rotate_x, glm::vec3(0, 0, 1));
	modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(-1.0f, -1.0f, 0.0f))*modelTransformMatrix2*modelTransformMatrix1;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, 0);  // Object: Pyramid

	if (x > (-1.7 - 0.1*scale) && x < (-0.3 + 0.1*scale)) {
		if (y >(-1.7 - 0.1*scale) && y < (-0.3 + 0.1*scale)) {
			rotate_x += 0.002f;
		}
	}

	glFlush();
	glutPostRedisplay();
}

void setupMVPmatrix(void) {

	/* First step : generating our MVP matrix. This must be done for each model you render. */

	// Projection matrix : 45¢X Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Or, for an ortho camera :
	// glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(2, 5, 5), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, -1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	/* Second step : give it to GLSL */

	// Get a handle for our "MVP" uniform
	// Only during the initialisation
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

}


void initializedGL(void) //run only once
{
	glewInit();

	sendDataToOpenGL();
	installShaders();
	setupMVPmatrix();

}

int main(int argc, char *argv[])
{
	/*Initialization of GLUT library*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	/*Create a window with title specified*/
	//TODO:
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Assignment 1");

	initializedGL();

	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	glutDisplayFunc(paintGL);
	glutKeyboardFunc(keyboard);

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();

	return 0;
}