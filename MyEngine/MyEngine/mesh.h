#ifndef MESH
#define MESH

class mesh {
public:
	~mesh() {};
	void initsquare();
	unsigned int getVao() { return vao; };
private:
	unsigned int vao;
	unsigned int vertexbuffer;
	unsigned int colorbuffer;
	unsigned int uvbuffer;

};

#endif
