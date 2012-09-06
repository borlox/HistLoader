#include <iostream>
#include "single.h"

int main(int argc, char *argv[])
{
	single sing(0,0,0);
	single w(0,0,0);
	sing = 42.;
	//w = 1.;
	std::cout << sing << std::endl;
	float f =0;
	f= sing.getFloat();
	std::cout << "float " << f << std::endl;
	//std::cout << "add " << (sing + w).getFloat() << std::endl;
	return 0;
}