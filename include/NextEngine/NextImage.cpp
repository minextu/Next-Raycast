NextImage::NextImage()
{
	
}
NextImage::NextImage(NextEngine engine)
{
	this->setEngine(engine);
}

void NextImage::setEngine(NextEngine engine)
{
	this->engine = engine;
}

int NextImage::loadSurface(std::string imagePath)
{
	SDL_Surface* loadedImage = IMG_Load(imagePath.c_str());
	
	//If nothing went wrong in loading the image
    if( loadedImage == NULL )
		logSDLError(std::cout, "LoadSurface");
	
	this->surface = loadedImage;
	
	return 0;
}

int NextImage::load(std::string imagePath)
{
	SDL_Texture *texture = IMG_LoadTexture(this->engine.renderer, imagePath.c_str());
	
	if (texture == nullptr)
		logSDLError(std::cout, "LoadTexture");

	this->image = texture;
	
	return 0;
}

SDL_Texture* NextImage::get()
{
	return this->image;
}

Uint32 NextImage::getPixel(int x, int y)
{
	//Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)this->surface->pixels;
    
    //Get the requested pixel
    return pixels[ ( y * this->surface->w ) + x ];
}
