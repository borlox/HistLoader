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
	return o;
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
	sign = f > 0 ? false : true;
	uint32_t intc = (uint32_t) f;
	for (int i = 0; i < 23; i++) {
		mantis[i] = (intc & ( 1 << i )) >> i;
	}

	for (int i = 23; i < 31; i++) {
		exponent[i] = (intc & ( 1 << i )) >> i;
	}

	return *this;
}
	

float single::getFloat()
{
	uint32_t intc = 0;
	for (int i = 0; i < 23; i++) {
		intc |= mantis[i] << i;
	}

	for (int i = 23; i < 31; i++) {
		intc |= exponent[i] << i;
	}
	intc |= sign << 31;
	return (float) intc;
}

single& single::operator+(const single &s)
{
	sign = 0;
	
	for (int i = 0; i < 8; i++) {
		
	}
	bool carry  = 0;
	for (int i = 0; i < 23; i++) {
		bool x = mantis[i];
		bool y = s.mantis[i];
		bool cin = carry;
		bool cout;
		mantis[i] = x ^ y ^ cin ;
		cout = (x & y) | (cin & x) | (cin & y);
		carry = cout ;
	}
	return *this;
}

single& single::operator-(const single &s)
{

}

single& single::operator*(const single &s)
{

}

