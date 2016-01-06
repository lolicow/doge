//#include "material.h"
//#include "shader.h"
//#include "texture.h"
//#include"glew.h"
//#include"glut.h"
//
//int material::loadmaterial()
//{
//	glUseProgram(s->getProgram());
//
//	int program = s->getProgram();
//
//	glActiveTexture(GL_TEXTURE0 + 1);
//	glBindTexture(GL_TEXTURE_2D, t->getTextureID());
//	int loc = glGetUniformLocation(program, "Texture");
//	glUniform1i(loc, 1);
//
//
//
//	return program;
//}
//
//void material::unloadmaterial()
//{
//	glActiveTexture(GL_TEXTURE0 + 1);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	glUseProgram(0);
//}
