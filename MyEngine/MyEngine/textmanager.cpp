#include "textmanager.h"
#include "glew.h"
#include<algorithm>
#include<string>
#include"shader.h"
#include"textmessage.h"
#include"playerstate.h"
#include"gameobject.h"

extern shader Tshader;
extern int width;
extern int height;
extern bool gameover;
extern bool win;
extern gameobject* player;

textmanager::textmanager()
{ 
	
}

void textmanager::init(const char * ttfpath)
{
	//library
	FT_Library ft;

	if (FT_Init_FreeType(&ft)) {
		fprintf(stderr, "Could not init freetype library\n");
		return;
	}

	//face
	FT_Face face;

	if (FT_New_Face(ft, ttfpath, 0, &face)) {
		fprintf(stderr, "Could not open font\n");
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	g = face->glyph;
	int w = 0;
	int h = 0;

	//load characters
	for (int i = 0; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}

		w += g->bitmap.width;
		h = std::max(h, g->bitmap.rows);

	}

	atlas_h = h;
	atlas_w = w;

	//generate atlas
	glActiveTexture(GL_TEXTURE9);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int x = 0;

	for (int i = 0; i < 128; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
			continue;
		}


		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		c[i].ax = g->advance.x >> 6;
		c[i].ay = g->advance.y >> 6;

		c[i].bw = g->bitmap.width;
		c[i].bh = g->bitmap.rows;

		c[i].bl = g->bitmap_left;
		c[i].bt = g->bitmap_top;

		c[i].tx = (float)x / atlas_w;
		x += g->bitmap.width;
	}

	glUseProgram(Tshader.getProgram());
	int loc = glGetUniformLocation(Tshader.getProgram(), "tex");
	glUniform1i(loc, 9);
	loc = glGetUniformLocation(Tshader.getProgram(), "width");
	glUniform1i(loc, width);
	loc = glGetUniformLocation(Tshader.getProgram(), "height");
	glUniform1i(loc, height);
	glUseProgram(0);

	initHelp();
	glGenBuffers(1, &thisbuffer);
}

void textmanager::initHelp()
{
	helpInfo hI;
	//initlize help message
	hI.helptext = "A doge woke up in a strange world,";
	hI.countDown = 480;
	hI.color[0] = 1.0f;
	hI.color[1] = 1.0f;
	hI.color[2] = 1.0f;
	Help.push_back(hI);
	hI.helptext = "Help him find the way out";
	hI.countDown = 480;
	hI.color[0] = 1.0f;
	hI.color[1] = 1.0f;
	hI.color[2] = 1.0f;
	Help.push_back(hI);
	hI.helptext = "Press arrow keys to move & jump";
	hI.countDown = 600;
	hI.color[0] = 1.0f;
	hI.color[1] = 1.0f;
	hI.color[2] = 1.0f;
	Help.push_back(hI);
	hI.helptext = "Press x to enter debug mode";
	hI.countDown = 600;
	hI.color[0] = 1.0f;
	hI.color[1] = 1.0f;
	hI.color[2] = 1.0f;
	Help.push_back(hI);
}

void textmanager::rendertext(const char * text, float x, float y, float sx, float sy,float color[3], float blend)
{
	struct point {
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
		void set(GLfloat fx, GLfloat fy, GLfloat fs, GLfloat ft) {
			x = fx;
			y = fy;
			s = fs;
			t = ft;
		}
	};

	int const len = 6 * strlen(text);
	point* coords = new point[len];
	int n = 0;

	for (const char *p = text; *p; p++) {
		float x2 = x + c[*p].bl * sx;
		float y2 = -y - c[*p].bt * sy;
		float w = c[*p].bw * sx;
		float h = c[*p].bh * sy;

		/* Advance the cursor to the start of the next character */
		x += c[*p].ax * sx;
		y += c[*p].ay * sy;

		/* Skip glyphs that have no pixels */
		if (!w || !h)
			continue;

		coords[n++].set(x2, -y2, c[*p].tx, 0);
		coords[n++].set(x2 + w, -y2, c[*p].tx + c[*p].bw / atlas_w, 0 );
		coords[n++].set(x2, -y2 - h, c[*p].tx, c[*p].bh / atlas_h ); //remember: each glyph occupies a different amount of vertical space
		coords[n++].set(x2 + w, -y2, c[*p].tx + c[*p].bw / atlas_w, 0 );
		coords[n++].set(x2, -y2 - h, c[*p].tx, c[*p].bh / atlas_h );
		coords[n++].set(x2 + w, -y2 - h, c[*p].tx + c[*p].bw / atlas_w, c[*p].bh / atlas_h );
	}

	glUseProgram(Tshader.getProgram());
	
	glBindBuffer(GL_ARRAY_BUFFER, thisbuffer);
	glBufferData(GL_ARRAY_BUFFER, len * sizeof(point), coords, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, thisbuffer);
	glVertexAttribPointer(
		0,                  // must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	int loc = glGetUniformLocation(Tshader.getProgram(), "color");
	glUniform3fv(loc, 1, &color[0]);

	loc = glGetUniformLocation(Tshader.getProgram(), "blendindex");
	glUniform1f(loc, blend);
	
	glDrawArrays(GL_TRIANGLES, 0, n);
	glUseProgram(0);
	delete[] coords;
}

void textmanager::renderAllUIText()
{
	renderHelp();
	//renderPlayerStats();
	if (gameover)
		renderGG();
	if (win)
		renderWIN();
}

void textmanager::renderHelp()
{
	if (Help.size() <= 0)
		return;
	int renderHeight = 600;
	for (auto i = Help.begin();i != Help.end();)
	{
		if ((*i).countDown > 0)
		{ 
			renderHeight -= 50;
			rendertext((*i).helptext.c_str(), width/2 -250, renderHeight, 0.7, 0.7, (*i).color, 2.0f*(*i).countDown/HelpMaxCount);
			(*i).countDown--;
			i++;
		}
		else
		{
			i = Help.erase(i);
		}

	}
}

void textmanager::renderGG()
{
	float red[3] = { 1,0,0 };
	rendertext("YOU DIED! Press S to reset to home and continue", 150, 550, 1, 1,red, 1.0);
	rendertext("or press R to restart", width / 2 - 250, 500, 1, 1, red, 1.0);
}

void textmanager::renderWIN()
{
	float red[3] = { 1,0,0 };
	rendertext("CONGRATULATIONS! You saved the doge!", width / 2 - 450, 550, 1, 1, red, 1.0);
	rendertext("feel free to donate $1 to save more doges :D (joke)", width / 2 - 550, 500, 1, 1, red, 1.0);
}

void textmanager::renderPlayerStats()
{
	playerstate* currentPlayerState = static_cast<playerstate*>(player->getcomponent(T_GAMESTATE));
	std::string hpBar = "HP:";
	for (int i = 0;i < currentPlayerState->HPLeft;i++)
	{
		hpBar += "@";
	}
	float green[3] = { 0,1,0 };
	rendertext(hpBar.c_str(), 0, 850, 0.8, 0.8, green, 1.0);
}

void textmanager::handleMessage(message * m)
{
	if (m->MType == M_RENDERTEXT)
	{
		textmessage* tm = static_cast<textmessage*>(m);
		helpInfo hI;
		hI.helptext = tm->thistext;
		hI.countDown = 480;
		hI.color[0] = tm->color[0];
		hI.color[1] = tm->color[1];
		hI.color[2] = tm->color[2];
		Help.push_back(hI);
	}
}
