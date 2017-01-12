#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#ifndef nglFileUtils_h__
#define nglFileUtils_h__



class nglFileUtils{
public:
	static bool FileExists(std::string);
	static std::string ReadStringFile(std::string);



};

#endif // nglFileUtils_h__