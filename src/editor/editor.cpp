#include <iostream>
#include <string>
#include <unistd.h>

#include "../../include/editor.hpp"

template<typename K>
Editor<K>::Editor(std::string content) : buffer(PieceTable(content)), render_engine(RenderEngine()), key_system(KeySystem<K>()), rawMode(RawMode()), isOpen(true), is_blinking(true) {
	rawMode.enable();
};

template<typename K>
bool Editor<K>::editorShouldClose() {
	return !isOpen;
};


template<typename K>
void Editor<K>::run() {
	int i = 0;
	buffer.onTraverse([this, &i](const std::string &text){
			render_engine.render("PRESSED " + std::to_string(++i) + " " + text);
	});

	key_system.onKeyStroke([this, &i](const Key& key){
		switch(key.type) {
		case CHARACTER:
		case SPACE:
		case NEW_LINE:
			if (key.val == 'q') isOpen = false;
			else {
				buffer.insert(render_engine.get_cursor().pos, key.val);
			}
			break;
			case UP_ARROW:
				render_engine.move_cursor(0, 1);
				break;
			case RIGHT_ARROW:
				render_engine.move_cursor(1, 0);
				break;
		}		
	});

	while(isOpen) {
		render_engine.clear_screen();
		key_system.readKey();
		//buffer.traverse();
		for (auto line:buffer.read_lines()) {
			render_engine.render(line);
			render_engine.render('\n');
		}
		render_engine.render_cursor();
	}
}

template<typename K>
Editor<K>::~Editor() {
	rawMode.disable();
}

#include "../../include/zord-key-system.hpp"
template class Editor<ZordKeySystem>;
