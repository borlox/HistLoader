#include <iostream>

#include "HistLoader.h"

bool StartsWithFollowedBy(const std::string& str, const std::string& withWhat, const std::string& byWhat);

int main(int argc, char *argv[])
{
	HistLoader histLoader;

	//std::cout << "Hello World!\n";
	//std::cout << StartsWithFollowedBy("template\t", "template", " \t");

	histLoader.LoadFile("hists.lst");

	std::cout << "xtitle: " << histLoader.GetDef("massTTbar").Get("xtitle") << "\n";
}
