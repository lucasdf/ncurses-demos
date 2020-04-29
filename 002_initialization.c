#include <ncurses.h>
#include <stdio.h>

int main() {
  int ch;
  initscr();
  raw(); // liner buffer disabled. dont need to <CR> to get input with getch
  keypad(stdscr, TRUE); // enable F-keys
  noecho();             // no echo on getch. avoid escape characters on screen

  printw("Type something to see it in bold. Press F1 to quit.\n");
  ch = getch();

  printw("The pressed key is ");
  while (ch != KEY_F(1)) {
    attron(A_BOLD);
    printw("%c", ch);
    attroff(A_BOLD);
    refresh();
    ch = getch();
  }

  endwin();

  return 0;
}
