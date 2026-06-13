#include <string_view>
#include "../../include/piece-table-buffer.hpp"

Piece::Piece(size_t offset, size_t length, PieceSource source, Piece* prev, Piece* next)
    : offset(offset), length(length), source(source), prev(prev), next(next) {}

PieceSource Piece::getSource() const { return source; }
size_t Piece::getOffset() const { return offset; }
size_t Piece::getLength() const { return length; }
void Piece::setNext(Piece* next) { this->next = next; }

PieceTable::PieceTable(std::string original) {
    init(original);
}

void PieceTable::init(std::string original) {
    this->original = original;
    add.clear();
    head = nullptr;

    if (!original.empty()) {
        head = new Piece(0, original.size(), ORIGINAL);
    }

    // Invisible EOF sentinel
    add.push_back('\0');
    eofPiece = new Piece(add.size() - 1, 1, ADD);

    if (head) {
        Piece* last = head;
        while (last->next) last = last->next;
        last->next = eofPiece;
        eofPiece->prev = last;
    } else {
        head = eofPiece;
    }
}

void PieceTable::traverse() {
    if (!head) return;

    Piece* curr = head;
    while (curr && curr != eofPiece) {
        auto& sourceStr = (curr->getSource() == ORIGINAL) ? original : add;
        std::string_view content(sourceStr.data() + curr->getOffset(), curr->getLength());

        for (auto& callback : traverse_callback)
            callback(content.data()); // or callback(content) if callback takes string_view

        curr = curr->next;
    }
}

std::vector<std::string> PieceTable::read_lines() {
	if (!head) return {};

	std::vector<std::string> lines = {};
	Piece* curr = head;
	std::string line = "";
	while(curr && curr != eofPiece) {
			auto& sourceStr = (curr->getSource() == ORIGINAL) ? original : add;
			std::string content  =  sourceStr.substr(curr->getOffset(), curr->getLength());
			for (char ch:content) {
				if (ch == '\n') {
					std::reverse(line.begin(), line.end());
					lines.push_back(line);
					line.clear();
				} else {
					line.push_back(ch);
				}
			}
			curr = curr->next;
	}

	std::reverse(line.begin(), line.end());
	lines.push_back(line);

	return lines;
}

void PieceTable::insert(Position pos, std::string text) {
    if (text.empty()) return;

    size_t addOffset = add.size();
    add += text;

    Piece* current = head;
    size_t line = 0, index = 0;

    // Find the piece corresponding to the position
    while (current && current != eofPiece) {
        size_t len = current->getLength();

        if (line == pos.line && index + len >= pos.index)
            break;

        std::string_view pieceData = (current->getSource() == ORIGINAL) ? original : add;
        pieceData.remove_prefix(current->getOffset());
        pieceData = pieceData.substr(0, len);

        for (char c : pieceData) {
            if (c == '\n') { line++; index = 0; }
            else index++;
        }

        current = current->next;
    }

    if (!current)
        current = eofPiece;

    size_t pieceRelOffset = pos.index - index;

    Piece* before = nullptr;
    Piece* after = nullptr;

    // Split current piece if insertion happens in the middle
    if (pieceRelOffset > 0) {
        before = new Piece(current->getOffset(), pieceRelOffset, current->getSource());
    }

    if (pieceRelOffset < current->getLength()) {
        after = new Piece(
            current->getOffset() + pieceRelOffset,
            current->getLength() - pieceRelOffset,
            current->getSource()
        );
    }

    Piece* inserted = new Piece(addOffset, text.size(), ADD);

    // Link before → inserted → after
    if (before) before->next = inserted;
    inserted->prev = before;

    inserted->next = after;
    if (after) after->prev = inserted;

    // Reconnect with current’s neighbors
    Piece* newStart = before ? before : inserted;
    Piece* newEnd   = after ? after : inserted;

    if (current->prev)
        current->prev->next = newStart;
    newStart->prev = current->prev;

    if (current->next)
        current->next->prev = newEnd;
    newEnd->next = current->next;

    // Update head if insertion was at the beginning
    if (current == head)
        head = newStart;

    // Safely remove current (only if not EOF)
    if (current != eofPiece) {
        current->prev = current->next = nullptr;
        delete current;
    }

    // Update insertion position
    pos.index += text.size();
}

void PieceTable::insert(Position pos, char ch) {
    insert(pos, std::string(1, ch));
}

/*
void PieceTable::remove(Position &start, Position end) {
    // TODO: implement piece splitting and merging for removal
}

void PieceTable::replace(Position &start, Position end, std::string text) {
    remove(start, end);
    insert(start, text);
}
*/

size_t PieceTable::getAbsoluteIndex(Position pos) {
    size_t absIndex = 0;
    auto curr = head;
    size_t line = 0, index = 0;

    while (curr && curr != eofPiece) {
        auto& sourceStr = (curr->getSource() == ORIGINAL) ? original : add;
        size_t len = curr->getLength();
        std::string_view pieceData(sourceStr.data() + curr->getOffset(), len);

        for (char ch : pieceData) {
            if (line == pos.line && index == pos.index)
                return absIndex;
            absIndex++;
            if (ch == '\n') { line++; index = 0; }
            else index++;
        }
        curr = curr->next;
    }

    return absIndex;
}

PieceTable::~PieceTable() {
    auto curr = head;
    while (curr) {
        auto temp = curr;
        curr = curr->next;
        delete temp;
    }
    head = nullptr;
}
