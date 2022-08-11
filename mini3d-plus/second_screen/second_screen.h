#if TARGET_SIMULATOR
#include <gtk/gtk.h>
#endif

typedef void render_callback_t(uint8_t *data, int stride);

void second_screen_init(render_callback_t *render_callback);
void second_screen_invalidate();

