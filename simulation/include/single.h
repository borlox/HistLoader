#include <iostream>

class single {
	public:
		bool sign;
		bool exponent[8];
		bool mantis[23];
		single();
		single(bool sign, int mant, int expo);
		void reset();
		friend std::ostream  &operator<<(std::ostream &o, const single& s);
};