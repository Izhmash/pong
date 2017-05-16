#include "SDL.h"
#include <string>
#include "Ball.h"

Ball::Ball(int xS, int yS, int x, int y, SDL_Texture* tex)
{
	speedX = xS;
	speedY = yS;
	locX = x;
	locY = y;
	texture = tex;
}

void Ball::setSpeedX(int xS)
{
	speedX = xS;
}

void Ball::setSpeedY(int yS)
{
	speedY = yS;
}

int Ball::getSpeedX()
{
	return speedX;
}

int Ball::getSpeedY()
{
	return speedY;
}

void Ball::setLocX(int x)
{
	locX = x;
}

void Ball::setLocY(int y)
{
	locY = y;
}

int Ball::getLocX()
{
	return locX;
}

int Ball::getLocY()
{
	return locY;
}

SDL_Texture* Ball::getTexture()
{
	return texture;
}