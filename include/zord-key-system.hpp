#pragma once

#include "key-system.hpp"


class ZordKeySystem : public KeySystem<ZordKeySystem> {
	private:
		void readChar(char *ch);
	public:
		void readImpl();
		void handleKeyStrokeImpl(char input);
};

