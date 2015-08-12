#include "battery.h"

static Layer* s_battery_layer = NULL;
static GPath* s_battery_path = NULL;
static GPathInfo BATTERY_PATH_INFO = {
  .num_points = 8,
  .points = (GPoint[]){{0, 0}, {10, 0}, {10, 2}, {11, 2}, {11, 5}, {10, 5}, {10, 7}, {0, 7}}
};

static void draw_battery(Layer* layer, GContext* ctx) {
  BatteryChargeState state = battery_state_service_peek();

  GColor color;
  if (state.is_charging) {
    color = GColorGreen;
  } else if (state.charge_percent <= 15) {
    color = GColorRed;
  } else {
    color = GColorWhite;
  }
  graphics_context_set_stroke_color(ctx, color);
  graphics_context_set_fill_color(ctx, color);
  gpath_draw_outline(ctx, s_battery_path);
  short lvl = state.charge_percent/10;
  graphics_fill_rect(ctx, GRect(0, 0, lvl, 7), 0, GCornerNone);
}


Layer* battery_init() {
  s_battery_layer = layer_create(GRect(5, 5, 20, 20));
  layer_set_update_proc(s_battery_layer, draw_battery);

  s_battery_path = gpath_create(&BATTERY_PATH_INFO);

  return s_battery_layer;
}

void battery_deinit() {
  gpath_destroy(s_battery_path);
  layer_destroy(s_battery_layer);
}

