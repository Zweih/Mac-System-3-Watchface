#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;
static TextLayer *date_layer;
static GBitmap *s_bitmap;
static BitmapLayer *s_bitmap_layer;


void handle_timechanges(struct tm *tick_time, TimeUnits units_changed){
	static char time_buffer[10];
	static char date_buffer[10];
	strftime(time_buffer, sizeof(time_buffer), "%H:%M", tick_time);
	text_layer_set_text(s_text_layer, time_buffer);
	
	strftime(date_buffer, sizeof(date_buffer), "%b %e", tick_time);
	text_layer_set_text(date_layer, date_buffer);
}

static void init(void) {
	//load the font
	
	
	
	// Create a window and get information about the window
	s_window = window_create();
	Layer *window_layer = window_get_root_layer(s_window);
	GRect bounds = layer_get_bounds(window_layer);
		//load image
	s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BACKG_IMG);
	
	
	//create the image
	s_bitmap_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
	bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
	
	// Create a text layer and set the text
	s_text_layer = text_layer_create(bounds);
  
  // Set the font and text alignment
	GFont Chicago = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_Font_Chicago_34));
	s_text_layer = text_layer_create(GRect(3, 61, 121, 41));
	text_layer_set_font(s_text_layer, Chicago);
	text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
	
	date_layer = text_layer_create(GRect(61, 46, 78, 13));
	text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
	
	tick_timer_service_subscribe(MINUTE_UNIT, handle_timechanges);

	// Add the text layer to the window
	layer_add_child(window_get_root_layer(s_window), bitmap_layer_get_layer(s_bitmap_layer));
	layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_text_layer));
	
	layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(date_layer));
	
	
  // Enable text flow and paging on the text layer, with a slight inset of 10, for round screens
  text_layer_enable_screen_text_flow_and_paging(s_text_layer, 10);

	// Push the window, setting the window animation to 'true'
	window_stack_push(s_window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

static void deinit(void) {
	// Destroy the text layer
	text_layer_destroy(s_text_layer);
	text_layer_destroy(date_layer);
	gbitmap_destroy(s_bitmap);
	bitmap_layer_destroy(s_bitmap_layer);
	
	// Destroy the window
	window_destroy(s_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
