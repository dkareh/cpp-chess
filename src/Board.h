// Author: Daniel Kareh
// Summary: A class that represents the entire state of a chess board *except*
//          who moves next (that is stored by the game object).

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <array>
#include <functional>
#include "Piece.h"

class Board {
public:
	using Rank = std::array<std::optional<Piece>, 8>;
	using ChooseMoveCallback = std::function<int(const std::vector<MoveDetails>&)>;

	Board();
	explicit Board(std::array<Rank, 8>);

	Square get_dimensions() const { return { 8, 8 }; }
	bool is_in_bounds(Square) const;
	std::optional<Piece> get_piece(Square) const;
	std::optional<MoveDetails> move(Move, const ChooseMoveCallback&);
	std::vector<MoveDetails> get_legal_moves(Move) const;
	bool is_piece_under_attack(Square) const;

	// Return true if moving the piece from one square to another would
	// place it under attack.
	bool would_piece_be_attacked(Square from, Square to) const;

	Square find_king(color) const;
	Square get_en_passant_target() const { return en_passant_target; }
	bool is_occupied(Square square) const { return get_piece(square).has_value(); }
	bool is_out_of_bounds(Square square) const { return !is_in_bounds(square); }

	// An iterator type that goes through each square of a board.
	// Iteration starts at square 'a8' and ends at square 'h1'.
	class Iterator {
	public:
		explicit Iterator(Square current)
			: current{ current } {}

		const Square& operator*() const { return current; }
		const Square* operator->() const { return &current; }
		Iterator& operator++();
		Iterator operator++(int);
		bool operator==(const Iterator&) const;
		bool operator!=(const Iterator&) const;

	private:
		Square current;
	};

	// By defining `begin` and `end`, we can use C++ range-based for loops on
	// instances of `Board`!
	Iterator begin() const { return Iterator{ { get_dimensions().rank - 1, 0 } }; }
	Iterator end() const { return Iterator{ { -1, 0 } }; }

private:
	// Apply a move to the board, ignoring whether the move is legal or not.
	// This is private because external code shouldn't be able to manipulate
	// the board in illegal ways.
	void force_move(Move, MoveDetails);

	Piece& move_one_piece(Square, Square);
	Piece pick_up(Square);
	Piece& put_down(Square, Piece);
	std::optional<Piece>& operator[](Square);
	const std::optional<Piece>& operator[](Square) const;
	std::optional<Piece>& at(Square);
	const std::optional<Piece>& at(Square) const;

	std::array<Rank, 8> ranks;
	Square en_passant_target{};
};

#endif
