/*********************************************************
FILE : submit.cpp (csci3260 2016-2017 Assignment 2)
*********************************************************/
/*********************************************************
Student Information
Student ID: 1155032539
Student Name: NG, Ting Yuk
*********************************************************/

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using glm::vec3;
using glm::mat4;

GLint programID;
// Could define the Vao&Vbo and interaction parameter here
GLuint vao[3];
GLuint vertexbuffer[3];
GLuint uvbuffer[3];
GLuint normalbuffer[3];
size_t vertices_size[3];

std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals; // Won't be used at the moment.

GLuint Texture[6];
GLuint TextureID[6];
float xpos;
float ypos;
float diff_brightness = 2.0f;
GLint spec_brightness = 10;
GLfloat rotate_x;
bool pause = false;
GLfloat x_tran = 0.0f;
GLfloat y_tran = 0.0f;


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
	string temp = readShaderCode("VertexShader.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
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

void keyboard(unsigned char key, int x, int y)
{
	//TODO: Use keyboard to do interactive events and animation
	if (key == 'q') {
		diff_brightness += 1.0f;
	}
	if (key == 'w') {
		diff_brightness -= 1.0f;
	}
	if (key == 'z') {
		if (spec_brightness < 105) spec_brightness += 5;
	}
	if (key == 'x') {
		if(spec_brightness > 5) spec_brightness -= 5;
	}
	if (key == 's') {
		if (pause == false) {
			pause = true;
			return;
		}
		if (pause == true) {
			pause = false;
			return;
		}
	}
}

void SpecialInput(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		x_tran += 0.2f;
		//printf("x_tran: %f \n", x_tran);
	}
	if (key == GLUT_KEY_DOWN) {
		x_tran -= 0.2f;
		//printf("x_tran: %f \n", x_tran);
	}
	if (key == GLUT_KEY_LEFT) {
		y_tran -= 0.2f;
		//printf("y_tran: %f \n", y_tran);
	}
	if (key == GLUT_KEY_RIGHT) {
		y_tran += 0.2f;
		//printf("y_tran: %f \n", y_tran);
	}
}

void PassiveMouse(int x, int y)
{
	//TODO: Use Mouse to do interactive events and animation

	xpos = (float)  (640 - x) / 640  * 18;
	ypos = (float)  (480 - y) / 480  * 18;

	//printf("xpos:%f ", xpos);
	//printf("ypos:%f \n", ypos);
}

bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

				   // else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}

	printf("Success!\n");
	return true;
}

GLuint loadBMP_custom(const char * imagepath) {

	printf("Reading image %s\n", imagepath);

	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	unsigned char * data;

	FILE * file = fopen(imagepath, "rb");
	if (!file) { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0; }

	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return 0;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    return 0; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    return 0; }

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	if (imageSize == 0)    imageSize = width*height * 3; 
	if (dataPos == 0)      dataPos = 54; 

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);

	
	GLuint textureID;
	//TODO: Create one OpenGL texture and set the texture parameter 
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	printf("Success!\n");
	return textureID;
}

void sendDataToOpenGL()
{
	//TODO:
	//Load objects and bind to VAO & VBO
	//Load texture

	// Read our .obj file
	bool res[3];
	res[0] = loadOBJ("jeep.obj", vertices, uvs, normals);

	Texture[0] = loadBMP_custom("jeep_texture.bmp");
	TextureID[0] = glGetUniformLocation(programID, "myTextureSampler");
	Texture[3] = loadBMP_custom("jeep_texture.bmp");
	TextureID[3] = glGetUniformLocation(programID, "myTextureSampler_2");

	// Load it into a VBO

	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, &vertexbuffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[0]);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	vertices_size[0] = vertices.size();
	// printf("%d \n", vertices_size[0]);

	glGenBuffers(1, &uvbuffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[0]);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glGenBuffers(1, &normalbuffer[0]);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[0]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[0]);
	glVertexAttribPointer(
		2,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	res[1] = loadOBJ("plane.obj", vertices, uvs, normals);

	Texture[1] = loadBMP_custom("plane_texture.bmp");
	TextureID[1] = glGetUniformLocation(programID, "myTextureSampler");
	Texture[4] = loadBMP_custom("grass_texture.bmp");
	TextureID[4] = glGetUniformLocation(programID, "myTextureSampler_2");

	glGenVertexArrays(1, &vao[1]);
	glBindVertexArray(vao[1]);

	glGenBuffers(1, &vertexbuffer[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[1]);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	vertices_size[1] = vertices.size();
	// printf("%d \n", vertices_size[1]);

	glGenBuffers(1, &uvbuffer[1]);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[1]);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glGenBuffers(1, &normalbuffer[1]);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[1]);
	glVertexAttribPointer(
		2,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	res[2] = loadOBJ("tank.obj", vertices, uvs, normals);

	Texture[2] = loadBMP_custom("tank_texture.bmp");
	TextureID[2] = glGetUniformLocation(programID, "myTextureSampler");
	Texture[5] = loadBMP_custom("tank_texture.bmp");
	TextureID[5] = glGetUniformLocation(programID, "myTextureSampler_2");

	glGenVertexArrays(1, &vao[2]);
	glBindVertexArray(vao[2]);

	glGenBuffers(1, &vertexbuffer[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[2]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer[2]);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	vertices_size[2] = vertices.size();
	// printf("%d \n", vertices_size[2]);


	glGenBuffers(1, &uvbuffer[2]);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[2]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer[2]);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glGenBuffers(1, &normalbuffer[2]);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[2]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer[2]);
	glVertexAttribPointer(
		2,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

}

void setupMVPmatrix(void) {

	/* First step : generating our MVP matrix. This must be done for each model you render. */

	// Projection matrix : 45¢X Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Or, for an ortho camera :
	// glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	//x += 0.001;
	//printf("%f\n", x);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(xpos, ypos, 18), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
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

void paintGL(void)
{

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);



	glClearColor(0.0f, 0.0f, 0.2f, 0.0f); // navy blue background
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//TODO:
	//Set lighting information, such as position and color of lighting source
	//Set transformation matrix
	//Bind different texture
	
	setupMVPmatrix();

	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	vec3 ambientLight(0.2f, 0.2f, 0.2f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	vec3 lightPosition(2.0f, 2.0f, 2.0f);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	GLint diff_brightnessUniformLocation = glGetUniformLocation(programID, "Diff_brightness");
	vec3 diff_brightnessPosition(diff_brightness, diff_brightness, diff_brightness);
	glUniform3fv(diff_brightnessUniformLocation, 1, &diff_brightnessPosition[0]);

	GLint eyePositionUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	vec3 eyePosition(0.0f, 0.0f, 0.0f);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);

	int spec_brightnessLocation = glGetUniformLocation(programID, "spec_brightness");
	glUniform1i(spec_brightnessLocation, spec_brightness);
	
	glm::mat4 modelTransformMatrix;
	GLint modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");;

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture[0]);
	glUniform1i(TextureID[0], 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture[3]);
	glUniform1i(TextureID[3], 1);

	glBindVertexArray(vao[0]);
	//glm::mat4 modelTransformMatrix_temp = glm::translate(glm::mat4(), glm::vec3(-x_tran, 0.0f, -y_tran));
	//glm::mat4 modelTransformMatrix_temp2 = glm::rotate(glm::mat4(), rotate_x, glm::vec3(0, 1, 0));
	//modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(x_tran, 0.0f, y_tran)) * modelTransformMatrix_temp2 * modelTransformMatrix_temp;
	modelTransformMatrix = glm::rotate(glm::mat4(), rotate_x, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	if (pause == false) rotate_x += 0.0002f;

	glDrawArrays(GL_TRIANGLES, 0, vertices_size[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture[1]);
	glUniform1i(TextureID[1], 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture[4]);
	glUniform1i(TextureID[4], 1);

	glBindVertexArray(vao[1]);



	modelTransformMatrix = glm::mat4(1.0);
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, vertices_size[0], vertices_size[1] - vertices_size[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture[2]);
	glUniform1i(TextureID[2], 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture[5]);
	glUniform1i(TextureID[5], 1);

	glBindVertexArray(vao[2]);
	modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(y_tran + 7.0f, 2.0f, x_tran + 5.0f));
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, vertices_size[1] , vertices_size[2] - vertices_size[1]);

	glFlush();
	glutPostRedisplay();

}

void initializedGL(void) 
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	installShaders();
	sendDataToOpenGL();

}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	//TODO:
	//Create a window with title specified
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Assignment 2");

	initializedGL();
	glutDisplayFunc(paintGL);
	glutPassiveMotionFunc(PassiveMouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialInput);

	glutMainLoop();

	return 0;
}