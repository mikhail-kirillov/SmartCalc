// Copyright [2023] <Burr Etienne>

#include <cairo.h>
#include <math.h>
#include <pango/pangocairo.h>

#include "../gui/gui_windows.h"

#define GRAPH_LABEL_MAX_STRING_SIZE (128)
#define GRAPH_LABEL_SCALE_TEXT ("SCALE: %d/%.2f")

#define GRAPH_DRAW_AREA_SIZE_SQUARE (600)
#define GRAPH_DRAW_ROTATE_TEXT (1)
#define GRAPH_DRAW_DONT_ROTATE_TEXT (0)
#define GRAPH_DRAW_STEP (10)
#define GRAPH_DRAW_STEP_PAUSE (2)
#define GRAPH_DARW_LINE_WIDTH_NUM (2)
#define GRAPH_DARW_AXIS_WIDTH_NUM (0.5)
#define GRAPH_DRAW_TEXT_SIZE (10)
#define GRAPH_DRAW_UNIT_SCALE (1.0)

#define GRAPH_DRAW_STEP_GET(scale) (GRAPH_DRAW_STEP * scale)

// Data struct for function drawing
typedef struct graph_data {
  cairo_t *cr;
  gdouble spin_x_max;
  gdouble spin_x_min;
  gdouble spin_y_max;
  gdouble spin_y_min;
  gdouble right_limit;
  gdouble left_limit;
  gdouble low_limit;
  gdouble up_limit;
  gdouble square_scale;
} graph_data;

typedef enum graph_draw_func_line_flag {
  OUT_OF_RESTRICTIONS_FLAG,
  NORMAL_FLAG,
  NAN_OR_INF_FLAG
} graph_draw_func_line_flag;

// Listeners
void graph_button_draw_listener(GtkWidget *button, GObject *context_object);
void graph_button_close_listener(GtkWidget *widget, gpointer window);

// Draw function
static void graph_draw_area_finction(GtkWidget *widget, cairo_t *cr,
                                     GObject *context_object);

// Help functions for drawing function
void graph_draw_axis(const graph_data graph_draw_data);
void graph_draw_axis_text(const graph_data graph_draw_data, gdouble value,
                          int rotate);
void graph_draw_function_line(const graph_data graph_draw_data,
                              char const *expression);

void graph_button_draw_listener(GtkWidget *button, GObject *context_object) {
  // Get object
  GtkDrawingArea *graph_draw_area =
      g_object_get_data(context_object, "graph_draw_area");

  // To call a draw function
  gtk_widget_queue_draw(GTK_WIDGET(graph_draw_area));
}

void graph_button_close_listener(GtkWidget *widget, gpointer window) {
  // Destroy window
  gtk_widget_destroy(GTK_WIDGET(window));
}

static void graph_draw_area_finction(GtkWidget *widget, cairo_t *cr,
                                     GObject *context_object) {
  // Get objects
  GtkLabel *graph_label_scale =
      g_object_get_data(context_object, "graph_label_scale");
  GtkSpinButton *graph_entry_max_x =
      g_object_get_data(context_object, "graph_entry_max_x");
  GtkSpinButton *graph_entry_min_x =
      g_object_get_data(context_object, "graph_entry_min_x");
  GtkSpinButton *graph_entry_max_y =
      g_object_get_data(context_object, "graph_entry_max_y");
  GtkSpinButton *graph_entry_min_y =
      g_object_get_data(context_object, "graph_entry_min_y");
  gchar const *expression = g_object_get_data(context_object, "expression");

  // Graph data set
  graph_data graph_draw_data = {};
  // Set cairo
  graph_draw_data.cr = cr;
  // Set background color and draw it
  cairo_set_source_rgb(graph_draw_data.cr, 1.0, 1.0, 1.0);
  cairo_paint(graph_draw_data.cr);

  // Get value from spins
  graph_draw_data.spin_x_max =
      gtk_spin_button_get_value(GTK_SPIN_BUTTON(graph_entry_max_x));
  graph_draw_data.spin_x_min =
      gtk_spin_button_get_value(GTK_SPIN_BUTTON(graph_entry_min_x));
  graph_draw_data.spin_y_max =
      gtk_spin_button_get_value(GTK_SPIN_BUTTON(graph_entry_max_y));
  graph_draw_data.spin_y_min =
      gtk_spin_button_get_value(GTK_SPIN_BUTTON(graph_entry_min_y));

  // Get limits
  graph_draw_data.right_limit =
      graph_draw_data.spin_x_min > graph_draw_data.spin_y_min
          ? graph_draw_data.spin_y_min
          : graph_draw_data.spin_x_min;
  graph_draw_data.left_limit =
      graph_draw_data.spin_x_max < graph_draw_data.spin_y_max
          ? graph_draw_data.spin_y_max
          : graph_draw_data.spin_x_max;
  graph_draw_data.low_limit =
      graph_draw_data.spin_y_max > graph_draw_data.spin_x_max
          ? -graph_draw_data.spin_y_max
          : -graph_draw_data.spin_x_max;
  graph_draw_data.up_limit =
      graph_draw_data.spin_y_min < graph_draw_data.spin_x_min
          ? -graph_draw_data.spin_y_min
          : -graph_draw_data.spin_x_min;

  // Get distance from left and right, up and low limits
  double x_distance = graph_draw_data.left_limit - graph_draw_data.right_limit;
  double y_distance = graph_draw_data.up_limit - graph_draw_data.low_limit;

  // Get middle of right and distance, low and distance
  double x_middle = (fabs(graph_draw_data.right_limit) / (x_distance)) *
                    GRAPH_DRAW_AREA_SIZE_SQUARE;
  double y_middle = (fabs(graph_draw_data.low_limit) / (y_distance)) *
                    GRAPH_DRAW_AREA_SIZE_SQUARE;

  // Set scale
  graph_draw_data.square_scale = (x_distance) / GRAPH_DRAW_AREA_SIZE_SQUARE;

  // Set scale to label text
  char buffer[GRAPH_LABEL_MAX_STRING_SIZE];
  snprintf(buffer, GRAPH_LABEL_MAX_STRING_SIZE, GRAPH_LABEL_SCALE_TEXT,
           GRAPH_DRAW_AREA_SIZE_SQUARE, x_distance);
  gtk_label_set_text(GTK_LABEL(graph_label_scale), buffer);

  // Move the drawing space to the center
  cairo_translate(graph_draw_data.cr, x_middle, y_middle);

  // Set scale of drawing area
  cairo_scale(graph_draw_data.cr,
              GRAPH_DRAW_UNIT_SCALE / graph_draw_data.square_scale,
              GRAPH_DRAW_UNIT_SCALE / graph_draw_data.square_scale);

  // Draw axis and lines
  graph_draw_axis(graph_draw_data);

  // Draw function line
  graph_draw_function_line(graph_draw_data, expression);
}

void graph_draw_axis(const graph_data graph_draw_data) {
  // Set step to darwing lines and print text
  gdouble step =
      GRAPH_DRAW_STEP_PAUSE * GRAPH_DRAW_STEP_GET(graph_draw_data.square_scale);
  int vector = -1;  // crutch for reverse graphic numbers

  // Set settings for drawing lines and printing text
  cairo_set_source_rgb(graph_draw_data.cr, 0.0, 0.0, 0.0);
  cairo_set_line_width(graph_draw_data.cr, graph_draw_data.square_scale /
                                               GRAPH_DARW_LINE_WIDTH_NUM);
  cairo_select_font_face(graph_draw_data.cr, "Arial", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(graph_draw_data.cr,
                      graph_draw_data.square_scale * GRAPH_DRAW_TEXT_SIZE);

  // I and IV bottom draw lines and print text
  for (gdouble i = 0; i < graph_draw_data.left_limit; i += step) {
    cairo_move_to(graph_draw_data.cr, i, graph_draw_data.low_limit);
    cairo_line_to(graph_draw_data.cr, i, graph_draw_data.up_limit);
    cairo_move_to(graph_draw_data.cr, i, graph_draw_data.up_limit);
    graph_draw_axis_text(graph_draw_data, i, GRAPH_DRAW_ROTATE_TEXT);
  }

  // II and III bottom draw lines and print text
  for (gdouble i = 0; i > graph_draw_data.right_limit; i -= step) {
    cairo_move_to(graph_draw_data.cr, i, graph_draw_data.low_limit);
    cairo_line_to(graph_draw_data.cr, i, graph_draw_data.up_limit);
    cairo_move_to(graph_draw_data.cr, i, graph_draw_data.up_limit);
    graph_draw_axis_text(graph_draw_data, i, GRAPH_DRAW_ROTATE_TEXT);
  }

  // III and IV left draw lines and print text
  for (gdouble i = 0; i < graph_draw_data.up_limit; i += step) {
    cairo_move_to(graph_draw_data.cr, graph_draw_data.right_limit, i);
    cairo_line_to(graph_draw_data.cr, graph_draw_data.left_limit, i);
    cairo_move_to(graph_draw_data.cr, graph_draw_data.right_limit, i);
    graph_draw_axis_text(graph_draw_data, vector * i,
                         GRAPH_DRAW_DONT_ROTATE_TEXT);
  }

  // I and II left draw lines and print text
  for (gdouble i = 0; i > graph_draw_data.low_limit; i -= step) {
    cairo_move_to(graph_draw_data.cr, graph_draw_data.right_limit, i);
    cairo_line_to(graph_draw_data.cr, graph_draw_data.left_limit, i);
    cairo_move_to(graph_draw_data.cr, graph_draw_data.right_limit, i);
    graph_draw_axis_text(graph_draw_data, vector * i,
                         GRAPH_DRAW_DONT_ROTATE_TEXT);
  }

  // End draw
  cairo_stroke(graph_draw_data.cr);

  // Set axis line width
  cairo_set_line_width(graph_draw_data.cr, graph_draw_data.square_scale /
                                               GRAPH_DARW_AXIS_WIDTH_NUM);
  // Move and draw axis
  cairo_move_to(graph_draw_data.cr, graph_draw_data.right_limit, 0);
  cairo_line_to(graph_draw_data.cr, graph_draw_data.left_limit, 0);
  cairo_move_to(graph_draw_data.cr, 0, graph_draw_data.low_limit);
  cairo_line_to(graph_draw_data.cr, 0, graph_draw_data.up_limit);

  // Set font size to X and Y
  cairo_set_font_size(graph_draw_data.cr, 15 * graph_draw_data.square_scale);

  // Move to end of X line and print text
  cairo_move_to(graph_draw_data.cr,
                graph_draw_data.left_limit - 15 * graph_draw_data.square_scale,
                0 + 15 * graph_draw_data.square_scale);
  cairo_show_text(graph_draw_data.cr, "X");

  // Move to end of Y line and print text
  cairo_move_to(graph_draw_data.cr, 0 + 5 * graph_draw_data.square_scale,
                graph_draw_data.low_limit + 15 * graph_draw_data.square_scale);
  cairo_show_text(graph_draw_data.cr, "Y");

  // End draw
  cairo_stroke(graph_draw_data.cr);
}

void graph_draw_axis_text(const graph_data graph_draw_data, gdouble value,
                          gint rotate) {
  char buffer[GRAPH_LABEL_MAX_STRING_SIZE];

  // Convert double value to text
  snprintf(buffer, GRAPH_LABEL_MAX_STRING_SIZE, "%.1f", value);

  // Check rotate (== 1)
  if (rotate) {
    // Rotate
    cairo_rotate(graph_draw_data.cr, -1);

    // Print
    cairo_show_text(graph_draw_data.cr, buffer);

    // Rotate set default
    cairo_rotate(graph_draw_data.cr, 1);
  } else {
    // Print
    cairo_show_text(graph_draw_data.cr, buffer);
  }
}

void graph_draw_function_line(const graph_data graph_draw_data,
                              char const *expression) {
  int func_result_flag = OUT_OF_RESTRICTIONS_FLAG;

  // Set color and line width
  cairo_set_source_rgb(graph_draw_data.cr, 1, 0, 0);
  cairo_set_line_width(graph_draw_data.cr, GRAPH_DARW_LINE_WIDTH_NUM *
                                               graph_draw_data.square_scale);

  // Set function line step
  gdouble step = graph_draw_data.square_scale;
  int vector = -1;  // crutch for reverse draw line

  // For cycle calculation f(x) to get y and draw it
  for (gdouble x = graph_draw_data.spin_x_min; x < graph_draw_data.spin_x_max;
       x += step) {
    double y = calculation(expression, &x);

    // If y out of y max
    if (y > graph_draw_data.spin_y_max) {
      // If before are ok
      if (func_result_flag == NORMAL_FLAG) {
        // Draw line to y max
        cairo_line_to(graph_draw_data.cr, x,
                      vector * graph_draw_data.spin_y_max);
      }

      // Go to y max
      cairo_move_to(graph_draw_data.cr, x, vector * graph_draw_data.spin_y_max);

      // Set out of restsrictions flag
      func_result_flag = OUT_OF_RESTRICTIONS_FLAG;

      // If y out of y min
    } else if ((y < graph_draw_data.spin_y_min)) {
      // If before are ok
      if (func_result_flag == NORMAL_FLAG) {
        // Draw line to y min
        cairo_line_to(graph_draw_data.cr, x,
                      vector * graph_draw_data.spin_y_min);
      }

      // Go to y min
      cairo_move_to(graph_draw_data.cr, x, vector * graph_draw_data.spin_y_min);

      // Set out of restsrictions flag
      func_result_flag = OUT_OF_RESTRICTIONS_FLAG;

      // If y is inf or nan
    } else if (isnan(y) || isinf(y)) {
      // Set nan or inf flag
      func_result_flag = NAN_OR_INF_FLAG;

      // If flag before < NAN flag
    } else if (func_result_flag < NAN_OR_INF_FLAG) {
      // Draw line to y
      cairo_line_to(graph_draw_data.cr, x, vector * y);

      // Set normal flag
      func_result_flag = NORMAL_FLAG;

    } else {
      // Go to y
      cairo_move_to(graph_draw_data.cr, x, vector * y);

      // Set normal flag
      func_result_flag = NORMAL_FLAG;
    }
  }

  // End draw
  cairo_stroke(graph_draw_data.cr);
}

void graph_window(char *input) {
  // Get builder from file
  GtkBuilder *builder = gtk_builder_new_from_file("gui/gui_graph_window.glade");

  // Make window from builder
  GtkWindow *graph_window =
      GTK_WINDOW(gtk_builder_get_object(builder, "graph_window"));

  // Connect objects from builder and add connects to listeners
  GtkLabel *graph_label_scale =
      GTK_LABEL(gtk_builder_get_object(builder, "graph_label_scale"));

  // Entry expression
  GtkEntry *graph_entry_expression =
      GTK_ENTRY(gtk_builder_get_object(builder, "graph_entry_expression"));
  gtk_entry_set_text(graph_entry_expression, input);

  // Spin buttons
  GtkSpinButton *graph_entry_max_x =
      GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "graph_entry_max_x"));
  GtkSpinButton *graph_entry_min_x =
      GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "graph_entry_min_x"));
  GtkSpinButton *graph_entry_max_y =
      GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "graph_entry_max_y"));
  GtkSpinButton *graph_entry_min_y =
      GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "graph_entry_min_y"));

  // Close button
  GtkButton *graph_button_close =
      GTK_BUTTON(gtk_builder_get_object(builder, "graph_button_close"));
  g_signal_connect(G_OBJECT(graph_button_close), "clicked",
                   G_CALLBACK(graph_button_close_listener),
                   G_OBJECT(graph_window));

  // Input expression var from entry
  static gchar expression[MAX_STRING_SIZE];
  snprintf(expression, MAX_STRING_SIZE, "%s", input);

  // Draw area
  GtkDrawingArea *graph_draw_area =
      GTK_DRAWING_AREA(gtk_builder_get_object(builder, "graph_draw_area"));
  gtk_widget_set_size_request(GTK_WIDGET(graph_draw_area),
                              GRAPH_DRAW_AREA_SIZE_SQUARE,
                              GRAPH_DRAW_AREA_SIZE_SQUARE);
  g_object_set_data(G_OBJECT(graph_draw_area), "graph_entry_max_x",
                    graph_entry_max_x);
  g_object_set_data(G_OBJECT(graph_draw_area), "graph_entry_min_x",
                    graph_entry_min_x);
  g_object_set_data(G_OBJECT(graph_draw_area), "graph_entry_max_y",
                    graph_entry_max_y);
  g_object_set_data(G_OBJECT(graph_draw_area), "graph_entry_min_y",
                    graph_entry_min_y);
  g_object_set_data(G_OBJECT(graph_draw_area), "graph_label_scale",
                    graph_label_scale);
  g_object_set_data(G_OBJECT(graph_draw_area), "expression", &expression);
  g_signal_connect(G_OBJECT(graph_draw_area), "draw",
                   G_CALLBACK(graph_draw_area_finction), graph_draw_area);

  // Draw button
  GtkButton *graph_button_draw =
      GTK_BUTTON(gtk_builder_get_object(builder, "graph_button_draw"));
  g_object_set_data(G_OBJECT(graph_button_draw), "graph_draw_area",
                    graph_draw_area);
  g_signal_connect(GTK_BUTTON(graph_button_draw), "clicked",
                   G_CALLBACK(graph_button_draw_listener), graph_button_draw);

  // Unref builde, set position and show graph window
  g_object_unref(builder);
  gtk_window_set_position(GTK_WINDOW(graph_window), GTK_WIN_POS_CENTER);
  gtk_widget_show_all(GTK_WIDGET(graph_window));
}
