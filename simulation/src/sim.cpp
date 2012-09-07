#include <iostream>
#include "single.h"

int main(int argc, char *argv[])
{
	single sing;
	single w;
	sing = 42.;
	w = 1.;
	std::cout << "   " <<sing << std::endl;
	float f =0;
	f= sing.getFloat();
	//std::cout << "float " << f << std::endl;
	//std::cout << "w " << w.getFloat() << std::endl;
	std::cout << " + " <<w << std::endl;
	//std::cout << "add " << (sing + w).getFloat() << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	single sum = w+sing;
	std::cout << "   "<< sum << std::endl;
	return 0;
}