#include <cstdlib>
#include <unistd.h>
#include<iostream>

#include "../include/editor.hpp"
#include "../include/zord-key-system.hpp"

RawMode* globalRawModePointer = nullptr;

void disableRawModeAtExit() {
    if (globalRawModePointer) {
        globalRawModePointer->disable();
    }
}

int main() {
	Editor editor = Editor<ZordKeySystem>("Hello, World!!");
	globalRawModePointer = editor.getRawMode();
	std::atexit(disableRawModeAtExit);

	editor.run();
	
	try{
		editor.getRawMode()->disable();
	} catch(std::exception e) {
		std::cout<<e.what()<<"\n";
	}

	
	
	return 0;
}
