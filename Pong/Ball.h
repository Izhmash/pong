class Ball
{
		int speedX;
		int speedY;
		int locX;
		int locY;
		SDL_Texture* texture;
	public:
		Ball(int xSpeed, int ySpeed, int x, int y, SDL_Texture* tex);
		int getSpeedX();
		int getSpeedY();
		void setSpeedX(int xS);
		void setSpeedY(int yS);
		int getLocX();
		int getLocY();
		void setLocX(int x);
		void setLocY(int y);
		SDL_Texture* getTexture();
};