#define _CRT_SECURE_NO_WARNINGS
#include "texture.h"
#include"glew.h"
#include<stdio.h>
#include <fstream>
#include"SDL_surface.h"



void texture::loadBMP_custom(int framep, int maxf, const char * imagepath)
{	
	frameper = framep;
	maxframe = maxf;
	//commented part is from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/
	//// Data read from the header of the BMP file
	//unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	//unsigned int dataPos;     // Position in the file where the actual data begins
	//unsigned int width, height;
	//unsigned int imageSize;   // = width*height*3
	//						  // Actual RGB data
	//unsigned char * data;

	//FILE * file = fopen(imagepath, "rb");
	//if (!file) { printf("Image could not be opened\n"); return; }


	//if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
	//	printf("Not a correct BMP file\n");
	//	return;
	//}

	//if (header[0] != 'B' || header[1] != 'M') {
	//	printf("Not a correct BMP file\n");
	//	return;
	//}

	//// Read ints from the byte array
	//dataPos = *(int*)&(header[0x0A]);
	//imageSize = *(int*)&(header[0x22]);
	//width = *(int*)&(header[0x12]);
	//height = *(int*)&(header[0x16]);

	//// Some BMP files are misformatted, guess missing information
	//if (imageSize == 0)    imageSize = width*height * 4; // 3 : one byte for each Red, Green and Blue component
	//if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	//									 // Create a buffer
	//data = new unsigned char[imageSize];

	//// Read the actual data from the file into the buffer
	//fread(data, 1, imageSize, file);

	////Everything is in memory now, the file can be closed
	//fclose(file);

	//// Create one OpenGL texture
	//glGenTextures(1, &textureID);

	//// "Bind" the newly created texture : all future texture functions will modify this texture
	//glBindTexture(GL_TEXTURE_2D, textureID);

	//glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	SDL_Surface *TextureImage;
	if ((TextureImage = SDL_LoadBMP(imagepath)))
	{
		glActiveTexture(GL_TEXTURE10);
		// Create one OpenGL texture
		glGenTextures(1, &textureID);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, textureID);


		glTexImage2D(GL_TEXTURE_2D, 0, 4, TextureImage->w,TextureImage->h, 0, GL_BGRA,GL_UNSIGNED_BYTE, TextureImage->pixels);

		// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// Generate mipmaps, by the way.
		glGenerateMipmap(GL_TEXTURE_2D);
	}


}