#ifndef SHADER
#define SHADER

#include"glew.h"
#include"glut.h"



class shader
{
public:
	GLuint getProgram() { return program; };
	void LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
	void LoadPerspective();
private:
	GLuint program;
	
};

#endif
