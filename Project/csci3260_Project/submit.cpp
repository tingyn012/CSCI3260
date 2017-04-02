/*********************************************************
FILE : submit.cpp (csci3260 2016-2017 Assignment 2)
*********************************************************/
/*********************************************************
Student Information
Student ID:
Student Name:
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

GLuint vao[5];
GLint fulltransformMatrixUniformLocation;
GLint modelTransformMatrixUniformLocation;
GLint ViewMatrixID;
// Could define the Vao&Vbo and interaction parameter here
GLuint skybox_vbo;
GLuint vbo_vertex[4];
GLuint vbo_uv[4];
GLuint vbo_norm[4];
float angle = 0.0f;
float wx;
float wy;
float xpos;
float ypos;
bool start = false;
float diffusebr = 1.0f;
float specbr = 1.0f;
float tank_r = 0.0f;
float bullet_r = 0.0f;
float b_sin_temp = 0.0f;
float b_cos_temp = 0.0f;
float b_sin_t = 0.0f;
float b_cos_t = 0.0f;
float shoot_sin = 0.0f;
float shoot_cos = 0.0f;
float sin_temp = 0.0f;
float cos_temp = 0.0f;
float sin_t = 0.0f;
float cos_t = 0.0f;
bool shoot = false;
float bullet_r_temp = 0.0f;

std::vector<glm::vec3> vertices;
std::vector<glm::vec2> uvs;
std::vector<glm::vec3> normals;

std::vector<glm::vec3> vertices1;
std::vector<glm::vec2> uvs1;
std::vector<glm::vec3> normals1;

std::vector<glm::vec3> vertices2;
std::vector<glm::vec2> uvs2;
std::vector<glm::vec3> normals2;

std::vector<glm::vec3> vertices3;
std::vector<glm::vec2> uvs3;
std::vector<glm::vec3> normals3;

GLuint Texture[5];
GLuint TextureID[5];
GLint programID[2];

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

GLint installShaders(const char * VertexShader, const char * FragmentShader)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode(VertexShader);
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode(FragmentShader);
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return 0;
	GLint programID;
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	return programID;

	if (!checkProgramStatus(programID))
		return 0;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

}

void keyboard(unsigned char key, int x, int y)
{
	//TODO: Use keyboard to do interactive events and animation
	if (key == 's')
	{
		start = !start;
	}
	if (key == 'w')
	{
		diffusebr -= 0.1f;
	}
	if (key == 'q')
	{
		diffusebr += 0.1f;
	}
	if (key == 'x')
	{
		specbr -= 1.0f;
	}
	if (key == 'z')
	{
		specbr += 1.0f;
	}
	if (key == ' ')
	{
		b_sin_temp = sin(tank_r);
		b_cos_temp = cos(tank_r);
		b_sin_t = sin_t;
		b_cos_t = cos_t;
		bullet_r_temp = tank_r;
		shoot = true;

	}
}

void SpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		tank_r += 0.1f;
		bullet_r += 0.1f;
		break;
	case GLUT_KEY_RIGHT:
		tank_r -= 0.1f;
		bullet_r -= 0.1f;
		break;
	case GLUT_KEY_UP:
		sin_temp = sin(tank_r);
		cos_temp = cos(tank_r);
		sin_t = sin_t + sin_temp;
		cos_t = cos_t + cos_temp;
		if (shoot == false) {
			b_sin_temp = sin(bullet_r);
			b_cos_temp = cos(bullet_r);
			b_sin_t = b_sin_t + sin_temp;
			b_cos_t = b_cos_t + cos_temp;
		}
		break;
	case GLUT_KEY_DOWN:
		sin_temp = sin(tank_r);
		cos_temp = cos(tank_r);
		sin_t = sin_t - sin_temp;
		cos_t = cos_t - cos_temp;
		if (shoot == false) {
			b_sin_temp = sin(bullet_r);
			b_cos_temp = cos(bullet_r);
			b_sin_t = b_sin_t - sin_temp;
			b_cos_t = b_cos_t - cos_temp;
		}
		break;
	}
}

//void PassiveMouse(int x, int y)
//{
//	//TODO: Use Mouse to do interactive events and animation
//	wx = -((x / 512.0f) - 0.5f);
//	wy = ((y / 512.0f) - 0.5f);
//}

void PassiveMouse(int x, int y)
{
	//TODO: Use Mouse to do interactive events and animation

	xpos = (float)(640 - x) / 640 * 5 + 10;
	ypos = (float)(480 - y) / 480 * 5 + 10;

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
	printf("success\n");
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
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	
	
	return textureID;
}

GLuint loadCubemap(vector<const GLchar*> faces) {

	int width, height;
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned char* image;
	unsigned char header[54];
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++) {
		FILE * file = fopen(faces[i], "rb");
		if (!file) { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n"); getchar(); return 0; }

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


		printf("Loading Skybox face %d ...\n", i);
		dataPos = *(int*)&(header[0x0A]);
		imageSize = *(int*)&(header[0x22]);
		width = *(int*)&(header[0x12]);
		height = *(int*)&(header[0x16]);
		if (imageSize == 0)    imageSize = width*height * 3;
		if (dataPos == 0)      dataPos = 54;
		image = new unsigned char[imageSize];
		fread(image, 1, imageSize, file);
		fclose(file);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return textureID;
}


void sendDataToOpenGL()
{
	//TODO:
	//Load objects and bind to VAO & VBO
	//Load texture


	//GLfloat skyboxVertices[] = 
	//{
	//	//RIGHT
	//	1.0f, -1.0f,  1.0f,  //A
	//	1.0f,  1.0f,  1.0f,  //B
	//	1.0f,  1.0f, -1.0f,  //C
	//	1.0f,  1.0f, -1.0f,  //C
	//	1.0f, -1.0f, -1.0f,  //H
	//	1.0f, -1.0f,  1.0f,  //A
	//	//FRONT
	//	1.0f, -1.0f,  1.0f,  //A
	//	-1.0f, -1.0f,  1.0f, //D
	//	-1.0f,  1.0f,  1.0f, //G
	//	-1.0f,  1.0f,  1.0f, //G
	//	1.0f,  1.0f,  1.0f,  //B
	//	1.0f, -1.0f,  1.0f,  //A
	//	//LEFT
	//	-1.0f,  1.0f,  1.0f, //G
	//	-1.0f,  1.0f, -1.0f, //F
	//	-1.0f, -1.0f, -1.0f, //E
	//	-1.0f, -1.0f, -1.0f, //E
	//	-1.0f, -1.0f,  1.0f, //D
	//	-1.0f,  1.0f,  1.0f, //G
	//	//BACK
	//	-1.0f,  1.0f, -1.0f, //F
	//	1.0f,  1.0f, -1.0f,  //C
	//	1.0f, -1.0f, -1.0f,  //H
	//	1.0f, -1.0f, -1.0f,  //H
	//	-1.0f, -1.0f, -1.0f, //E
	//	-1.0f,  1.0f, -1.0f, //F
	//	//TOP
	//	-1.0f,  1.0f, -1.0f, //F
	//	1.0f,  1.0f, -1.0f,  //C
	//	1.0f,  1.0f,  1.0f,  //B
	//	1.0f,  1.0f,  1.0f,  //B
	//	-1.0f,  1.0f,  1.0f, //G
	//	-1.0f,  1.0f, -1.0f, //F
	//	//BOTTOM
	//	-1.0f, -1.0f, -1.0f, //E
	//	1.0f, -1.0f, -1.0f,  //H
	//	1.0f, -1.0f,  1.0f,  //A
	//	1.0f, -1.0f,  1.0f,  //A
	//	-1.0f, -1.0f,  1.0f, //D
	//	-1.0f, -1.0f, -1.0f, //E
	//};


	GLfloat skyboxVertices[] = 
	{
		-10.0f,-10.0f,-10.0f, // triangle 1 : begin
		-10.0f,-10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f, // triangle 1 : end
		10.0f, 10.0f,-10.0f, // triangle 2 : begin
		-10.0f,-10.0f,-10.0f,
		-10.0f, 10.0f,-10.0f, // triangle 2 : end
		10.0f,-10.0f, 10.0f,
		-10.0f,-10.0f,-10.0f,
		10.0f,-10.0f,-10.0f,
		10.0f, 10.0f,-10.0f,
		10.0f,-10.0f,-10.0f,
		-10.0f,-10.0f,-10.0f,
		-10.0f,-10.0f,-10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f,-10.0f,
		10.0f,-10.0f, 10.0f,
		-10.0f,-10.0f, 10.0f,
		-10.0f,-10.0f,-10.0f,
		-10.0f, 10.0f, 10.0f,
		-10.0f,-10.0f, 10.0f,
		10.0f,-10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f,-10.0f,-10.0f,
		10.0f, 10.0f,-10.0f,
		10.0f,-10.0f,-10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f,-10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		10.0f, 10.0f,-10.0f,
		-10.0f, 10.0f,-10.0f,
		10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f,-10.0f,
		-10.0f, 10.0f, 10.0f,
		10.0f, 10.0f, 10.0f,
		-10.0f, 10.0f, 10.0f,
		10.0f,-10.0f, 10.0f
	};

	// SKYBOX VAO
	glGenVertexArrays(1, &vao[4]);
	glGenBuffers(1, &skybox_vbo);
	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	//load skybox image
	vector<const GLchar*> space_faces;
	space_faces.push_back("skybox/space/right.bmp");
	space_faces.push_back("skybox/space/left.bmp");
	space_faces.push_back("skybox/space/bottom.bmp");
	space_faces.push_back("skybox/space/top.bmp");
	space_faces.push_back("skybox/space/back.bmp");
	space_faces.push_back("skybox/space/front.bmp");

	Texture[4] = loadCubemap(space_faces);
	TextureID[4] = glGetUniformLocation(programID[1], "skybox");

	printf("GLuint Texture[4]:%d \n", Texture[4]);

	glUseProgram(programID[0]);

	bool res = loadOBJ("plane.obj", vertices, uvs, normals);

	Texture[0] = loadBMP_custom("plane_texture.bmp");
	TextureID[0] = glGetUniformLocation(programID[0], "myTextureSampler");

	//vao
	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);
	//vbo vertex
	glGenBuffers(1, &vbo_vertex[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//vbo uv
	glGenBuffers(1, &vbo_uv[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[0]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0],	GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[0]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//vbo normal
	glGenBuffers(1, &vbo_norm[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_norm[0]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_norm[0]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	bool res1 = loadOBJ("jeep.obj", vertices1, uvs1, normals1);


	Texture[1] = loadBMP_custom("jeep_texture.bmp");
	TextureID[1] = glGetUniformLocation(programID[0], "myTextureSampler");
	//vao
	glGenVertexArrays(1, &vao[1]);
	glBindVertexArray(vao[1]);
	//vbo vertex
	glGenBuffers(1, &vbo_vertex[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex[1]);
	glBufferData(GL_ARRAY_BUFFER, vertices1.size() * sizeof(glm::vec3), &vertices1[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//vbo uv
	glGenBuffers(1, &vbo_uv[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[1]);
	glBufferData(GL_ARRAY_BUFFER, uvs1.size() * sizeof(glm::vec2), &uvs1[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//vbo normal
	glGenBuffers(1, &vbo_norm[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_norm[1]);
	glBufferData(GL_ARRAY_BUFFER, normals1.size() * sizeof(glm::vec3), &normals1[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_norm[1]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	bool res2 = loadOBJ("tank.obj", vertices2, uvs2, normals2);


	Texture[2] = loadBMP_custom("tank.bmp");
	TextureID[2] = glGetUniformLocation(programID[0], "myTextureSampler");
	//vao
	glGenVertexArrays(1, &vao[2]);
	glBindVertexArray(vao[2]);
	//vbo vertex
	glGenBuffers(1, &vbo_vertex[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex[2]);
	glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(glm::vec3), &vertices2[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//vbo uv
	glGenBuffers(1, &vbo_uv[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[2]);
	glBufferData(GL_ARRAY_BUFFER, uvs2.size() * sizeof(glm::vec2), &uvs2[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[2]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//vbo normal
	glGenBuffers(1, &vbo_norm[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_norm[2]);
	glBufferData(GL_ARRAY_BUFFER, normals2.size() * sizeof(glm::vec3), &normals2[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_norm[2]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	bool res3 = loadOBJ("bullet.obj", vertices3, uvs3, normals3);
	Texture[3] = loadBMP_custom("bullet.bmp");
	TextureID[3] = glGetUniformLocation(programID[0], "myTextureSampler");


	//vao
	glGenVertexArrays(1, &vao[3]);
	glBindVertexArray(vao[3]);
	//vbo vertex
	glGenBuffers(1, &vbo_vertex[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex[3]);
	glBufferData(GL_ARRAY_BUFFER, vertices3.size() * sizeof(glm::vec3), &vertices3[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//vbo uv
	glGenBuffers(1, &vbo_uv[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[3]);
	glBufferData(GL_ARRAY_BUFFER, uvs3.size() * sizeof(glm::vec2), &uvs3[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uv[3]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//vbo normal
	glGenBuffers(1, &vbo_norm[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_norm[3]);
	glBufferData(GL_ARRAY_BUFFER, normals3.size() * sizeof(glm::vec3), &normals3[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_norm[3]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

}

void paintGL(void)
{
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//draw skybox

	glDepthMask(GL_FALSE);
	glUseProgram(programID[1]);

	GLuint skb_ModelUniformLocation = glGetUniformLocation(programID[1], "M");
	glm::mat4 skb_ModelMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(skb_ModelUniformLocation, 1, GL_FALSE, &skb_ModelMatrix[0][0]);
	glm::mat4 camera = glm::lookAt(glm::vec3(xpos, ypos, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(programID[1], "view"), 1, GL_FALSE, &camera[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID[1], "projection"), 1, GL_FALSE, &projectionMatrix[0][0]);

	//Skybox Cube
	glBindVertexArray(vao[4]);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture[4]);
	glUniform1i(TextureID[4], 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	glUseProgram(programID[0]);

	//TODO:
	//Set lighting information, such as position and color of lighting source
	//Set transformation matrix
	//Bind different texture	
	glm::mat4 fulltransformMatrix;
	glm::mat4 modelTransformMatrix;
	glm::mat4 rotationMAtrix_x = glm::rotate(glm::mat4(), wx-1.6f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationMAtrix_y = glm::rotate(glm::mat4(), wy, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 model = glm::mat4(1.0f);
	fulltransformMatrix = projectionMatrix * camera * model;
	glUniformMatrix4fv(fulltransformMatrixUniformLocation, 1, GL_FALSE, &fulltransformMatrix[0][0]);

	//ambient
	GLint ambientLightUniformLocation = glGetUniformLocation(programID[0], "ambientLight");
	vec3 ambientLight(0.1f, 0.1f, 0.1f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	//Diffuse
	GLint lightPositionUniformLocation = glGetUniformLocation(programID[0], "lightPosition");
	glm::vec3 lightPos = glm::vec3(0.0f, 4.0f, 5.0f);
	glUniform3f(lightPositionUniformLocation, lightPos.x, lightPos.y, lightPos.z);
	GLint DiffuseBrightnessUniformLocation = glGetUniformLocation(programID[0], "DiffuseBrightness");
	vec3 DiffuseBrightness(diffusebr, diffusebr, diffusebr);
	glUniform3fv(DiffuseBrightnessUniformLocation, 1, &DiffuseBrightness[0]);
	//Specular
	GLint eyePositionUniformLocation = glGetUniformLocation(programID[0], "eyePosition");
	vec3 eyePosition(0.0f, 10.0f, 15.0f);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	GLint specBrightnessUniformLocation = glGetUniformLocation(programID[0], "specBrightness");
	vec3 specBrightness(specbr, specbr, specbr);
	glUniform3fv(specBrightnessUniformLocation, 1, &specBrightness[0]);


	//draw 
	glBindTexture(GL_TEXTURE_2D, Texture[0]);
	glUniform1i(TextureID[0], 0);
	glBindVertexArray(vao[0]);
	glm::mat4 scaleMAtrix_plane = glm::scale(glm::mat4(), glm::vec3(1.2f, 1.0f, 2.3f));
	modelTransformMatrix = rotationMAtrix_x * rotationMAtrix_y * scaleMAtrix_plane;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glBindVertexArray(vao[1]);
	glBindTexture(GL_TEXTURE_2D, Texture[1]);
	glUniform1i(TextureID[1], 0);
	glm::mat4 jeep_rotationMAtrix = glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	modelTransformMatrix = rotationMAtrix_x * rotationMAtrix_y * jeep_rotationMAtrix;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices1.size());
	if (start == true) {
		angle = angle - 0.0005f;
	}

	glBindVertexArray(vao[2]);
	glBindTexture(GL_TEXTURE_2D, Texture[2]);
	glUniform1i(TextureID[2], 0);
	glm::mat4 tank_mov = glm::translate(glm::mat4(), glm::vec3(+6.0f, +0.4f, 0.0f));
	glm::mat4 scaleMAtrix = glm::scale(glm::mat4(), glm::vec3(0.6f , 0.6f, 0.6f));
	glm::mat4 tank_ini = glm::translate(glm::mat4(), glm::vec3(+6.0f, +0.4f, 0.0f));
	glm::mat4 tank_trasform = glm::translate(glm::mat4(), glm::vec3(sin_t, 0.0f, cos_t));
	glm::mat4 tank_rotat = glm::rotate(glm::mat4(), tank_r, glm::vec3(0.0f, 1.0f, 0.0f));
	modelTransformMatrix = rotationMAtrix_x * rotationMAtrix_y * tank_ini * scaleMAtrix * tank_trasform * tank_rotat;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices2.size());


	glBindVertexArray(vao[3]);
	glBindTexture(GL_TEXTURE_2D, Texture[3]);
	glUniform1i(TextureID[3], 0);
	glm::mat4 bullet_rotat = glm::rotate(glm::mat4(), 1.6f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 bullet_ini = glm::translate(glm::mat4(), glm::vec3(6.0f, 1.49f, 0.0f));
	glm::mat4 bullet_scale = glm::scale(glm::mat4(), glm::vec3(0.06f, 0.06f, 0.06f));
	glm::mat4 bullet_trasform = glm::translate(glm::mat4(), glm::vec3(b_sin_t * 10.0f , 0.0f, b_cos_t * 10.0f));
	glm::mat4 bullet_ro = glm::rotate(glm::mat4(), bullet_r, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 bullet_shoot = glm::translate(glm::mat4(), glm::vec3(shoot_sin * 10.0f, 0.0f, shoot_cos * 10.0f));
	modelTransformMatrix = rotationMAtrix_x * rotationMAtrix_y  * bullet_ini * bullet_scale * bullet_trasform * bullet_ro * bullet_shoot;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, vertices3.size());
	if (shoot == true) {
		b_sin_t = b_sin_t + (b_sin_temp) * 0.03f;
		b_cos_t = b_cos_t + (b_cos_temp) * 0.03;
		bullet_r = bullet_r_temp;
	}

	glFlush();
	glutPostRedisplay();
}

void initializedGL(void) 
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	programID[0] = installShaders("VertexShader.glsl", "FragmentShader.glsl");
	programID[1] = installShaders("Skybox_VertexShader.glsl", "Skybox_FragmentShader.glsl");

	glUseProgram(programID[0]);
	glUseProgram(programID[1]);


	fulltransformMatrixUniformLocation = glGetUniformLocation(programID[0], "fulltransformMatrix");
	modelTransformMatrixUniformLocation = glGetUniformLocation(programID[0], "modelTransformMatrix");
	ViewMatrixID = glGetUniformLocation(programID[0], "V");

	sendDataToOpenGL();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	//TODO:
	//Create a window with title specified
	glutInitWindowSize(512, 512);
	glutCreateWindow("InitialTry");

	initializedGL();
	glutDisplayFunc(paintGL);
	glutPassiveMotionFunc(PassiveMouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);

	glutMainLoop();

	return 0;
}