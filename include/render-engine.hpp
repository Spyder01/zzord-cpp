#pragma once

#include<cstdint>
#include<atomic>
#include <unordered_map>
#include<vector>

#include "position.hpp"

enum CursorType : uint8_t {
	BLINK_BLOCK = 0,
	STEADY_BLOCK,
	BLINK_UNDERLINE,
	STEADY_UNDERLINE,
	BLINK_BAR,
	STEADY_BAR,
};

typedef struct Cursor {
	CursorType type;
	Position pos;
} Cursor;

class RenderEngine {
	private:
		std::atomic<bool> is_blinking;
		Cursor cursor;
		std::unordered_map<uint64_t, uint64_t> line_info;

	public:
		RenderEngine();
		void clear_screen();
		void render_cursor();
		void render(char ch);
		void render(const std::string &text);
		void move_cursor(int i, int j);

		inline void set_cursor_type(CursorType cursor_type) {
			this->cursor.type = cursor_type;
		};

		inline Cursor get_cursor() const {
			return cursor;
		}
}; 
