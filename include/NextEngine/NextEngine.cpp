#include "NextEngine.h"
#include "NextImage.h"
#include "cleanup.h"

int NextEngine::createWindow(char *title, int width, int height, bool vsync)
{
	this->width = width;
	this->height = height;
	
	//setup sdl
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0)
	{
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow(title, 100, 100, width,
		height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window == nullptr)
	{
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}
	
	this->window = window;
	
	SDL_Renderer *renderer = nullptr;
	
	if (vsync)
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	else
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	if (renderer == nullptr)
	{
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		SDL_Quit();
		return 1;
	}
	
	this->renderer = renderer;
	
	this->initFont();
	return this->initImage();
}

int NextEngine::initImage()
{
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}
	return 0;
}

void NextEngine::initFont()
{
	if (TTF_Init() != 0)
	{
		logSDLError(std::cout, "TTF_Init() Failed");
		SDL_Quit();
		return;
	}
}

void NextEngine::updateSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void NextEngine::hideMouse(bool status)
{
	int show;
	if (status == true)
		show = 0;
	else
		show = 1;
	SDL_ShowCursor(show);
}

void NextEngine::centerMouse()
{
	SDL_WarpMouseInWindow(this->window, this->width / 2, this->height / 2); 
}
void NextEngine::clear()
{
	// Set render color
	SDL_SetRenderDrawColor( this->renderer, 0, 0, 0, 255 );
	SDL_RenderClear(renderer);
}

void NextEngine::drawImage(NextImage image, int drawX, int drawY)
{
	SDL_Texture *texture = image.get();
	int width, height;
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	
	this->drawImage(image, drawX, drawY, width, height);
}

void NextEngine::drawImage(NextImage image, int drawX, int drawY, int width, int height)
{
	SDL_Texture *texture = image.get();
	int sourceWidth, sourceHeight;
	SDL_QueryTexture(texture, NULL, NULL, &sourceWidth, &sourceHeight);
	
	this->drawImage(image, 0, 0, sourceWidth, sourceHeight, drawX, drawY, width, height);
}

void NextEngine::drawImage(NextImage image, int sourceX, int sourceY, int sourceWidth, int sourceHeight, int drawX, int drawY, int width, int height)
{
	SDL_Texture *texture = image.get();
	this->drawImageTexture(texture, sourceX, sourceY, sourceWidth, sourceHeight, drawX, drawY, width, height);
}

void NextEngine::drawImageTexture(SDL_Texture *texture, int sourceX, int sourceY, int sourceWidth, int sourceHeight, int drawX, int drawY, int width, int height)
{
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = drawX;
	dst.y = drawY;
	dst.w = width;
	dst.h = height;
	
	SDL_Rect clip;
	clip.x = sourceX;
	clip.y = sourceY;
	clip.w = sourceWidth;
	clip.h = sourceHeight;
	
	SDL_SetTextureAlphaMod(texture,this->alpha);
	SDL_RenderCopy(this->renderer, texture, &clip, &dst);
}

void NextEngine::drawImageSurface(SDL_Surface *surface, int drawX, int drawY, int width, int height)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(this->renderer, surface);
	int sourceWidth, sourceHeight;
	SDL_QueryTexture(texture, NULL, NULL, &sourceWidth, &sourceHeight);
	
	this->drawImageTexture(texture, 0, 0, sourceWidth, sourceHeight, drawX, drawY, width, height);
}

void NextEngine::fillRect(int drawX, int drawY, int width, int height)
{
	SDL_Rect r;
	r.x = drawX;
	r.y = drawY;
	r.w = width;
	r.h = height;
	
	
	// Set render color
	SDL_SetRenderDrawColor( this->renderer, this->fillColorRed, this->fillColorGreen, this->fillColorBlue, this->alpha );
	
	SDL_RenderFillRect(renderer, &r);
}

void NextEngine::strokeRect(int drawX, int drawY, int width, int height)
{
	SDL_Rect r;
	r.x = drawX;
	r.y = drawY;
	r.w = width;
	r.h = height;
	
	
	// Set render color
	SDL_SetRenderDrawColor( this->renderer, this->fillColorRed, this->fillColorGreen, this->fillColorBlue, this->alpha );
	
	SDL_RenderDrawRect(renderer, &r);
}

void NextEngine::drawLine(int x1, int y1, int x2, int y2)
{
	SDL_SetRenderDrawColor( this->renderer, this->fillColorRed, this->fillColorGreen, this->fillColorBlue, this->alpha );
	SDL_RenderDrawLine( this->renderer, x1, y1, x2, y2 );
}

void NextEngine::drawPixel(int x, int y)
{
	// Set render color
	SDL_SetRenderDrawColor( this->renderer, this->fillColorRed, this->fillColorGreen, this->fillColorBlue, this->alpha );
	SDL_RenderDrawPoint(this->renderer, x, y);
}

void NextEngine::setFont(std::string fontPath, int fontSize)
{
	this->font = TTF_OpenFont(fontPath.c_str(), fontSize);
	if (font == NULL)
    {
		logSDLError(std::cout, "TTF_OpenFont() Failed");
		SDL_Quit();
		return;
    }
}

void NextEngine::fillText(std::string textString, int drawX, int drawY)
{     
	if (this->font == nullptr)
	{
		std::cout << "No font spezified!";
		SDL_Quit();
		return;
	}
	
	SDL_Color textColor = { this->fillColorRed, this->fillColorGreen, this->fillColorBlue };
	SDL_Surface* textSurface = TTF_RenderText_Solid(this->font, textString.c_str(), textColor);
	SDL_Texture* text = SDL_CreateTextureFromSurface(this->renderer, textSurface);
	int textWidth = textSurface->w;
	int textHeight = textSurface->h;
	SDL_FreeSurface(textSurface);
	SDL_Rect renderQuad = { drawX, drawY, textWidth, textHeight };
	SDL_RenderCopy(renderer, text, NULL, &renderQuad);
	SDL_DestroyTexture(text);
}

void NextEngine::setAlpha(int alpha)
{
	SDL_SetRenderDrawBlendMode(this->renderer,
                               SDL_BLENDMODE_BLEND);
	this->alpha = alpha;
}
void NextEngine::setColor(std::string colorName)
{
	if (colorName == "red")
		this->setColor(255,0,0);
	else if (colorName == "lightred")
		this->setColor(255,204,203);
	else if (colorName == "green")
		this->setColor(0,255,0);
	else if (colorName == "lightgreen")
		this->setColor(144,238,144);
	else if (colorName == "blue")
		this->setColor(0,0,255);
	else if (colorName == "lightblue")
		this->setColor(173,216,230);
	else if (colorName == "yellow")
		this->setColor(255,255,224);
	else if (colorName == "lightyellow")
		this->setColor(255,255,0);
	else if (colorName == "black")
		this->setColor(0,0,0);
	else if (colorName == "white")
		this->setColor(255,255,255);
	else if (colorName == "gray" || colorName == "grey")
		this->setColor(128,128,128);
	else if (colorName == "lightgray" || colorName == "lightgrey")
		this->setColor(211,211,211);
	else
		this->setColor(255,255,255);
}

void NextEngine::setColor(Uint32 color)
{
	int r = (color >> 16) & 255;
	int g = (color >> 8) & 255;
	int b = color & 255;
	this->setColor(r,g,b);
}
void NextEngine::setColor(int r, int g, int b)
{
	this->fillColorRed = r;
	this->fillColorGreen = g;
	this->fillColorBlue = b;
}

void NextEngine::prepareTexture(int width, int height)
{
	SDL_DestroyTexture(this->texTarget);
	this->texTarget = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888,
	SDL_TEXTUREACCESS_TARGET, width, height);
}

void NextEngine::setTargetTexture()
{
	SDL_SetRenderTarget(this->renderer, this->texTarget);
}

void NextEngine::clearTarget()
{
	SDL_SetRenderTarget(this->renderer, NULL);
}

void NextEngine::renderTexture(int drawX, int drawY)
{
	SDL_Rect dst;
	dst.x = drawX;
	dst.y = drawY;
	dst.w = width;
	dst.h = height;
	
	SDL_RenderCopy(this->renderer, this->texTarget, NULL, &dst);
	SDL_DestroyTexture(this->texTarget);
}

void NextEngine::renderTextureR(int drawX, int drawY, int centerX, int centerY, double angle)
{
	int angleDegress = angle * (180 / M_PI);
	SDL_Rect dst;
	dst.x = drawX;
	dst.y = drawY;
	dst.w = 1;
	dst.h = height;
	
	SDL_Point center = { centerX, centerY };
	SDL_RenderCopyEx(this->renderer, this->texTarget, NULL, &dst, angleDegress, &center, SDL_FLIP_NONE);
	SDL_DestroyTexture(this->texTarget);
}

void NextEngine::render()
{
	SDL_RenderPresent(this->renderer);
}
