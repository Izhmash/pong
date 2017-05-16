#include "Paddle.h"

Paddle::Paddle(int x, int y, SDL_Texture* tex)
{
	locX = x;
	locY = y;
	SDL_QueryTexture(tex, NULL, NULL, &texW, &texH);
    texture = tex;
}

void Paddle::setLocX(int x)
{
	locX = x;
}

void Paddle::setLocY(int y)
{
	locY = y;
}

int Paddle::getLocX()
{
	return locX;
}

int Paddle::getLocY()
{
	return locY;
}

int Paddle::getTexW()
{
	return texW;
}

int Paddle::getTexH()
{
	return texH;
}

SDL_Texture* Paddle::getTexture()
{
	return texture;
}
