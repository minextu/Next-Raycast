#include "functions.cpp"

class NextImage;

class NextEngine
{
	public:
		SDL_Renderer *renderer;
	private:
		int width, height;
		Uint8 fillColorRed = 0, fillColorGreen = 0, fillColorBlue = 0, alpha = 255;
		TTF_Font* font = nullptr;
		SDL_Window *window;
		SDL_Texture *texTarget = nullptr;
		

	
	private:
		int initImage();
		void initFont();
	public:
		int createWindow(char*, int, int, bool);
		void updateSize(int, int);
		void clear();
		void hideMouse(bool);
		void centerMouse();
		void drawImage(NextImage, int, int);
		void drawImage(NextImage, int, int, int, int);
		void drawImage(NextImage, int, int, int, int, int, int, int, int);
		void drawImageTexture(SDL_Texture*, int, int, int, int, int, int, int, int);
		void drawImageSurface(SDL_Surface*, int, int, int, int);
		void fillRect(int, int, int, int);
		void strokeRect(int, int, int, int);
		void drawLine(int, int, int, int);
		void drawPixel(int,int);
		void setFont(std::string, int);
		void fillText(std::string, int, int);
		void setColor(std::string);
		void setColor(Uint32);
		void setColor(int, int, int);
		void setAlpha(int);
		void render();
		void prepareTexture(int, int);
		void setTargetTexture();
		void clearTarget();
		void renderTexture(int, int);
		void renderTextureR(int, int, int, int, double);
};
