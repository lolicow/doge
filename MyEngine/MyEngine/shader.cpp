#include"shader.h"

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>

extern int width;
extern int height;

//modified from www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
void shader::LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open())
	{
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode +=  Line + "\n";
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += Line + "\n";
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);
	   
	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)));
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	program = ProgramID;
	LoadPerspective();
}

void shader::LoadPerspective()
{
	float perspective[4][4];

	for (int i = 0; i<4; i++)
		for (int j = 0; j<4; j++)
			perspective[i][j] = i == j ? 1.0 : 0.0;

	float ry = 0.2;
	float rx = ry * width / height;
	float back = 3000.0f;
	float front = 0.01f;
	perspective[0][0] = 1.0f / rx;
	perspective[1][1] = 1.0f / ry;
	perspective[2][2] = -(back + front) / (back - front);
	perspective[2][3] = -(2.0f*front*back) / (back - front);
	perspective[3][2] = -1.0f;
	perspective[3][3] = 0.0f;

	glUseProgram(program);

	int loc = glGetUniformLocation(program, "perspective");
	glUniformMatrix4fv(loc, 1, GL_TRUE, &perspective[0][0]);

	glUseProgram(0);
}


