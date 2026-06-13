#include <stdexcept>
#include <unistd.h>

#include "../../include/key-system.hpp"
#include "../../include/zord-key-system.hpp"

void ZordKeySystem::handleKeyStrokeImpl(char input) {
	Key key;

	switch (input) {
		case 27: { // ESC
			char seq1, seq2;

			readChar(&seq1);
			readChar(&seq2);

			if (seq1 == '[') {
				switch (seq2) {
					case 'A':
						key.type = UP_ARROW;
						key.val = 0;
						break;
					case 'B':
						key.type = DOWN_ARROW;
						key.val = 0;
						break;
					case 'C':
						key.type = RIGHT_ARROW;
						key.val = 0;
						break;
					case 'D':
						key.type = LEFT_ARROW;
						key.val = 0;
						break;
					default:
						return; 
				}
			} else {
				key.type = ESCAPE;
				key.val = '\0';
			}
			break;
		}

		case '\n':
		case '\r':
			key.type = NEW_LINE;
			key.val = '\n';
			break;

		case ' ':
			key.type = SPACE;
			key.val = ' ';
			break;

		default:
			key.type = CHARACTER;
			key.val = input;
			break;
	}

	notify(key);
}

void ZordKeySystem::readChar(char *ch) {
    ssize_t r = read(STDIN_FILENO, ch, 1);
    if (r == -1) {
        throw std::runtime_error(std::string("Error reading character: ") + strerror(errno));
    }
    if (r == 0) {
			*ch = '\0';
    }
}


void ZordKeySystem::readImpl() {
	char ch;
	readChar(&ch);
	if (ch != '\0')
	handleKeyStrokeImpl(ch);

	ch = '\0';
}



