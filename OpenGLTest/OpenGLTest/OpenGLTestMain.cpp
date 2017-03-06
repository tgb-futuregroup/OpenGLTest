#include <GL/glew.h>
#include "GL/wglew.h"
#include <GL/freeglut.h>

#include "OpenGLHelper.h"

// vbo variables
GLuint vbo;

// Uniform variables
GLint loc;

// Texture ID
GLuint Texture;

// Texture sampler ID
GLuint textureSamplerID;

// GL functionality
bool initGL(int *argc, char **argv);
void createVBO(GLuint *vbo);

float time = 0.0;

// rendering callbacks
void display();
void timerEvent(int value);

float data[6][3] =
{
	{ -1.0,  1.0,  0.0 },
	{ -1.0, -1.0,  0.0 },
	{  1.0, -1.0,  0.0 },
	{  1.0, -1.0,  0.0 },
	{ -1.0,  1.0,  0.0 },
	{  1.0,  1.0,  0.0 },
};

int main(int argc, char **argv)
{
	if (false == initGL(&argc, argv))return false;

	glutDisplayFunc(display);
	glutTimerFunc(10, timerEvent, 0);

	createVBO(&vbo, (float*)data, sizeof(data));

	GLuint vertexShader = loadShader("VertexShader.vert", GL_VERTEX_SHADER);
	GLuint fragmentShader = loadShader("FragmentShader.frag", GL_FRAGMENT_SHADER);

	GLuint shaderProgram = glCreateProgram();		// Assign our program handle a "name"

	glAttachShader(shaderProgram, vertexShader);	// Attach our shaders to our program
	glAttachShader(shaderProgram, fragmentShader);

	const unsigned int shaderAttribute = 0;			// Bind attribute index 0 (shaderAttribute) to in_Position
	glBindAttribLocation(shaderProgram, shaderAttribute, "in_Position");	// "in_Position" will represent "data" 
																			// array's contents in the vertex shader
	glLinkProgram(shaderProgram);					// Link shader program
	glUseProgram(shaderProgram);					// Set shader program as being actively used

	loc = glGetUniformLocation(shaderProgram, "time");
	if (loc != -1) { glUniform1f(loc, time); }
	else { printf("Error Binding time uniform"); }

	Texture  = loadBMP_custom("stones.bmp");

	textureSamplerID = glGetUniformLocation(shaderProgram, "myTextureSampler");

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureSamplerID, 0);

	glutMainLoop();							 		// start rendering mainloop

	// Dummy edit
}

void timerEvent(int value)
{
	if (glutGetWindow())
	{
		time += 0.01f;
		glutPostRedisplay();
		glutTimerFunc(10, timerEvent, 0);
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform1f(loc, time);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(textureSamplerID, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDrawArrays(GL_TRIANGLES, 0, (sizeof(data) / 3) / sizeof(GLfloat));
	glutSwapBuffers();
}