#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <Piece.h>
#include <array>

class Board {
public:
	using Rank = std::array<std::optional<Piece>, 8>;

	Board();
	Board(std::array<Rank, 8>);

	Square get_dimensions() const { return { 8, 8 }; }
	bool is_in_bounds(Square) const;
	std::optional<Piece> get_piece(Square) const;
	MoveDetails move(Move);
	bool is_piece_under_attack(Square) const;
	Square find_king(color) const;
	Square get_en_passant_target() const { return en_passant_target; }
	bool is_occupied(Square square) const { return get_piece(square).has_value(); }
	bool is_out_of_bounds(Square square) const { return !is_in_bounds(square); }

	class Iterator {
	public:
		Iterator(Square current)
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

	Iterator begin() const { return { { get_dimensions().rank - 1, 0 } }; }
	Iterator end() const { return { { -1, 0 } }; }

private:
	void force_move(Move, MoveDetails);
	void move_one_piece(Square, Square);
	std::optional<Piece>& operator[](Square);
	const std::optional<Piece>& operator[](Square) const;
	std::optional<Piece>& at(Square);
	const std::optional<Piece>& at(Square) const;

	std::array<Rank, 8> ranks;
	Square en_passant_target{};
};

#endif
