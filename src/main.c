/*
 * Resistor Colour Reference
 */

#include "pebble.h"

static Window *s_main_window;
static Layer *s_image_layer;
static GBitmap *s_image;

static void layer_update_callback(Layer *layer, GContext* ctx) {
#ifdef PBL_PLATFORM_BASALT
  GSize image_size = gbitmap_get_bounds(s_image).size;
#else 
  GSize image_size = s_image->bounds.size;
#endif

  graphics_draw_bitmap_in_rect(ctx, s_image, GRect(0, 0, image_size.w, image_size.h));
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_frame(window_layer);

  s_image_layer = layer_create(bounds);
  layer_set_update_proc(s_image_layer, layer_update_callback);
  layer_add_child(window_layer, s_image_layer);

  s_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RESISTOR_REF);
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(s_image);
  layer_destroy(s_image_layer);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_set_fullscreen(s_main_window, true);
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

