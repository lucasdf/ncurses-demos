#include <curses.h>
#include <stdio.h>

int main() {
  initscr();
  printw("Hello World !!!");
  refresh();
  getch();
  endwin();
  return 0;
}
