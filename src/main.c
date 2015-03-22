/*
 * Resistor Colour Reference
 */

#include "pebble.h"

static Window *s_main_window;
static ScrollLayer *s_scroll_layer;
static BitmapLayer *s_image_layer;
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
  s_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_RESISTOR_REF);
#ifdef PBL_PLATFORM_BASALT
  GRect image_size = gbitmap_get_bounds(s_image);
#else 
  GRect image_size = s_image->bounds;
#endif  
  s_image_layer = bitmap_layer_create(image_size);
  
  
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_frame(window_layer);
  layer_set_update_proc(bitmap_layer_get_layer(s_image_layer), layer_update_callback);
  
  s_scroll_layer = scroll_layer_create(bounds);
  scroll_layer_set_click_config_onto_window(s_scroll_layer, window);
  scroll_layer_set_content_size(s_scroll_layer, GSize(bounds.size.w, image_size.size.h));
  scroll_layer_add_child(s_scroll_layer, bitmap_layer_get_layer(s_image_layer));
  
  layer_add_child(window_layer, scroll_layer_get_layer(s_scroll_layer));
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(s_image);
  bitmap_layer_destroy(s_image_layer);
  scroll_layer_destroy(s_scroll_layer);
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

