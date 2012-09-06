#include "single.h"

#include "boost/lexical_cast.hpp"

single::single()
{
	reset();
}

single::single(bool sign, uint32_t mant, uint8_t expo): sign(sign)
{
	for (int i = 0; i < 8; i++) {
		exponent[i] = (expo & ( 1 << i )) >> i;
	}
	for (int i = 0; i < 23; i++) {
		mantis[i] = (mant & ( 1 << i )) >> i;
	}
}

std::ostream  &operator<<(std::ostream &o, const single& s)
{
	o << s.sign;
	o << ' ';
	for (int i = 7; i >= 0; i--) {
		o << s.exponent[i];
	}
	o << ' ';
	for (int i = 22; i >= 0; i--) {
		o << s.mantis[i];
	}
}

void single::reset()
{
	sign = 0;
	for (int i = 0; i < 8; i++) {
		exponent[i] = 0;
	}
	for (int i = 0; i < 23; i++) {
		mantis[i] = 0;
	}
}

single &single::operator=(const float &f)
{

}
		

float single::operator=(const single &s)
{

}