#include "single.h"

single::single()
{
	reset();
}

single::single(bool sign, int mant, int expo): sign(sign)
{
	for (int i = 0; i < 8; i++) {
		exponent[i] = ((expo >> i) && 0x01);
	}
	for (int i = 0; i < 23; i++) {
		mantis[i] = ((mant >> i) && 0x01);
	}
}

std::ostream  &operator<<(std::ostream &o, const single& s)
{
	o << s.sign;
	o << ' ';
	for (int i = 0; i < 8; i++) {
		o << s.exponent[i];
	}
	o << ' ';
	for (int i = 0; i < 23; i++) {
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