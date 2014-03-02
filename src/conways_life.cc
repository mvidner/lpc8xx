#include "conways_life.h"
#include <cstring>

#include <cstdio>
ConwaysLife::ConwaysLife() {
  clear();
}

void ConwaysLife::clear() {
  current_board = 0;
  memset(boards, 0, sizeof(boards));
}

ConwaysLife::State ConwaysLife::get(int x, int y) const {
  const Board * board = &boards[current_board];
  x = (x + SIZE_X) % SIZE_X;
  y = (y + SIZE_Y) % SIZE_Y;
  unsigned char b = (*board)[SIZE_X * y + x / 8];
  unsigned int mask = 1 << (x % 8);
  return State(b & mask);
}

void ConwaysLife::set_board(unsigned x, unsigned y, ConwaysLife::State s, unsigned board_idx) {
  if (x < SIZE_X && y < SIZE_Y) {
    unsigned char mask = 1 << (x % 8);
    unsigned char * dest = &boards[board_idx][SIZE_X * y + x / 8];
    if (s) {
      *dest |= mask;
    }
    else {
      *dest &= ~mask;
    }
  }
}

void ConwaysLife::set(unsigned x, unsigned y, ConwaysLife::State s) {
  set_board(x, y, s, current_board);
}

void ConwaysLife::next() {
#define g(dx, dy) (unsigned ( get(x + (dx), y + (dy)) ))
  unsigned next_board = (current_board + 1) % NBOARDS;
  for (unsigned y = 0; y < size_y(); ++y) {
    for (unsigned x = 0; x < size_x(); ++x) {
      int count =
	g(-1, -1) + g(-1, 0) + g(-1, +1) +
	g( 0, -1) +     0    + g( 0, +1) +
	g(+1, -1) + g(+1, 0) + g(+1, +1);
      State next_state;
      if (g(0, 0))
	next_state = (count == 2 || count == 3)? ALIVE : DEAD;
      else
	next_state = (count == 3)? ALIVE : DEAD;
      set_board(x, y, next_state, next_board);
    }
  }
  current_board = next_board;
#undef g
}

int ConwaysLife::stabilized() const {
  for(unsigned other_offset = 1; other_offset < NBOARDS; ++other_offset) {
    int compared = memcmp(&boards[current_board], &boards[(current_board - other_offset + NBOARDS) % NBOARDS], sizeof(Board));
    if (compared == 0) {
      return other_offset;
    }
  }
  return 0;
}
