class NextImage
{
	private:
		SDL_Texture *image = nullptr;
		NextEngine engine;
	
	public:
		SDL_Surface *surface = nullptr;
		
		NextImage();
		NextImage(NextEngine);
		void setEngine(NextEngine);
		SDL_Texture* get();
		int load(std::string);
		int loadSurface(std::string);
		Uint32 getPixel(int,int);
};
 
