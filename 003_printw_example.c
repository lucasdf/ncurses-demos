#include <ncurses.h>
#include <string.h>

int main() {

  while (getch() != 'q') {
    char msg[][100] = {"Hello there.\n"
                       "Press 'q' to exit.\n"
                       "Experiment resizing the window.\n"};
    int max_row, max_col;
    initscr();
    getmaxyx(stdscr, max_row, max_col);

    for (int i = 0; i < 3; i++) {
      mvprintw(max_row / 2 - i, (max_col - strlen(msg[i])) / 2, "%s", msg[i]);
    }

    mvprintw(max_row - 2, 0, "This screen has %d rows and %d columns\n",
             max_row, max_col);
    refresh();
  }
  endwin();
  return 0;
}
