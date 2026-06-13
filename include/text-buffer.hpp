#pragma once

#include<string>
#include<vector>
#include <sys/types.h>

#include "position.hpp"


template <typename Derived>
class TextBuffer {
	public:	
		TextBuffer(std::string contents) {};

		std::vector<std::string> read() {
			return static_cast<Derived*>(this)->readImpl();
		};
	 	void insert(Position &start, std::string &text) {
			static_cast<Derived*>(this)->insertImpl(start, text);
		};
	 	void remove(Position &start, Position &end) {
			static_cast<Derived*>(this)->removeImpl(start, end);
		}
		void replace(Position &start, Position &end, std::string &text) {
			static_cast<Derived*>(this)->replaceImpl(start, end, text);
		}
};

