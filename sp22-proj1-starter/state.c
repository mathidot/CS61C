#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t* state, int x, int y);
static void set_board_at(game_state_t* state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t* state, int snum);
static char next_square(game_state_t* state, int snum);
static void update_tail(game_state_t* state, int snum);
static void update_head(game_state_t* state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t* state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t* state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
#define BOARD_X 14
#define BOARD_Y 10
game_state_t* create_default_state() {
  // TODO: Implement this function.
  game_state_t *initial_game_state = (game_state_t *)malloc(sizeof(game_state_t));
  assert(initial_game_state);
  initial_game_state->x_size = BOARD_X;
  initial_game_state->y_size = BOARD_Y;

  // Initialize the board settings
  char **board = (char **)malloc(sizeof(char *) * BOARD_Y);
  assert(board);
  for(int i = 0;i < BOARD_Y;i++){
    board[i] = (char *)malloc(sizeof(char) * (BOARD_X + 1));
    assert(board[i]);
  }
  strcpy(board[0],"##############");
  strcpy(board[9],"##############");
  for(int i = 1;i <= 8;i++){
    if(i == 2){
      strcpy(board[i],"#        *   #");
    }else if(i == 4){
      strcpy(board[i],"#   d>       #");
    }else{
      strcpy(board[i],"#            #");
    }
  }

  initial_game_state->board = board;
  // Initialize the num_snakes
  initial_game_state->num_snakes = 1;
  
  //Initialize the snakes
  initial_game_state->snakes = (snake_t *)malloc(sizeof(game_state_t) * initial_game_state->num_snakes);
  assert(initial_game_state->snakes);
  initial_game_state->snakes->tail_x = 4;
  initial_game_state->snakes->tail_y = 4;
  initial_game_state->snakes->head_x = 5;
  initial_game_state->snakes->head_y = 4;
  initial_game_state->snakes->live = true; 
  return initial_game_state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
  int i;
  for(i = 0;i < BOARD_Y;i++){
    free(state->board[i]);
  }

  free(state->board);

  /*
  Why free memory like this, will cause bug?
  for(i = 0;i < state->num_snakes;i++){
    free(state->snakes + i);
  }
  */
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
  for(uint32_t i = 0;i < state->y_size;i++){
    fprintf(fp,"%s\n",state->board[i]);
  }
  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
  // TODO: Implement this function.
  return (c == 'w' || c == 'a' || c == 's' || c == 'd');
}

static bool is_body(char c) {
  return (c == '^' || c == 'v' || c == '<' || c == '>'
          || c == 'w' || c == 'a' || c == 's' || c == 'd');
}

static bool is_snake(char c) {
  // TODO: Implement this function.

return (c == 'w' || c == 'a' || c == 's' || c == 'd' ||
          c == '^' || c == 'v' || c == '<' || c == '>' || c == 'x');

}

static char body_to_tail(char c) {
  // TODO: Implement this function.
  switch (c)
  {
    case '^': return 'w';
    case 'v': return 's';
    case '<': return 'a';
    case '>': return 'd';
    default:  assert(false);
  }
}

static int incr_x(char c) {
  // TODO: Implement this function.
  if(c == '>' || c == 'd'){
    return 1;
  }else if(c == '<' ||c == 'a'){
    return -1;
  }else{
    return 0;
  }
}

static int incr_y(char c) {
  // TODO: Implement this function.
  if(c == 'v' || c == 's'){
    return 1;
  }else if(c == '^' ||c == 'w'){
    return -1;
  }else{
    return 0;
  }
}

/* Task 4.2 */
static char next_square(game_state_t* state, int snum) {
  // TODO: Implement this function.
    snake_t *snake = state->snakes + snum;
    char snake_head_char = state->board[snake->head_y][snake->head_x];
    int move_x = snake->head_x + incr_x(snake_head_char);
    int move_y = snake->head_y + incr_y(snake_head_char);
    return get_board_at(state,move_x,move_y);
}

/* Task 4.3 */
static void update_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
    snake_t *snake = state->snakes + snum;
    char snake_head_char = get_board_at(state,snake->head_x,snake->head_y);
    int move_x = snake->head_x + incr_x(snake_head_char);
    int move_y = snake->head_y + incr_y(snake_head_char);
    set_board_at(state,move_x,move_y,snake_head_char);
    snake->head_x = move_x;
    snake->head_y = move_y;
    return;
}

/* Task 4.4 */
static void update_tail(game_state_t* state, int snum) {
  // TODO: Implement this function.
    snake_t *snake = state->snakes + snum;
    char snake_tail_char = get_board_at(state,snake->tail_x,snake->tail_y);
    int move_x = snake->tail_x + incr_x(snake_tail_char);
    int move_y = snake->tail_y + incr_y(snake_tail_char);
    set_board_at(state,snake->tail_x,snake->tail_y,' ');
    set_board_at(state,move_x,move_y,body_to_tail(get_board_at(state,move_x,move_y)));
    snake->tail_x = move_x;
    snake->tail_y = move_y;
    return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  uint32_t i;
  for(i = 0;i < state->num_snakes;i++){
    snake_t *snake = state->snakes + i;
    char snake_head_char = get_board_at(state,snake->head_x,snake->head_y);
    int move_x = snake->head_x + incr_x(snake_head_char);
    int move_y = snake->head_y + incr_y(snake_head_char);
    char snake_move_to_char = get_board_at(state,move_x,move_y);

    if(is_body(snake_move_to_char) || snake_move_to_char == '#'){
      set_board_at(state,snake->head_x,snake->head_y,'x');
      snake->live = false;
    }
    else if(snake_move_to_char == '*'){
      update_head(state,i);
      set_board_at(state,snake->head_x,snake->head_y,snake_head_char);
      add_food(state);
    }
    else{
      update_head(state,i);
      update_tail(state,i);
    }
  }
  return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implement this function.
  game_state_t *game = (game_state_t*)malloc(sizeof(game_state_t));
  game->board = (char**)malloc(sizeof(char*)*100);
  char buffer[1000];  
  game->snakes = (snake_t*)malloc(sizeof(snake_t)*100);   
  FILE *f;
  f = fopen(filename, "r");
  if (f == NULL) {
    fprintf(stderr, "\nError opening file\n");
    exit(1);
  }
  int i = 0;
  int const_length = 0;
  while (!feof(f)) {
    fgets(buffer, 1000, f);
    const_length = strlen(buffer);
    game->board[i] = (char*)malloc(sizeof(char)*const_length);
    for(int j = 0; j < const_length; j++) {
      game->board[i][j] = buffer[j];
    }
    // printf("%s",buffer);
    game->board[i][const_length-1] = '\0';
    i++;
  }
  game->x_size = const_length - 1;
  game->y_size = i - 1;

  fclose(f);
  return game;
}

/* Task 6.1 */
static void find_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  snake_t *snake = state->snakes + snum;
  uint32_t current_pos_x = snake->tail_x;
  uint32_t current_pos_y = snake->tail_y;
  char current_char;
  uint32_t current_pos_x_cpy = current_pos_x;
  uint32_t current_pos_y_cpy = current_pos_y;

  for(;;){
    current_char = get_board_at(state,current_pos_x,current_pos_y);
    switch (current_char)
    {
    case 'w':
      current_pos_y_cpy--;
      break;
    case 'a':
      current_pos_x_cpy--;
      break;
    case 's':
      current_pos_y_cpy++;
      break;
    case 'd':
      current_pos_x_cpy++;
      break;
    case '^':
      current_pos_y_cpy--;
      break;
    case 'v':
      current_pos_y_cpy++;
      break;
    case '<':
      current_pos_x_cpy--;
      break;
    case '>':
      current_pos_x_cpy++;
      break;
    default:
      break;
    }

    if(!is_snake(get_board_at(state,current_pos_x_cpy,current_pos_y_cpy))){
      snake->head_x = current_pos_x;
      snake->head_y = current_pos_y;
      return;
    }
    current_pos_x = current_pos_x_cpy;
    current_pos_y = current_pos_y_cpy;
  }
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  uint32_t i = 0;
  for(uint32_t y = 1; y < state->y_size-1;y++)
    for(uint32_t x = 1;x < state->x_size-1;x++){
      if(is_tail(get_board_at(state,x,y))){
        (state->snakes + i)->tail_x = x;
        (state->snakes + i)->tail_y = y;
        find_head(state,i);
        (state->snakes + i)->live = true;
        i++;
      }
    }
  state->num_snakes = i;
  return state;
}
