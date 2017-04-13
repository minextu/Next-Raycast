#ifndef RES_PATH_H
#define RES_PATH_H

#include <iostream>
#include <string>

/*
 * Get the resource path for resources located in res/
 * It's assumed the project directory is structured like:
 * bin/
 *  the executable
 * res/
 *
 * Paths returned will be Project_Root/res/
 */
std::string getResourcePath()
{
	#ifdef _WIN32
		const char PATH_SEP = '\\';
	#else
		const char PATH_SEP = '/';
	#endif
		static std::string baseRes;
		if (baseRes.empty()){
			char *basePath = SDL_GetBasePath();
			if (basePath){
				baseRes = basePath;
				SDL_free(basePath);
			}
			else {
				std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
				return "";
			}
			//We replace the last bin/ with res/ to get the the resource path
			size_t pos = baseRes.rfind("bin");
			baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
		}
		return baseRes;
}

#endif 
