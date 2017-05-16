#include "SDL.h"
#include <string>
class Paddle
{
		int locX;
		int locY;
		int texW;
		int texH;
		SDL_Texture* texture;
	public:
		Paddle(int x, int y, SDL_Texture* texture);
		int getLocX();
		int getLocY();
		int getTexW();
		int getTexH();
		void setLocX(int);
		void setLocY(int);
		SDL_Texture* getTexture();
};