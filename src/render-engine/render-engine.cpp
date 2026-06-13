#include<iostream>
#include <string>

#include "../../include/render-engine.hpp"

RenderEngine::RenderEngine() : cursor(Cursor{STEADY_BLOCK, {0, 0}}), is_blinking(true), line_info({}) {};

void RenderEngine::clear_screen() {
	 std::cout << "\x1B[2J\x1B[H";
}

void RenderEngine::render_cursor() {
	std::cout << "\033[" << cursor.pos.line+1 << ";" << cursor.pos.index+1<<"H";

		switch(cursor.type) {
        case BLINK_BLOCK:
						if(!is_blinking)
            std::cout << "\033[0 q"; 
            break;
        case STEADY_BLOCK:
            std::cout << "\033[2 q";
            break;
        case BLINK_UNDERLINE:
						if(!is_blinking)
            std::cout << "\033[3 q";
            break;
        case STEADY_UNDERLINE:
            std::cout << "\033[4 q";
            break;
        case BLINK_BAR:
						if(!is_blinking)
            std::cout << "\033[5 q";
            break;
        case STEADY_BAR:
            std::cout << "\033[6 q";
            break;
        default:
            std::cout << "\033[1 q";
            break;
    }
    std::cout.flush();
}

void RenderEngine::move_cursor(int i, int j) {
	int ni = cursor.pos.index + i, nj = cursor.pos.line + j;
	//if (ni > 0 && ni < line_info[cursor.pos.line] && nj > 0 && nj < line_info.size()) {
		cursor.pos.index = ni;
		cursor.pos.line = nj;
	//}  
}

void RenderEngine::render(char ch) {
	if (ch == '\0') return;

	std::cout<<ch;
	ch == '\n'?move_cursor(1, 0):move_cursor(0, 1);
}

void RenderEngine::render(const std::string &text) {
	int line_no = 0;
	for (int i=0; i<text.size(); ++i) {
		char ch = text[i];
		if (ch == '\n') {
			++line_no;
		}
		else {
			++line_info[line_no];
		}
		render(ch);
	}
}
   
