#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include "nglFileUtils.h"

bool nglFileUtils::FileExists(std::string file_path){

	std::ifstream f(file_path.c_str());
	if (f.good()) {
		f.close();
		return true;
	} else {
		f.close();
		return false;
	}   


}

std::string nglFileUtils::ReadStringFile(std::string file_path){

	if (FileExists(file_path)){

		std::ifstream t(file_path);
		std::string str;

		t.seekg(0, std::ios::end);   
		str.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());

		return str;

	} else {

		return std::string();
	}


}