#include <pebble.h>

Window *window;
TextLayer *date_layer;
TextLayer *time_layer;
Layer *window_layer;
GRect bounds;

GBitmap *moon_bitmap, *dayNight_bitmap;
BitmapLayer *moon_layer, *dayNight_layer;


// Called once per second
static void handle_second_tick(struct tm* tick_time, TimeUnits units_changed) {
  static char time_text[] = "00:00:00"; // Needs to be static because it's used by the system later.
  static char date_text[] = "0000-00-00";
  strftime(time_text, sizeof(time_text), "%T", tick_time);
  strftime(date_text, sizeof(date_text), "%F", tick_time);
  text_layer_set_text(time_layer, time_text);
  text_layer_set_text(date_layer,date_text);
}

void handle_init(void) {
	// Create a window and text layer
	window = window_create();
  window_layer = window_get_root_layer(window);
  window_set_background_color(window, GColorBlack);
  
  bounds = layer_get_frame(window_layer);
	date_layer = text_layer_create(GRect(11,100,129,180));
  time_layer = text_layer_create(GRect(0,130,150,180));
  
	
	// Set the text, font, and text alignment
	//text_layer_set_text(text_layer, "Hi, I'm a Pebble!");
	text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_color(date_layer,GColorWhite);
	text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_background_color(date_layer,GColorBlack);
  
  //text_layer_set_text(time_layer, "Text example description");
	text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS ));
  text_layer_set_text_color(time_layer,GColorWhite);
	text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_background_color(time_layer,GColorBlack);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
  
  moon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_Moon);
  moon_layer = bitmap_layer_create(GRect(1,0,30,30));
  bitmap_layer_set_bitmap(moon_layer, moon_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(moon_layer));
  
  // Ensures time is displayed immediately (will break if NULL tick event accessed).
  // (This is why it's a good idea to have a separate routine to do the update itself.)
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  handle_second_tick(current_time, SECOND_UNIT);

  tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);

	// Push the window
	window_stack_push(window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

void handle_deinit(void) {
	// Destroy the text layer
	text_layer_destroy(date_layer);
	text_layer_destroy(time_layer);
	// Destroy the window
	window_destroy(window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
