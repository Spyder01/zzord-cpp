#pragma once

#include <functional>
#include <vector>
#include <cstdint>

enum KeyType : uint8_t {
	ESCAPE = 0,
	NEW_LINE,
	SPACE,
	UP_ARROW,
	DOWN_ARROW,
	RIGHT_ARROW,
	LEFT_ARROW,
	CHARACTER,
};

typedef struct Key {
	KeyType type;
	char val;
} Key;

using KeyEventCallback = std::function<void(const Key&)>;

template<typename Implementation>
class KeySystem {
	private:
		std::vector<KeyEventCallback> subscribers;

	public:
		void notify(Key key) {
			for (auto &callback:subscribers) {
				callback(key);
			}
		};

		void onKeyStroke(KeyEventCallback callback) {
			subscribers.push_back(callback);
		};

		void handleKeyStroke(char input) {
			static_cast<Implementation*>(this)->handleKeyStrokeImpl();
		};
		void readKey(){
			static_cast<Implementation*>(this)->readImpl();
		};
};

