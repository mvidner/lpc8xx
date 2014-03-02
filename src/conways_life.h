#ifndef CONWAYS_LIFE_H
#define CONWAYS_LIFE_H

class ConwaysLife {
  static const unsigned SIZE_X = 84;
  static const unsigned SIZE_Y = 48;
  static const unsigned NBOARDS = 7;
public:
  enum State { DEAD = 0, ALIVE = 1 };

  ConwaysLife();

  void clear();
  void next();
  // return 0 if the board has not yet stabilized,
  // or the oscillation period
  int stabilized() const;

  unsigned size_x() const { return SIZE_X; };
  unsigned size_y() const { return SIZE_Y; };
  
  // operates on the current_board
  State get(int x, int y) const;
  // operates on the current_board
  void set(unsigned x, unsigned y, State s);
  void set_board(unsigned x, unsigned y, State s, unsigned board_idx);

private:
  typedef State Board[SIZE_X * SIZE_Y];
  Board boards[NBOARDS];
  unsigned current_board;
};
#endif
