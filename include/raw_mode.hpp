#pragma once

#include<termios.h>

class RawMode {
	private:
		struct termios raw;
	
	public:
		void enable();
		void disable();
};
