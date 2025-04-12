#include <gtk/gtk.h>

static GtkGesture *rotate = NULL;
static GtkGesture *zoom = NULL;

static cairo_matrix_t matrix;
static cairo_pattern_t *pat;
static gdouble angle, scale, prev_angle;
static gdouble x_center, y_center;

gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data) {
  gtk_main_quit();
  return FALSE;
}

static void rotation_angle_changed(GtkGestureRotate *gesture, gdouble angle,
                                   gdouble delta, GtkWidget *widget) {
  gtk_widget_queue_draw(widget);
}

static void zoom_scale_changed(GtkGestureZoom *gesture, gdouble scale,
                               GtkWidget *widget) {
  gtk_widget_queue_draw(widget);
}

static gboolean drawing_area_draw(GtkWidget *widget, cairo_t *cr) {
  GtkAllocation allocation;

  gtk_widget_get_allocation(widget, &allocation);

  if (gtk_gesture_is_recognized(rotate) || gtk_gesture_is_recognized(zoom)) {
    gdouble temp;

    gtk_gesture_get_bounding_box_center(GTK_GESTURE(zoom), &x_center,
                                        &y_center);

    cairo_get_matrix(cr, &matrix);
    cairo_matrix_translate(&matrix, x_center, y_center);

    cairo_save(cr);

    temp = prev_angle;
    prev_angle = angle;
    angle =
        temp + gtk_gesture_rotate_get_angle_delta(GTK_GESTURE_ROTATE(rotate));
    cairo_matrix_rotate(&matrix, angle);

    scale = gtk_gesture_zoom_get_scale_delta(GTK_GESTURE_ZOOM(zoom));
    cairo_matrix_scale(&matrix, scale, scale);

    g_print("%f %f %f %f %f %f (%f+%f)\n", matrix.x0, matrix.xx, matrix.xy,
            matrix.y0, matrix.yx, matrix.yy, angle, prev_angle);
    cairo_set_matrix(cr, &matrix);
    cairo_rectangle(cr, -100, -100, 400, 200);

    pat = cairo_pattern_create_linear(-100, 0, 200, 0);
    cairo_pattern_add_color_stop_rgb(pat, 0, 0, 0, 1);
    cairo_pattern_add_color_stop_rgb(pat, 1, 1, 0, 0);
    cairo_set_source(cr, pat);
    cairo_fill(cr);

    cairo_restore(cr);

    cairo_pattern_destroy(pat);
  }
  return TRUE;
}

int app(void) {
  static GtkWidget *window = NULL;
  GtkWidget *drawing_area;
  GtkGesture *gesture;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "piece-of-glass");

  /* Suppress default theming of window bg.
   * Docs: "The application is then entirely responsible
   *        for drawing the widget background."
   * ...so we claim responsibility then do nothing LOL
   * */
  gtk_widget_set_app_paintable(window, TRUE);

  drawing_area = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER(window), drawing_area);
  gtk_widget_add_events(drawing_area, GDK_POINTER_MOTION_MASK | GDK_TOUCH_MASK |
                                          GDK_TOUCHPAD_GESTURE_MASK);

  g_signal_connect(drawing_area, "draw", G_CALLBACK(drawing_area_draw), NULL);

  /* rotate */
  rotate = gesture = gtk_gesture_rotate_new(drawing_area);
  g_signal_connect(gesture, "angle-changed", G_CALLBACK(rotation_angle_changed),
                   drawing_area);
  gtk_event_controller_set_propagation_phase(GTK_EVENT_CONTROLLER(gesture),
                                             GTK_PHASE_BUBBLE);
  g_object_weak_ref(G_OBJECT(drawing_area), (GWeakNotify)g_object_unref,
                    gesture);

  /* zoom */
  zoom = gesture = gtk_gesture_zoom_new(drawing_area);
  g_signal_connect(gesture, "scale-changed", G_CALLBACK(zoom_scale_changed),
                   drawing_area);
  gtk_event_controller_set_propagation_phase(GTK_EVENT_CONTROLLER(gesture),
                                             GTK_PHASE_BUBBLE);
  g_object_weak_ref(G_OBJECT(drawing_area), (GWeakNotify)g_object_unref,
                    gesture);

  g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);

  gtk_widget_show_all(window);

  return 0;
}

int main(int argc, char **argv) {
  gtk_init(&argc, &argv);

  app();

  gtk_main();

  return 0;
}
