#include "ResourceManager.h"
#include "texture.h"

ResourceManager::ResourceManager()
{

}

void ResourceManager::InitResourceManager()
{
}

SDL_Surface * ResourceManager::LoadBMP(const char * name)
{
	if ((SurfaceMap)[name])
		return (SurfaceMap)[name];
	return (SurfaceMap)[name] = SDL_LoadBMP(name);
}

texture* ResourceManager::loadTexture(int framep, int maxf, const char * name)
{
	if ((TextureMap)[name])
		return (TextureMap)[name];
	else
	{ 
		(TextureMap)[name] = new texture();
		(TextureMap)[name]->loadBMP_custom(framep, maxf, name);
	}
	return (TextureMap)[name];

}


ResourceManager::~ResourceManager()
{
	for (auto it = (SurfaceMap).begin(); it != (SurfaceMap).end(); ++it)
		SDL_FreeSurface(it->second);
	for (auto it = (TextureMap).begin(); it != (TextureMap).end(); ++it)
	{
		if (it->second != NULL)
			delete it->second;
	}
		
}