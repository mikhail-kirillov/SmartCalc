// Copyright [2023] <Burr Etienne>

#include "../gui/gui_windows.h"

#define MAIN_APPLICATION_ID ("ru.burretie.Calculator")

#define MAIN_ERROR_CALCULATION_MESSAGE \
  ("Неверный ввод или вычисление невозможно")
#define MAIN_PLUS_INF_CALCULATION_MESSAGE ("+Бесконечность")
#define MAIN_MINUS_INF_CALCULATION_MESSAGE ("-Бесконечность")

#define CREDIT_ANNUITET ("Аннуитетный")
#define CREDIT_DIFF ("Дифференцированный")

#define DIALOG_ERROR_TITLE ("Ошибка")
#define DIALOG_ERROR_BUTTON_TEXT ("OK")
#define DIALOG_ERROR_MESSAGE ("Ошибка ввода!")

// Credit type for GtkRadioButton
typedef enum credit_type { DIFF_TYPE, ANNUITET_TYPE } credit_type;

// Connection
static void on_activate(GtkApplication *app);

// Listeners

// Main window
void main_nums_and_operators_buttons_listener(GtkButton *button, gpointer data);
void main_function_button_listener(GtkButton *button, gpointer data);
void main_result_button_listener(GtkWidget *widget, GObject *context_object);
void main_clear_button_listener(GtkWidget *widget, gpointer data);
void main_delete_button_listener(GtkWidget *widget, gpointer data);
void main_graph_window_open_button_listener(GtkWidget *widget,
                                            GObject *context_object);

// Credit window
void credit_type_button_listener(GtkButton *button, GObject *context_object);
void credit_button_calculate_listener(GtkWidget *widget,
                                      GObject *context_object);

// Error window
void dialog_error_show(GtkWindow *parent, gchar *message);

void main_nums_and_operators_buttons_listener(GtkButton *button,
                                              gpointer data) {
  // Get text from button and entry
  const gchar *button_text = gtk_button_get_label(button);
  const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(data));

  // Alloc memory for entry with button texts
  gchar *tmp_text =
      g_malloc0_n(strlen(button_text) + strlen(entry_text) + 1, sizeof(gchar));

  // Two strings in one
  g_snprintf(tmp_text, strlen(button_text) + strlen(entry_text) + 1, "%s%s",
             entry_text, button_text);

  // Set new string to entry
  gtk_entry_set_text(GTK_ENTRY(data), tmp_text);

  // Free memory
  g_free(tmp_text);
}

void main_function_button_listener(GtkButton *button, gpointer data) {
  // Get text from button and entry
  const gchar *button_text = gtk_button_get_label(button);
  const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(data));

  // Alloc memory for entry with button texts and param
  gchar *tmp_text = g_malloc0_n(
      strlen(button_text) + strlen(entry_text) + 1 + 1, sizeof(gchar));

  // Two strings in one with param
  g_snprintf(tmp_text, strlen(button_text) + strlen(entry_text) + 1 + 1,
             "%s%s%s", entry_text, button_text, "(");

  // Set new string to entry
  gtk_entry_set_text(GTK_ENTRY(data), tmp_text);

  // Free memory
  g_free(tmp_text);
}

void main_result_button_listener(GtkWidget *widget, GObject *context_object) {
  // Get objects
  GtkLabel *main_label_error =
      g_object_get_data(context_object, "main_label_error");
  GtkEntry *main_entry_x = g_object_get_data(context_object, "main_entry_x");
  GtkEntry *main_entry_expression =
      g_object_get_data(context_object, "main_entry_expression");
  GtkWidget *main_window_tmp = g_object_get_data(context_object, "main_window");

  // Get text from entry
  const gchar *main_entry_x_text = gtk_entry_get_text(GTK_ENTRY(main_entry_x));

  // Alloc memory for entry text
  gchar *entry_x_text =
      g_malloc0_n(strlen(main_entry_x_text) + 1, sizeof(gchar));

  // Copy text to allocated memory
  g_snprintf(entry_x_text, strlen(main_entry_x_text) + 1, "%s",
             main_entry_x_text);

  // Get text from entry
  const gchar *main_entry_expression_text =
      gtk_entry_get_text(GTK_ENTRY(main_entry_expression));

  // Alloc memory for entry text
  gchar *entry_expression_text =
      g_malloc0_n(strlen(main_entry_expression_text) + 1, sizeof(gchar));

  // Copy text to allocated memory
  g_snprintf(entry_expression_text, strlen(main_entry_expression_text) + 1,
             "%s", main_entry_expression_text);

  // Check input
  int x_in_exp = 0;
  int x_is_ok = 0;
  int exp_is_ok = 0;
  input_validation(entry_expression_text, entry_x_text, &x_in_exp, &exp_is_ok,
                   &x_is_ok);

  if (exp_is_ok && x_is_ok) {
    char *string = calloc(sizeof(char), MAX_STRING_SIZE);
    double res = NAN;

    // If x not in exp calculate
    if (!x_in_exp) {
      res = calculation(entry_expression_text, NULL);
    } else {
      // Get x value
      double x = 0;
      setlocale(LC_NUMERIC, "C");
      sscanf(entry_x_text, "%lf", &x);

      res = calculation(entry_expression_text, &x);
    }

    // Check answer
    if (isnan(res)) {
      snprintf(string, MAX_STRING_SIZE, "%s", MAIN_ERROR_CALCULATION_MESSAGE);
    } else if (isinf(res)) {
      if (res > 0) {
        snprintf(string, MAX_STRING_SIZE, "%s",
                 MAIN_PLUS_INF_CALCULATION_MESSAGE);
      } else {
        snprintf(string, MAX_STRING_SIZE, "%s",
                 MAIN_MINUS_INF_CALCULATION_MESSAGE);
      }
    } else {
      snprintf(string, MAX_STRING_SIZE, "%lf", res);
    }

    // Set result text
    gtk_label_set_text(GTK_LABEL(main_label_error), string);

    // Free memory
    free(string);
  } else {
    // Error show
    dialog_error_show(GTK_WINDOW(main_window_tmp), DIALOG_ERROR_MESSAGE);
  }

  // Free memory
  g_free(entry_x_text);
  g_free(entry_expression_text);
}

void main_clear_button_listener(GtkWidget *widget, gpointer data) {
  // Clear entry text
  gtk_entry_set_text(GTK_ENTRY(data), "");
}

void main_delete_button_listener(GtkWidget *widget, gpointer data) {
  // Get text from entry
  const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(data));

  // Entry text len
  int entry_text_len = strlen(entry_text);

  // Check len
  if (entry_text_len > 0) {
    // Alloc memory for text
    gchar *tmp_text = g_malloc0_n(entry_text_len, sizeof(gchar));

    // Copy text to memory
    g_snprintf(tmp_text, entry_text_len, "%s", entry_text);

    // Add \0 to n-1 text in memory
    tmp_text[entry_text_len - 1] = '\0';

    // Set new text in entry
    gtk_entry_set_text(GTK_ENTRY(data), tmp_text);

    // Free memory
    g_free(tmp_text);
  }
}

void main_graph_window_open_button_listener(GtkWidget *widget,
                                            GObject *context_object) {
  // Get objects
  GtkEntry const *main_entry_expression =
      g_object_get_data(context_object, "main_entry_expression");
  GtkWidget *main_window_tmp = g_object_get_data(context_object, "main_window");

  // Get text from entry
  const gchar *entry_exp_text =
      gtk_entry_get_text(GTK_ENTRY(main_entry_expression));

  // Copy text to allocated memory
  gchar *exp_text = g_malloc0_n(strlen(entry_exp_text) + 1, sizeof(gchar));
  g_snprintf(exp_text, strlen(entry_exp_text) + 1, "%s", entry_exp_text);

  // Need this to use function
  int x_in_exp = 0;
  int x_is_ok = 0;
  // Check exp
  int exp_is_ok = 0;
  input_validation(exp_text, "", &x_in_exp, &exp_is_ok, &x_is_ok);

  // Check function from entry
  if (exp_is_ok) {
    // Show graph window
    graph_window(exp_text);
  } else {
    // Error show
    dialog_error_show(GTK_WINDOW(main_window_tmp), DIALOG_ERROR_MESSAGE);
  }
  // Free memory
  g_free(exp_text);
}

void credit_type_button_listener(GtkButton *button, GObject *context_object) {
  // Get ptr from object
  gint *credit_radio_button_type =
      g_object_get_data(context_object, "credit_radio_button_type");

  // Check text from button
  if (g_strcmp0(gtk_button_get_label(button), CREDIT_ANNUITET)) {
    // Set other type, because listener use two objects
    *credit_radio_button_type = DIFF_TYPE;
  } else if (g_strcmp0(gtk_button_get_label(button), CREDIT_DIFF)) {
    *credit_radio_button_type = ANNUITET_TYPE;
  }
}

void credit_button_calculate_listener(GtkWidget *widget,
                                      GObject *context_object) {
  // Get objects

  // Labels for results
  GtkLabel *credit_label_total_pay =
      g_object_get_data(context_object, "credit_label_total_pay");
  GtkLabel *credit_label_month_pay =
      g_object_get_data(context_object, "credit_label_month_pay");
  GtkLabel *credit_label_overpay =
      g_object_get_data(context_object, "credit_label_overpay");

  // Spin buttons for input
  GtkSpinButton *credit_entry_credit_term =
      g_object_get_data(context_object, "credit_entry_credit_term");
  GtkSpinButton *credit_entry_interest_rate =
      g_object_get_data(context_object, "credit_entry_interest_rate");
  GtkSpinButton *credit_entry_credit_sum =
      g_object_get_data(context_object, "credit_entry_credit_sum");

  // Static var from out with credit type now
  gint const *credit_radio_button_type =
      g_object_get_data(context_object, "credit_radio_button_type");

  // Get nums from objects
  int int_credit_entry_credit_term = gtk_spin_button_get_value_as_int(
      GTK_SPIN_BUTTON(credit_entry_credit_term));
  double double_credit_entry_interest_rate =
      gtk_spin_button_get_value(GTK_SPIN_BUTTON(credit_entry_interest_rate));
  double double_credit_entry_credit_sum =
      gtk_spin_button_get_value(GTK_SPIN_BUTTON(credit_entry_credit_sum));

  // Check credit type
  if (*credit_radio_button_type == ANNUITET_TYPE) {
    double month_pay = 0;
    double overpay = 0;
    double credit_pay_sum = 0;

    // Credit calculation
    annuitet_credit_calculation(double_credit_entry_credit_sum,
                                int_credit_entry_credit_term,
                                double_credit_entry_interest_rate, &month_pay,
                                &overpay, &credit_pay_sum);

    // Allocate memory
    char *string = calloc(sizeof(char), MAX_STRING_SIZE);

    // Set text to memory and set text from memory to label
    snprintf(string, MAX_STRING_SIZE, "%.2lf", month_pay);
    gtk_label_set_text(credit_label_month_pay, string);

    // Clean string
    memset(string, ' ', MAX_STRING_SIZE);
    // Set text to memory and set text from memory to label
    snprintf(string, MAX_STRING_SIZE, "%.2lf", overpay);
    gtk_label_set_text(credit_label_overpay, string);

    // Clean string
    memset(string, ' ', MAX_STRING_SIZE);
    // Set text to memory and set text from memory to label
    snprintf(string, MAX_STRING_SIZE, "%.2lf", credit_pay_sum);
    gtk_label_set_text(credit_label_total_pay, string);

    // Free memory
    free(string);
  } else if (*credit_radio_button_type == DIFF_TYPE) {
    double max_month_pay = 0;
    double min_month_pay = 0;
    double overpay = 0;
    double credit_pay_sum = 0;

    // Credit calculation
    diff_credit_calculation(double_credit_entry_credit_sum,
                            int_credit_entry_credit_term,
                            double_credit_entry_interest_rate, &max_month_pay,
                            &min_month_pay, &overpay, &credit_pay_sum);
    // Allocate memory
    char *string = calloc(sizeof(char), MAX_STRING_SIZE);

    // Set text to memory and set text from memory to label
    snprintf(string, MAX_STRING_SIZE, "%.2lf...%.2lf", max_month_pay,
             min_month_pay);
    gtk_label_set_text(credit_label_month_pay, string);

    // Clean string
    memset(string, ' ', MAX_STRING_SIZE);
    // Set text to memory and set text from memory to label
    snprintf(string, MAX_STRING_SIZE, "%.2lf", overpay);
    gtk_label_set_text(credit_label_overpay, string);

    // Clean string
    memset(string, ' ', MAX_STRING_SIZE);
    // Set text to memory and set text from memory to label
    snprintf(string, MAX_STRING_SIZE, "%.2lf", credit_pay_sum);
    gtk_label_set_text(credit_label_total_pay, string);

    // Free memory
    free(string);
  }
}

void dialog_error_show(GtkWindow *parent, gchar *message) {
  // Create dialog
  GtkWidget *dialog = gtk_dialog_new_with_buttons(
      DIALOG_ERROR_TITLE, parent, GTK_DIALOG_DESTROY_WITH_PARENT,
      DIALOG_ERROR_BUTTON_TEXT, GTK_RESPONSE_NONE, NULL);
  GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
  GtkWidget *label = gtk_label_new(message);

  // Set connect
  g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy),
                           dialog);

  // Set label to dialog
  gtk_container_add(GTK_CONTAINER(content_area), label);
  gtk_widget_show_all(dialog);
}

static void on_activate(GtkApplication *app) {
  // Get builder from file
  GtkBuilder *builder = gtk_builder_new_from_file("gui/gui_main_window.glade");

  // Make window from builder
  GtkWidget *main_window_var =
      GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
  gtk_window_set_application(GTK_WINDOW(main_window_var), GTK_APPLICATION(app));

  // Main

  // Connect objects from builder and add connects to listeners

  // GtkLabel for errors
  GtkLabel *main_label_error =
      GTK_LABEL(gtk_builder_get_object(builder, "main_label_error"));

  // GtkEntry for x input
  GtkEntry *main_entry_x =
      GTK_ENTRY(gtk_builder_get_object(builder, "main_entry_x"));

  // GtkEntry for expression input
  GtkEntry *main_entry_expression =
      GTK_ENTRY(gtk_builder_get_object(builder, "main_entry_expression"));

  // Function buttons
  GtkButton *main_button_log =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_log"));
  g_signal_connect(G_OBJECT(main_button_log), "clicked",
                   G_CALLBACK(main_function_button_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_ln =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_ln"));
  g_signal_connect(G_OBJECT(main_button_ln), "clicked",
                   G_CALLBACK(main_function_button_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_cos =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_cos"));
  g_signal_connect(G_OBJECT(main_button_cos), "clicked",
                   G_CALLBACK(main_function_button_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_sin =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_sin"));
  g_signal_connect(G_OBJECT(main_button_sin), "clicked",
                   G_CALLBACK(main_function_button_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_tan =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_tan"));
  g_signal_connect(G_OBJECT(main_button_tan), "clicked",
                   G_CALLBACK(main_function_button_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_acos =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_acos"));
  g_signal_connect(G_OBJECT(main_button_acos), "clicked",
                   G_CALLBACK(main_function_button_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_asin =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_asin"));
  g_signal_connect(G_OBJECT(main_button_asin), "clicked",
                   G_CALLBACK(main_function_button_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_atan =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_atan"));
  g_signal_connect(G_OBJECT(main_button_atan), "clicked",
                   G_CALLBACK(main_function_button_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_sqrt =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_sqrt"));
  g_signal_connect(G_OBJECT(main_button_sqrt), "clicked",
                   G_CALLBACK(main_function_button_listener),
                   GTK_ENTRY(main_entry_expression));

  // Number and x buttons
  GtkButton *main_button_x =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_x"));
  g_signal_connect(G_OBJECT(main_button_x), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_one =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_one"));
  g_signal_connect(G_OBJECT(main_button_one), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_two =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_two"));
  g_signal_connect(G_OBJECT(main_button_two), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_three =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_three"));
  g_signal_connect(G_OBJECT(main_button_three), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_four =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_four"));
  g_signal_connect(G_OBJECT(main_button_four), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_five =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_five"));
  g_signal_connect(G_OBJECT(main_button_five), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_six =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_six"));
  g_signal_connect(G_OBJECT(main_button_six), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_seven =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_seven"));
  g_signal_connect(G_OBJECT(main_button_seven), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_eight =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_eight"));
  g_signal_connect(G_OBJECT(main_button_eight), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_nine =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_nine"));
  g_signal_connect(G_OBJECT(main_button_nine), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_zero =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_zero"));
  g_signal_connect(G_OBJECT(main_button_zero), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));

  // Operators buttons
  GtkButton *main_button_mod =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_mod"));
  g_signal_connect(G_OBJECT(main_button_mod), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_pow =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_pow"));
  g_signal_connect(G_OBJECT(main_button_pow), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_mult =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_mult"));
  g_signal_connect(G_OBJECT(main_button_mult), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_div =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_div"));
  g_signal_connect(G_OBJECT(main_button_div), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_point =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_point"));
  g_signal_connect(G_OBJECT(main_button_point), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_left_bracket =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_left_bracket"));
  g_signal_connect(G_OBJECT(main_button_left_bracket), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_right_bracket =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_right_bracket"));
  g_signal_connect(G_OBJECT(main_button_right_bracket), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_minus =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_minus"));
  g_signal_connect(G_OBJECT(main_button_minus), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));
  GtkButton *main_button_plus =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_plus"));
  g_signal_connect(G_OBJECT(main_button_plus), "clicked",
                   G_CALLBACK(main_nums_and_operators_buttons_listener),
                   GTK_ENTRY(main_entry_expression));

  // Expression delete button
  GtkButton *main_button_expression_delete = GTK_BUTTON(
      gtk_builder_get_object(builder, "main_button_expression_delete"));
  g_signal_connect(G_OBJECT(main_button_expression_delete), "clicked",
                   G_CALLBACK(main_delete_button_listener),
                   GTK_ENTRY(main_entry_expression));

  // Expression clear button
  GtkButton *main_button_expression_clear = GTK_BUTTON(
      gtk_builder_get_object(builder, "main_button_expression_clear"));
  g_signal_connect(G_OBJECT(main_button_expression_clear), "clicked",
                   G_CALLBACK(main_clear_button_listener),
                   GTK_ENTRY(main_entry_expression));

  // Equals button
  GtkButton *main_button_equals =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_equals"));
  // Add data to object to listener
  g_object_set_data(G_OBJECT(main_button_equals), "main_entry_x", main_entry_x);
  g_object_set_data(G_OBJECT(main_button_equals), "main_entry_expression",
                    main_entry_expression);
  g_object_set_data(G_OBJECT(main_button_equals), "main_window",
                    main_window_var);
  g_object_set_data(G_OBJECT(main_button_equals), "main_label_error",
                    main_label_error);
  g_signal_connect(GTK_BUTTON(main_button_equals), "clicked",
                   G_CALLBACK(main_result_button_listener), main_button_equals);

  // X clear button
  GtkButton *main_button_x_clear =
      GTK_BUTTON(gtk_builder_get_object(builder, "main_button_x_clear"));
  g_signal_connect(G_OBJECT(main_button_x_clear), "clicked",
                   G_CALLBACK(main_clear_button_listener),
                   GTK_ENTRY(main_entry_x));

  // Graph window button open
  GtkButton *main_button_graph_window_open = GTK_BUTTON(
      gtk_builder_get_object(builder, "main_button_graph_window_open"));
  // Add data to object to listener
  g_object_set_data(G_OBJECT(main_button_graph_window_open),
                    "main_entry_expression", main_entry_expression);
  g_object_set_data(G_OBJECT(main_button_graph_window_open), "main_window",
                    main_window_var);
  g_signal_connect(GTK_BUTTON(main_button_graph_window_open), "clicked",
                   G_CALLBACK(main_graph_window_open_button_listener),
                   main_button_graph_window_open);

  // Credit

  // Connect objects from builder and add connects to listeners

  // Labels for result
  GtkLabel *credit_label_month_pay =
      GTK_LABEL(gtk_builder_get_object(builder, "credit_label_month_pay"));
  GtkLabel *credit_label_overpay =
      GTK_LABEL(gtk_builder_get_object(builder, "credit_label_overpay"));
  GtkLabel *credit_label_total_pay =
      GTK_LABEL(gtk_builder_get_object(builder, "credit_label_total_pay"));

  // Spin buttons for input
  GtkSpinButton *credit_entry_credit_sum = GTK_SPIN_BUTTON(
      gtk_builder_get_object(builder, "credit_entry_credit_sum"));
  GtkSpinButton *credit_entry_credit_term = GTK_SPIN_BUTTON(
      gtk_builder_get_object(builder, "credit_entry_credit_term"));
  GtkSpinButton *credit_entry_interest_rate = GTK_SPIN_BUTTON(
      gtk_builder_get_object(builder, "credit_entry_interest_rate"));

  // Radio buttons for input
  static gint credit_radio_button_type = ANNUITET_TYPE;
  GtkRadioButton *credit_radio_button_anuity = GTK_RADIO_BUTTON(
      gtk_builder_get_object(builder, "credit_radio_button_anuity"));
  // Add data to object to listener
  g_object_set_data(G_OBJECT(credit_radio_button_anuity),
                    "credit_radio_button_type", &credit_radio_button_type);
  g_signal_connect(G_OBJECT(credit_radio_button_anuity), "clicked",
                   G_CALLBACK(credit_type_button_listener),
                   credit_radio_button_anuity);
  GtkRadioButton *credit_radio_button_diff = GTK_RADIO_BUTTON(
      gtk_builder_get_object(builder, "credit_radio_button_diff"));
  // Add data to object to listener
  g_object_set_data(G_OBJECT(credit_radio_button_diff),
                    "credit_radio_button_type", &credit_radio_button_type);
  g_signal_connect(G_OBJECT(credit_radio_button_diff), "clicked",
                   G_CALLBACK(credit_type_button_listener),
                   credit_radio_button_diff);

  // Calculate button
  GtkButton *credit_button_calculate =
      GTK_BUTTON(gtk_builder_get_object(builder, "credit_button_calculate"));
  // Add data to object to listener
  g_object_set_data(G_OBJECT(credit_button_calculate), "credit_label_total_pay",
                    credit_label_total_pay);
  g_object_set_data(G_OBJECT(credit_button_calculate), "credit_label_month_pay",
                    credit_label_month_pay);
  g_object_set_data(G_OBJECT(credit_button_calculate), "credit_label_overpay",
                    credit_label_overpay);
  g_object_set_data(G_OBJECT(credit_button_calculate),
                    "credit_entry_credit_term", credit_entry_credit_term);
  g_object_set_data(G_OBJECT(credit_button_calculate),
                    "credit_entry_interest_rate", credit_entry_interest_rate);
  g_object_set_data(G_OBJECT(credit_button_calculate),
                    "credit_entry_credit_sum", credit_entry_credit_sum);
  g_object_set_data(G_OBJECT(credit_button_calculate),
                    "credit_radio_button_type", &credit_radio_button_type);
  g_signal_connect(GTK_BUTTON(credit_button_calculate), "clicked",
                   G_CALLBACK(credit_button_calculate_listener),
                   credit_button_calculate);

  // Unref builder and present window
  g_object_unref(builder);
  gtk_window_present(GTK_WINDOW(main_window_var));
}

int main_window(int argc, char **argv) {
  // App make and connect
  GtkApplication *app =
      gtk_application_new(MAIN_APPLICATION_ID, G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  return g_application_run(G_APPLICATION(app), argc, argv);
}
