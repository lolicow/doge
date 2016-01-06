
#ifndef RESOURCEMANAGER
#define RESOURCEMANAGER

#include"SDL_surface.h"
#include <unordered_map>

class texture;

class ResourceManager {
public:
	ResourceManager();
	void InitResourceManager();
	SDL_Surface* LoadBMP(const char* name);
	texture* loadTexture(int framep, int maxf, const char* name);
	~ResourceManager();
private:
	std::unordered_map<std::string ,SDL_Surface*> SurfaceMap;	
	std::unordered_map<std::string,texture *> TextureMap;
};

#endif
