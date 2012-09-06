#include <iostream>

#include "boost/cstdint.hpp"

using boost::uint8_t;
using boost::uint32_t;

class single {
	public:
		bool sign;
		bool exponent[8];
		bool mantis[23];
		single();
		single(bool sign, uint32_t mant, uint8_t expo);
		void reset();
		friend std::ostream  &operator<<(std::ostream &o, const single& s);
		single &operator=(const float &f);
		float operator=(const single &s);
		single &operator+(const single &s);
		single &operator-(const single &s);
		single &operator*(const single &s);
};