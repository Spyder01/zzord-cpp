#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include<stdexcept>

#include "../../include/raw_mode.hpp"

void RawMode::enable() {
	if (tcgetattr(STDIN_FILENO, &raw) == -1) throw std::runtime_error("tcgetattr");
		
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) throw std::runtime_error("tcsetattr");
}


void RawMode::disable() {
	 if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        throw std::runtime_error("tcsetattr");
}

