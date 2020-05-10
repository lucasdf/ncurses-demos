#include <curses.h>
#include <form.h>

#define STARTX 15
#define STARTY 4
#define WIDTH 25
#define N_FIELDS 3

int main() {
  FIELD *field[N_FIELDS];
  FORM *my_form;
  int ch, i;

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  for (i = 0; i < N_FIELDS - 1; i++) {
    field[i] = new_field(1, WIDTH, STARTY + i * 2, STARTX, 0, 0);
  }
  field[N_FIELDS - 1] = NULL;

  set_field_back(field[1], A_UNDERLINE);
  field_opts_off(field[0], O_ACTIVE);
  field_opts_off(field[1], O_PUBLIC);
  field_opts_off(field[1], O_AUTOSKIP);

  my_form = new_form(field);
  post_form(my_form);
  refresh();

  set_field_just(field[0], JUSTIFY_CENTER);
  set_field_buffer(field[0], 0, "This is a static field");

  mvprintw(STARTY, STARTX - 10, "Field 1:");
  mvprintw(STARTY + 2, STARTX - 10, "Field 2:");
  refresh();

  while ((ch = getch()) != KEY_F(1)) {
    switch (ch) {
    case KEY_DOWN:
      form_driver(my_form, REQ_NEXT_FIELD);
      form_driver(my_form, REQ_END_LINE);
      break;
    case KEY_UP:
      form_driver(my_form, REQ_PREV_FIELD);
      form_driver(my_form, REQ_END_LINE);
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
