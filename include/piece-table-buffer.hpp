#pragma once

#include <functional>
#include <string>
#include <vector>

#include "position.hpp"

enum PieceSource {
    ORIGINAL,
    ADD,
};

class Piece {
private:
    PieceSource source;

public:
    size_t offset;
    size_t length;
    Piece* prev;
    Piece* next;

    Piece(size_t offset, size_t length, PieceSource source = ADD, Piece* prev = nullptr, Piece* next = nullptr);

    PieceSource getSource() const;
    size_t getOffset() const;
    size_t getLength() const;

    void setNext(Piece* next);
};

using InsertCallback = std::function<void(const Position &start, const Piece &piece)>;
using RemoveCallback = std::function<void(const Position &start, const Position &end, const Piece &piece)>;
using ReplaceCallback = std::function<void(const Position &start, const Position &end, const Piece &piece)>;
using TraverseCallback = std::function<void(const std::string &text)>;

class PieceTable {
private:
    Piece* head;
    std::string original;
    std::string add;
    Piece* eofPiece; 
		std::vector<InsertCallback> insert_callbacks;
		std::vector<RemoveCallback> remove_callbacks;
		std::vector<ReplaceCallback> replace_callbacks;
		std::vector<TraverseCallback> traverse_callback;

public:
    PieceTable(std::string original);
    ~PieceTable();

    void init(std::string original);
    std::vector<std::string> read_lines();

    void insert(Position pos, std::string text);
    void insert(Position pos, char ch);
		void onInsert(InsertCallback callback) {
			insert_callbacks.push_back(callback);
		}

    void remove(Position &start, Position &end);
		void onRemove(RemoveCallback callback){
			remove_callbacks.push_back(callback);
		}

    void replace(Position &start, Position &end, std::string text);
		void onReplace(ReplaceCallback callback) {
			replace_callbacks.push_back(callback);
		}

		void traverse();
		void onTraverse(TraverseCallback callback) {
			traverse_callback.push_back(callback);
		}

		void debug_print();
		
		std::string read_piece(const Piece &piece);
		

private:
    size_t getAbsoluteIndex(Position pos);
};
