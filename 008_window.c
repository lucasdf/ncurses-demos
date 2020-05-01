#include <ncurses.h>

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

void print_info(WINDOW *window, int cursor_x, int cursor_y) {
  int row, col;
  getmaxyx(stdscr, row, col);
  mvprintw(0, 0, "Press F1 to exit. x(%d) y(%d)", cursor_x, cursor_y);
  mvprintw(1, 0, "maxy_col(%d) maxx_row(%d)", col, row);
}

int main(int argc, char *argv[]) {
  WINDOW *my_win;
  int startx, starty, width, height;
  int ch;

  initscr();
  cbreak();
  keypad(stdscr, TRUE);

  height = 1;
  width = 10;
  starty = (LINES - height) / 2;
  startx = (COLS - width) / 2;
  int row, col;
  getmaxyx(stdscr, row, col);
  print_info(stdscr, startx, starty);
  refresh();
  my_win = create_newwin(height, width, starty, startx);

  // it is actually very inneficient to create and destroy_win a window for
  // each keypress
  while ((ch = getch()) != KEY_F(1)) {
    switch (ch) {
    case KEY_LEFT:
      startx--;
      starty = starty > row - 1 ? row - 1 : (starty < 1) ? 1 : starty;
      startx = startx > col - 1 ? col - 1 : (startx < 1) ? 1 : startx;
      destroy_win(my_win);
      my_win = create_newwin(height, width, starty, startx);
      break;
    case KEY_RIGHT:
      startx++;
      starty = starty > row - 1 ? row - 1 : (starty < 1) ? 1 : starty;
      startx = startx > col - 1 ? col - 1 : (startx < 1) ? 1 : startx;
      destroy_win(my_win);
      my_win = create_newwin(height, width, starty, startx);
      break;
    case KEY_UP:
      starty--;
      starty = starty > row - 1 ? row - 1 : (starty < 1) ? 1 : starty;
      startx = startx > col - 1 ? col - 1 : (startx < 1) ? 1 : startx;
      destroy_win(my_win);
      my_win = create_newwin(height, width, starty, startx);
      break;
    case KEY_DOWN:
      starty++;
      starty = starty > row - 1 ? row - 1 : (starty < 1) ? 1 : starty;
      startx = startx > col - 1 ? col - 1 : (startx < 1) ? 1 : startx;
      destroy_win(my_win);
      my_win = create_newwin(height, width, starty, startx);
      break;
    }
    print_info(stdscr, startx, starty);
    refresh();
  }

  endwin();
  return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx) {
  WINDOW *local_win;
  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0);
  wrefresh(local_win);
  return local_win;
}

void destroy_win(WINDOW *local_win) {
  // using box(local_win, ' ', ' ') would left the window corners on the screen
  wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  wrefresh(local_win);
  delwin(local_win);
}
