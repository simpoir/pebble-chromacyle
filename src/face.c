#include <pebble.h>
#include "battery.h"
#include "bt.h"

static Window* s_main_window = NULL;
static TextLayer* s_time_layer = NULL;
static GColor bg_color, bg_colorl, bg_colord;

GColor hsv_to_rgb(float h, float s, float v) {
  float hh, p, q, t, ff;
  int i;
  if (s <= 0.0f) {
    return GColorFromRGB(v, v, v);
  }
  hh = h;
  if (hh >= 360.0f) hh = 0.f;
  hh /= 60.f;
  i = hh;
  ff = hh-i;
  p = v*(1.f-s) * 255;
  q = v*(1.f-(s*ff)) * 255;
  t = v*(1.f-(s*(1.f-ff))) * 255;
  v *= 255;
  switch(i) {
    case 0:
      return GColorFromRGB(v, t, p);
    case 1:
      return GColorFromRGB(q, v, p);
    case 2:
      return GColorFromRGB(p, v, t);
    case 3:
      return GColorFromRGB(p, q, v);
    case 4:
      return GColorFromRGB(t, p, v);
    case 5:
    default:
      return GColorFromRGB(v, p, q);
  }
}

static void update_bg(struct tm* t) {
  int hue = t->tm_min*12%360;
  bg_color = hsv_to_rgb(hue, 1.f, 0.7f);
  bg_colorl = hsv_to_rgb(hue, 0.8f, 0.8f);
  bg_colord = hsv_to_rgb(hue, 1.f, 0.5f);
  layer_mark_dirty(window_get_root_layer(s_main_window));
}

static void update_time(struct tm* t, TimeUnits _u) {
  time_t temp = time(NULL);
  struct tm* tick_time = localtime(&temp);
  static char buffer[] = "00:00";
  strftime(
      buffer, sizeof(buffer),
      clock_is_24h_style()? "%H:%M": "%I:%M",
      tick_time);
  text_layer_set_text(s_time_layer, buffer);
  update_bg(tick_time);
}

static void draw_bg(Layer* layer, GContext* ctx) {
  GRect bounds = layer_get_bounds(layer);
  graphics_context_set_fill_color(ctx, bg_color);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);
}

static void main_window_load(Window *window) {
  bg_color = GColorBlack; // just be sure it's initialized
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
  layer_add_child(window_get_root_layer(s_main_window), battery_init());
  layer_add_child(window_get_root_layer(s_main_window), bluetooth_init());
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  battery_deinit();
  bluetooth_deinit();
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
