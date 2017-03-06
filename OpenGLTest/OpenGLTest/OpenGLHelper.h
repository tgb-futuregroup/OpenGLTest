#pragma once

#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <glew.h>
#include <freeglut.h>

/* A simple function that will read a file into an allocated char pointer buffer */
char* filetobuf(char *file)
{
	FILE *fptr;
	long length;
	char *buf;
	errno_t error;
	error = fopen_s(&fptr, file, "rb");	/* Open file for reading */
	if (!fptr)							/* Return NULL on failure */
		return NULL;
	fseek(fptr, 0, SEEK_END);			/* Seek to the end of the file */
	length = ftell(fptr);				/* Find out how many bytes into the file we are */
	buf = (char*)malloc(length + 1);	/* Allocate a buffer for the entire length of the file and a null terminator */
	fseek(fptr, 0, SEEK_SET);			/* Go back to the beginning of the file */
	fread(buf, length, 1, fptr);		/* Read the contents of the file in to the buffer */
	fclose(fptr);						/* Close the file */
	buf[length] = 0;					/* Null terminator */

	return buf;							/* Return the buffer */
}

void checkShader(GLuint shaderID) {
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
		printf("%s\n", &ShaderErrorMessage[0]);
	}
	else { printf("Shader %d compilation: OK.\n", shaderID); }
}

GLuint loadShader(char* Filename, GLuint ShaderType)
{
	char* shaderSource = filetobuf(Filename);

	/* Assign our handles a "name" to new shader objects */
	GLuint shader = glCreateShader(ShaderType);

	/* Associate the source code buffers with each handle */
	glShaderSource(shader, 1, (const GLchar**)&shaderSource, 0);

	/* Free the temporary allocated memory */
	free(shaderSource);

	/* Compile our shader objects */
	glCompileShader(shader);

	checkShader(shader);

	return shader;
}

bool initGL(int *argc, char **argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("GL with shaders");
	glewInit();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, 640, 480);
	return true;
}

void createVBO(GLuint *vbo, float* data, size_t datasize)
{
	glGenBuffers(1, vbo);						// Create a new VBO and use the variable "triangleVBO" to store the VBO id
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);		// Make the new VBO active
	glBufferData(GL_ARRAY_BUFFER, datasize, data, GL_STATIC_DRAW);	// Upload vertex data to the video device
																	/* Specify that our coordinate data is going into attribute index 0(shaderAttribute), and contains three floats per vertex */
	glVertexAttribPointer(
		0,					// Shader attribute 0
		3,					// Number of floats per vertex
		GL_FLOAT,			// 
		GL_FALSE,			//
		0,					//
		(void *)0			// 
	);
	glEnableVertexAttribArray(0);				// Enable attribute index 0(shaderAttribute) as being used
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);		// Make the new VBO active.
}

GLuint loadBMP_custom(const char * imagepath)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;
	// Actual RGB dataM
	unsigned char * textureData;

	// Open the file
	FILE * file;
	fopen_s(&file, imagepath, "rb");
	if (!file) { printf("Image could not be opened\n"); return 0; }

	if ((fread(header, 1, 54, file) != 54)
		|| (header[0] != 'B' || header[1] != 'M')) {
		printf("Not a correct BMP file\n");
		return 0;
	}
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	if (imageSize == 0) imageSize = width * height * 3;
	if (dataPos == 0) dataPos = 54;

	textureData = new unsigned char[imageSize];
	fread(textureData, 1, imageSize, file);
	//for(int i = 0; i < imageSize; i+=3) printf("%x%x%x, ", data[i], data[i+1], data[i+2]);

	fclose(file);

	// OpenGL
	GLuint textureID;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);
	//glTexParameterf(GL_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameterf(GL_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	free(textureData);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}
