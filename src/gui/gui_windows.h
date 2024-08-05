// Copyright [2023] <Burr Etienne>

#ifndef SRC_GUI_GUI_WINDOWS_H_
#define SRC_GUI_GUI_WINDOWS_H_

#include <gtk/gtk.h>

#include "../calc/headers/calc_calculation.h"
#include "../calc/headers/calc_help_functions.h"

#define MAX_STRING_SIZE (256)

void graph_window(char *input);
int main_window(int argc, char **argv);

#endif  // SRC_GUI_GUI_WINDOWS_H_
