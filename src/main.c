#include <pebble.h>
#include <ctype.h>
  
static Window *s_main_window;
static TextLayer *s_hour_background_layer_1, *s_hour_background_layer_2, *s_hour_background_layer_3;
static TextLayer *s_hour_layer;

static GFont s_time_font;

static GBitmap *s_background_bitmap;
static BitmapLayer *s_background_layer;

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char time[] = "0000";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 2h hour format
    strftime(time, sizeof(time), "%H%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(time, sizeof(time), "%I%M", tick_time);
  }
  
  // Display this hour on the TextLayer
  text_layer_set_text(s_hour_layer, time);
  
  static char text_one[] = "0000";
  snprintf(text_one, sizeof(text_one), "%u", rand() % 9000 + 1000);
  text_layer_set_text(s_hour_background_layer_1, text_one);
  
  static char text_two[] = "0000";
  snprintf(text_two, sizeof(text_two), "%u", rand() % 9000 + 1000);
  text_layer_set_text(s_hour_background_layer_2, text_two);
  
  static char text_three[] = "0000";
  snprintf(text_three, sizeof(text_three), "%u", rand() % 9000 + 1000);
  text_layer_set_text(s_hour_background_layer_3, text_three);
  
}

static void main_window_load(Window *window) {
  //Create BitmapLayer
  s_background_layer = bitmap_layer_create(GRect(0, 76, 144, 48));
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  bitmap_layer_set_background_color(s_background_layer, GColorClear);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_background_layer));
  
  //Create Hour Background TextLayer 1
  s_hour_background_layer_1 = text_layer_create(GRect(0, -12, 144, 50));
  text_layer_set_background_color(s_hour_background_layer_1, GColorClear);
  text_layer_set_text_color(s_hour_background_layer_1, GColorWhite);
  
  //Create Hour Background TextLayer 2
  s_hour_background_layer_2 = text_layer_create(GRect(0, 20, 144, 50));
  text_layer_set_background_color(s_hour_background_layer_2, GColorClear);
  text_layer_set_text_color(s_hour_background_layer_2, GColorWhite);
  
  //Create Hour Background TextLayer 3
  s_hour_background_layer_3 = text_layer_create(GRect(0, 108, 144, 50));
  text_layer_set_background_color(s_hour_background_layer_3, GColorClear);
  text_layer_set_text_color(s_hour_background_layer_3, GColorWhite);
  
  //Create Hour TextLayer
  s_hour_layer = text_layer_create(GRect(0, 64, 144, 50));
  text_layer_set_background_color(s_hour_layer, GColorClear);
  text_layer_set_text_color(s_hour_layer, GColorBlack);
  
  //Create GFonts
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_INVERSIONZ_50));

  //Apply to Hour Background TextLayer
  text_layer_set_font(s_hour_background_layer_1, s_time_font);
  text_layer_set_font(s_hour_background_layer_2, s_time_font);
  text_layer_set_font(s_hour_background_layer_3, s_time_font);
  text_layer_set_text_alignment(s_hour_background_layer_1, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_hour_background_layer_2, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_hour_background_layer_3, GTextAlignmentCenter);
  
  //Apply to Hour TextLayer
  text_layer_set_font(s_hour_layer, s_time_font);
  text_layer_set_text_alignment(s_hour_layer, GTextAlignmentCenter);
  
  // Add them as children layers to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_background_layer_1));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_background_layer_2));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_background_layer_3));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_hour_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  //Unload GFonts
  fonts_unload_custom_font(s_time_font);

  //Destroy BitmapLayer
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);

  // Destroy Hour TextLayer
  text_layer_destroy(s_hour_background_layer_1);
  text_layer_destroy(s_hour_background_layer_2);
  text_layer_destroy(s_hour_background_layer_3);
  text_layer_destroy(s_hour_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  srand(time(NULL));
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}