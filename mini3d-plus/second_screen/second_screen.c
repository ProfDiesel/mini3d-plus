#include <second_screen.h>

typedef struct _second_screen {
	GtkWidget *window;
	GtkWidget *drawing_area;

	int stride;
	uint8_t *image_data;

	render_callback_t *render_callback;
} second_screen;

static second_screen *SECOND_SCREEN_INSTANCE;

static gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data)
{
	second_screen *data_ = (second_screen*)data;
	if(data_->render_callback)
	{
		(*data_->render_callback)(data_->image_data, data_->stride);
		cairo_surface_t *image = cairo_image_surface_create_for_data(data_->image_data, CAIRO_FORMAT_A8, 400, 240, data_->stride);
		cairo_set_source_surface(cr, image, 0, 0);
		cairo_paint(cr);
		cairo_surface_destroy(image);
	}
	return TRUE;
}

second_screen *second_screen_new(render_callback_t *render_callback)
{
	second_screen *result = g_new(second_screen, 1);

	result->window = gtk_window_new(GTK_WINDOW_POPUP);
	result->drawing_area = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(result->window), result->drawing_area);
	gtk_widget_set_size_request(result->drawing_area, 400, 240);

	result->stride = cairo_format_stride_for_width(CAIRO_FORMAT_A8, 400);
	result->image_data = g_new(uint8_t, result->stride * 240);
	g_signal_connect(G_OBJECT(result->drawing_area), "draw", G_CALLBACK(draw_callback), result);

	result->render_callback = render_callback;

	return result;
}

void second_screen_init(render_callback_t render_callback)
{
  second_screen *result = second_screen_new(render_callback);
	gtk_widget_show_all(result->window);
	SECOND_SCREEN_INSTANCE = result;
}

void second_screen_invalidate()
{
	second_screen *screen = SECOND_SCREEN_INSTANCE;
	gtk_widget_queue_draw(screen->drawing_area);
}
