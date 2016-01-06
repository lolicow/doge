#ifndef TEXTURE
#define TEXTURE

#include<stdio.h>

class texture {
public:
	void loadBMP_custom(int framep,int maxf,const char * imagepath);
	unsigned int getTextureID() { return textureID; };
	int getFramePer() { return frameper; };
	int gerMaxFrame() { return maxframe; };
private:
	unsigned int textureID = 0;
	int frameper = 1;
	int maxframe = 1;
};

#endif