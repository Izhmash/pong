#include "SDL.h"
#include "SDL_image.h"    
#include "SDL_TTF.h" 
#include <stdexcept>
#include <string>
#include <iostream>
#include <fstream>
#include "Ball.h"
#include "Paddle.h"

bool quit;

typedef struct
{
	int scoreP1, scoreP2;
}	Score;

Score score;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

static Ball ball = Ball(5, 5, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, nullptr);
static Paddle paddle = Paddle(5, SCREEN_HEIGHT / 2, nullptr);
static Paddle paddle2 = Paddle(SCREEN_WIDTH - 5, SCREEN_HEIGHT / 2, nullptr);

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

SDL_Texture* LoadImage(const std::string& file)
{
	SDL_Texture* tex = nullptr;
	tex = IMG_LoadTexture(renderer, file.c_str());
	if (tex == nullptr)
		throw std::runtime_error("Failed to load image: " + file + " " + IMG_GetError());
	return tex;
}

void ApplySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend, SDL_Rect *clip = NULL)
{
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;

	if (clip != NULL)
	{
		pos.w = clip->w;
		pos.h = clip->h;
	}
	else
	{
		SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);  //get entire texture size
	}

	SDL_RenderCopy(rend, tex, clip, &pos);
}

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
		logSDLError(std::cout, "TTF_OpenFont");
		return nullptr;
	}
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		logSDLError(std::cout, "CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

void render()
{
	SDL_Color color = { 255, 255, 255, 255 }; 
	SDL_Texture *scoreText = renderText(std::to_string(score.scoreP1) + " | " + std::to_string(score.scoreP2), "sample.ttf",
		color, 64, renderer);
	
	int scoreW, scoreH;
	SDL_QueryTexture(scoreText, NULL, NULL, &scoreW, &scoreH);

	SDL_RenderClear(renderer);

	ApplySurface(SCREEN_WIDTH / 2 - (scoreW / 2), SCREEN_HEIGHT / 4, scoreText, renderer, NULL);
	ApplySurface(paddle.getLocX(), paddle.getLocY(), paddle.getTexture(), renderer, NULL);
	ApplySurface(paddle2.getLocX(), paddle2.getLocY(), paddle2.getTexture(), renderer, NULL);
	ApplySurface(ball.getLocX(), ball.getLocY(), ball.getTexture(), renderer, NULL);

	SDL_RenderPresent(renderer);
}

void update()
{
	SDL_Event e;
	SDL_PumpEvents();
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
				/*case SDLK_DOWN:
				if (paddle.getLocY() < SCREEN_HEIGHT - paddleH)
				paddle.setLocY(paddle.getLocY() + 1);
				break;
				case SDLK_UP:
				if (paddle.getLocY() > 0)
				paddle.setLocY(paddle.getLocY() - 1);
				break;*/
				//For quitting, escape key
			case SDLK_ESCAPE:
				quit = true;
				break;
			default:
				break;
			}
		}
	}
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_UP])
	{
		if (paddle2.getLocY() > 0)
			paddle2.setLocY(paddle2.getLocY() - 7);
	}
	if (keystate[SDL_SCANCODE_DOWN])
	{
		if (paddle2.getLocY() < SCREEN_HEIGHT - paddle2.getTexH())
			paddle2.setLocY(paddle2.getLocY() + 7);
	}
	if (keystate[SDL_SCANCODE_W])
	{
		if (paddle.getLocY() > 0)
			paddle.setLocY(paddle.getLocY() - 7);
	}
	if (keystate[SDL_SCANCODE_S])
	{
		if (paddle.getLocY() < SCREEN_HEIGHT - paddle.getTexH())
			paddle.setLocY(paddle.getLocY() + 7);
	}

	if (ball.getLocX() == SCREEN_WIDTH)
	{
		//ball.setSpeedX(ball.getSpeedX() * -1);
		//quit = true;
		score.scoreP1 += 1;
		ball.setLocX(paddle.getLocX() + 20);
		ball.setLocY(paddle.getLocY());
	}
	if (ball.getLocX() == 0)
	{
		//ball.setSpeedX(ball.getSpeedX() * -1);
		//quit = true;
		score.scoreP2 += 1;
		ball.setLocX(paddle2.getLocX() - 20);
		ball.setLocY(paddle2.getLocY());
	}

	if (ball.getLocY() >= SCREEN_HEIGHT || ball.getLocY() <= 0)
	{
		ball.setSpeedY(ball.getSpeedY() * -1);
	}
	if (ball.getLocY() > paddle.getLocY() && ball.getLocY() < paddle.getLocY() + paddle.getTexH()
		&& ball.getLocX() == paddle.getLocX() + paddle.getTexW())
	{
		ball.setSpeedX(ball.getSpeedX() * -1);
	}
	if (ball.getLocY() > paddle2.getLocY() && ball.getLocY() < paddle2.getLocY() + paddle2.getTexH()
		&& ball.getLocX() == paddle2.getLocX() - paddle2.getTexW())
	{
		ball.setSpeedX(ball.getSpeedX() * -1);
	}
	

	ball.setLocX(ball.getLocX() + (1 * ball.getSpeedX()));
	ball.setLocY(ball.getLocY() + (1 * ball.getSpeedY()));
}

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		return 2;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		return 3;
	}

	if (TTF_Init() != 0){
		logSDLError(std::cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}
	
	try
	{
		ball = Ball(5, 5, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, LoadImage("D:/Users/Ian/Pics/Pong/ball.png"/*"ball.png"*/));
	}
	catch (const std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
		std::ofstream log("log.txt");
		log << e.what() << std::endl;
		return 4;
	}

	try
	{
		paddle = Paddle(5, SCREEN_HEIGHT / 2, LoadImage("D:/Users/Ian/Pics/Pong/paddle_green.png"/*"paddle_green.png"*/));
	}
	catch (const std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
		std::ofstream log("log.txt");
		log << e.what() << std::endl;
		return 5;
	}

	try
	{
		paddle2 = Paddle(SCREEN_WIDTH, SCREEN_HEIGHT / 2, LoadImage("D:/Users/Ian/Pics/Pong/paddle_green.png"/*"paddle_green.png"*/));
		paddle2.setLocX(SCREEN_WIDTH - paddle2.getTexW() - 5);
	}
	catch (const std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
		std::ofstream log("log.txt");
		log << e.what() << std::endl;
		return 6;
	}
	
	quit = false;	
	score.scoreP1 = 0;
	score.scoreP2 = 0;

	const int FRAMES_PER_SECOND = 60;
	const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
	Uint32 nextGameTick = SDL_GetTicks();
	int sleep_time = 0;
	while (!quit)
	{
		update();
		render();
		
		nextGameTick += SKIP_TICKS;
		sleep_time = nextGameTick - SDL_GetTicks();

		if (sleep_time >= 0)
			SDL_Delay(sleep_time);
	}
	SDL_DestroyTexture(ball.getTexture());
	SDL_DestroyTexture(paddle.getTexture());
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}


