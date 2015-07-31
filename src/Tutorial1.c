#include <pebble.h>

static Window* s_main_window;
static TextLayer* s_time_layer;
static GPath* s_bg_rect_ptr;
static GPath* s_battery_path_ptr;

/*
void draw_battery(GRect position) {
  BatteryChargeState state = battery_state_service_peek();

  GColor color = GColorWhite;
  if (state.charge_percent <= 10) {
    color = GColorRed;
  }
  if (state.is_charging) {
    color = GColorGreen;
  }
}
*/


static void update_time(struct tm* _t, TimeUnits _u) {
  time_t temp = time(NULL);
  struct tm* tick_time = localtime(&temp);
  static char buffer[] = "00:00";
  strftime(
      buffer, sizeof(buffer),
      clock_is_24h_style()? "%H:%M": "%I:%M",
      tick_time);
  text_layer_set_text(s_time_layer, buffer);

  //draw_battery(GRect(5, 5, 25, 25));
}

void draw_bg(Layer* layer, GContext* ctx) {
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
}

static void main_window_load(Window *window) {
  layer_set_update_proc(window_get_root_layer(window), draw_bg);

  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");

  text_layer_set_font(
      s_time_layer,
      fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(
      window_get_root_layer(window),
      text_layer_get_layer(s_time_layer));

  tick_timer_service_subscribe(MINUTE_UNIT, update_time);
  update_time(NULL, 0);
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
}

static void init() {
  s_main_window = window_create();

  window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = main_window_load,
      .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}


int main() {
  init();
  app_event_loop();
  deinit();
}
