#ifndef CAMERA
#define CAMERA

#include"Component.h"
class shader;

class camera :public component {
public:
	camera() :component(T_CAMERA) {};
	void loadcamera();
	void update();
private:

};

#endif
