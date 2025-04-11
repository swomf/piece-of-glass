#include <gtk/gtk.h>

gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
  gtk_main_quit();
  return FALSE;
}

int main(int argc, char **argv) {
  GtkWidget *window;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "piece-of-glass");

  /* Suppress default theming of window bg.
   * Docs: "The application is then entirely responsible
   *        for drawing the widget background."
   * ...so we claim responsibility then do nothing LOL
   * */
  gtk_widget_set_app_paintable(window, TRUE);

  g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
