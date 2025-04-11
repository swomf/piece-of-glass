#include <cairo.h>
#include <gtk/gtk.h>

static gboolean supports_alpha = FALSE;

static void cb_screen_changed(GtkWidget *widget, GdkScreen *old_screen,
                              gpointer user_data) {
  GdkScreen *screen = gtk_widget_get_screen(widget);
  GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

  if (visual == NULL) {
    g_print("Your screen does not support alpha channels!\n");
    visual = gdk_screen_get_system_visual(screen);
    supports_alpha = FALSE;
  } else {
    g_print("Your screen supports alpha channels!\n");
    supports_alpha = TRUE;
  }

  gtk_widget_set_visual(widget, visual);
}

static gboolean cb_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.0);
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint(cr);

  return FALSE;
}

static gboolean clicked(GtkWidget *window, GdkEventButton *event,
                        gpointer user_data) {
  // Toggle window manager frames
  gtk_window_set_decorated(GTK_WINDOW(window),
                           !gtk_window_get_decorated(GTK_WINDOW(window)));

  return FALSE;
}

int main(int argc, char *argv[]) {
  GtkWidget *window;
  GtkWidget *fixed_container;
  GtkWidget *button;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
  gtk_window_set_title(GTK_WINDOW(window), "Alpha Demo");
  g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_set_app_paintable(window, TRUE);

  g_signal_connect(window, "draw", G_CALLBACK(cb_draw), NULL);
  g_signal_connect(window, "screen-changed", G_CALLBACK(cb_screen_changed),
                   NULL);

  gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
  gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
  g_signal_connect(window, "button-press-event", G_CALLBACK(clicked), NULL);

  fixed_container = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), fixed_container);

  button = gtk_button_new_with_label("button1");
  gtk_widget_set_size_request(button, 100, 100);
  gtk_fixed_put(GTK_FIXED(fixed_container), button, 150, 150);

  cb_screen_changed(window, NULL, NULL);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
