#pragma once

#include <cstdint>
#include <string>
#include<atomic>

#include "./raw_mode.hpp"

#include "key-system.hpp"
#include "piece-table-buffer.hpp"
#include "render-engine.hpp"

template<typename KeySystemImpl>
class Editor {
	private:
		std::atomic<bool> is_blinking, isOpen;
		PieceTable buffer;
		RawMode rawMode;
		KeySystem<KeySystemImpl> key_system;
		RenderEngine render_engine;

	public:
		Editor(std::string content = "");
		bool editorShouldClose();
		void run();
		void clear_screen();

		inline PieceTable* getBuffer() noexcept {
			return &buffer;
		};

		RawMode* getRawMode() noexcept {
			return &rawMode;
		};

		~Editor();

};


