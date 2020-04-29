#include <ncurses.h>

int main() {
  initscr();
  addch('A' | A_BOLD | A_UNDERLINE);
  addch('B' | A_BOLD | A_UNDERLINE);
  addch('C');
  addch('D');
  addch('E' | A_BOLD | A_UNDERLINE);
  addch('F' | A_BOLD | A_UNDERLINE);
  addstr("\nADDSTR RESULT - NOT BOLD OR UNDERLINE");
  refresh();
  getch();
  endwin();
  return 0;
}
