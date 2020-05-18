#include <curses.h>
#include <form.h>
#include <string.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color);

int main() {
  FIELD *field[3];
  FORM *my_form;
  WINDOW *my_form_win;
  int ch, rows, cols;

  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  init_pair(1, COLOR_RED, COLOR_BLACK);
  field[0] = new_field(1, 10, 6, 1, 0, 0);
  field[1] = new_field(1, 10, 8, 1, 0, 0);
  field[2] = NULL;

  set_field_back(field[0], A_UNDERLINE);
  field_opts_off(field[0], O_AUTOSKIP);
  set_field_back(field[1], A_UNDERLINE);
  field_opts_off(field[1], O_AUTOSKIP);

  my_form = new_form(field);
  scale_form(my_form, &rows, &cols);

  my_form_win = newwin(rows + 4, cols + 4, 4, 4);
  keypad(my_form_win, TRUE);

  set_form_win(my_form, my_form_win);
  set_form_sub(my_form, derwin(my_form_win, rows, cols, 2, 2));

  box(my_form_win, 0, 0);
  print_in_middle(my_form_win, 1, 0, cols + 4, "My Form", COLOR_PAIR(1));

  post_form(my_form);
  wrefresh(my_form_win);

  mvprintw(LINES - 2, 0, "Use UP, DOWN arrow keys to switch between fields.");
  refresh();

  while ((ch = wgetch(my_form_win)) != KEY_F(1)) {
    switch (ch) {
    case KEY_DOWN:
      form_driver(my_form, REQ_NEXT_FIELD);
      form_driver(my_form, REQ_END_LINE);
      break;
    case KEY_UP:
      form_driver(my_form, REQ_PREV_FIELD);
      form_driver(my_form, REQ_END_LINE);
      break;
    case KEY_BACKSPACE:
      form_driver(my_form, REQ_DEL_PREV);
      break;
    default:
      form_driver(my_form, ch);
      break;
    }
  }

  unpost_form(my_form);
  free_form(my_form);
  free_field(field[0]);
  free_field(field[1]);
  endwin();
  return 0;
}

void print_in_middle(WINDOW *win, int starty, int startx, int width,
                     char *string, chtype color) {
  int length, x, y;
  float temp;

  if (win == NULL)
    win = stdscr;
  getyx(win, y, x);
  if (startx != 0)
    x = startx;
  if (starty != 0)
    y = starty;
  if (width == 0)
    width = 90;

  length = strlen(string);
  temp = (width / length) / 2;
  x = startx + (int)temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
}
