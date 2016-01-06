#ifndef TEXTMANAGER
#define TEXTMANAGER

#include"ft2build.h"
#include FT_FREETYPE_H
#include<vector>

const int HelpMaxCount = 480;
class message;

class textmanager {
public:
	textmanager();
	void init(const char* ttfpath);
	void initHelp();
	void rendertext(const char *text, float x, float y, float sx, float sy,float color[3],float blend);
	unsigned int gettex() { return tex; };
	void renderAllUIText();
	void renderHelp();
	void renderGG();
	void renderWIN();
	void renderPlayerStats();
	void handleMessage(message* m);
private:
	unsigned int thisbuffer;
	FT_GlyphSlot g;
	int atlas_w;
	int atlas_h;
	unsigned int tex;
	struct character_info {
		float ax; // advance.x
		float ay; // advance.y

		float bw; // bitmap.width;
		float bh; // bitmap.rows;

		float bl; // bitmap_left;
		float bt; // bitmap_top;

		float tx; // x offset of glyph in texture coordinates
	} c[128];

	struct helpInfo {
		int countDown;
		std::string helptext;
		float color[3];
	};

	std::vector<helpInfo> Help;
};

#endif
