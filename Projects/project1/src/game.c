#include "game.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_t *game, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_t *game, unsigned int snum);
static char next_square(game_t *game, unsigned int snum);
static void update_tail(game_t *game, unsigned int snum);
static void update_head(game_t *game, unsigned int snum);

/* Task 1 */
game_t *create_default_game() {
  game_t* game = malloc(sizeof(game_t));
  game->num_rows = 18;
  game->board = malloc(sizeof(char*) * game->num_rows);
  /* 注意，在 C 语言中字符串，类似 "abc" 是放在 .rodata 上的，无权修改，如果修改会引发 segmentationn fault */
  /* 所以我们必须给 game->board 的每一行分配内存，然后再把 .rodata 上的内容复制进 heap 上，确保可以修改 */
  for (int i = 0; i < game->num_rows; ++i) {
    game->board[i] = malloc(22 * sizeof(char));
  }
  /* 注意 malloc 时分配的内存应该要包括 \n 与 \0 */
  /* 重要的是，在 C 语言中，string 末尾自带 \0，不用显式添加 */
  strcpy(game->board[0], "####################\n");
  strcpy(game->board[1], "#                  #\n");
  strcpy(game->board[2], "# d>D    *         #\n");
  strcpy(game->board[17], "####################\n");
  for (int i = 3; i <= 16; ++i) {
    strcpy(game->board[i], "#                  #\n");
  }
  game->num_snakes = 1;
  game->snakes = malloc(sizeof(snake_t));
  game->snakes->tail_row = 2;
  game->snakes->tail_col = 2;
  game->snakes->head_row = 2;
  game->snakes->head_col = 4;
  game->snakes->live = true;
  return game;
}

/* Task 2 */
void free_game(game_t *game) {
  free(game->snakes);
  for (int i = 0; i < game->num_rows; ++i) {
    free(game->board[i]);
  }
  free(game->board);
  free(game);
  return;
}

/* Task 3 */
void print_board(game_t *game, FILE *fp) {
  char** board = game->board;
  for (int i = 0; i < game->num_rows; ++i) {
    /* 通过查 man，我们知道了 fprintf 的参数，其中第一个是 stream，表示需要将内容 print 到函数参数给出的 fp 中 */
    fprintf(fp, "%s", board[i]);
  }
  return;
}

/*
  Saves the current game into filename. Does not modify the game object.
  (already implemented for you).
*/
void save_board(game_t *game, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(game, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_t *game, unsigned int row, unsigned int col) { return game->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_t *game, unsigned int row, unsigned int col, char ch) {
  game->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
    return true;
  }
  return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  if (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x') {
    return true;
  }
  return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  if (is_tail(c) || is_head(c) || c == '^' || c == '<' || c == 'v' || c == '>') {
    return true;
  }
  return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  if (c == '^') {
    return 'w';
  } else if (c == '<') {
    return 'a';
  } else if (c == '>') {
    return 'd';
  } else if (c == 'v') {
    return 's';
  }
  return '0';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  if (c == 'W') {
    return '^';
  } else if (c == 'A') {
    return '<';
  } else if (c == 'S') {
    return 'v';
  } else if (c == 'D') {
    return '>';
  }
  return '0';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  if (c == 'v' || c == 'S' || c == 's') {
    return cur_row + 1;
  } else if (c == '^' || c == 'w' || c == 'W') {
    return cur_row - 1;
  } else {
    return cur_row;
  }
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  if (c == '>' || c == 'd' || c == 'D') {
    return cur_col + 1;
  } else if (c == '<' || c == 'a' || c == 'A') {
    return cur_col - 1;
  } else {
    return cur_col;
  }
}

/*
  Task 4.2

  Helper function for update_game. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_t *game, unsigned int snum) {
  /* snakes 是一个 snake_t 的数组，所以需要传入 snum 来操控每一条蛇 */
  /* snakes[snum] 是一个 snake_t 结构体，所以访问其中元素不能用 ->，而要用 . */
  unsigned int head_row = game->snakes[snum].head_row;
  unsigned int head_col = game->snakes[snum].head_col;
  unsigned int next_row = get_next_row(head_row, get_board_at(game, head_row, head_col));
  unsigned int next_col = get_next_col(head_col, get_board_at(game, head_row, head_col));
  return get_board_at(game, next_row, next_col);
}

/*
  Task 4.3

  Helper function for update_game. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_t *game, unsigned int snum) {
  unsigned int head_row = game->snakes[snum].head_row;
  unsigned int head_col = game->snakes[snum].head_col;
  unsigned int next_row = get_next_row(head_row, get_board_at(game, head_row, head_col));
  unsigned int next_col = get_next_col(head_col, get_board_at(game, head_row, head_col));
  set_board_at(game, next_row, next_col, get_board_at(game, head_row, head_col));
  set_board_at(game, head_row, head_col, head_to_body(get_board_at(game, head_row, head_col)));
  /* 最后必须更新 snake 的其他成员变量，因为他们也是 snake 的一部分，也需要进行比较 */
  game->snakes[snum].head_col = next_col;
  game->snakes[snum].head_row = next_row;
}

/*
  Task 4.4

  Helper function for update_game. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_t *game, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 4.5 */
void update_game(game_t *game, int (*add_food)(game_t *game)) {
  // TODO: Implement this function.
  return;
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  // TODO: Implement this function.
  return NULL;
}

/* Task 5.2 */
game_t *load_board(FILE *fp) {
  // TODO: Implement this function.
  return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_t *game, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_t *initialize_snakes(game_t *game) {
  // TODO: Implement this function.
  return NULL;
}
