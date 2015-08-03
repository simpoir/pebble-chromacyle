#include "bt.h"

static GPath* s_bt_path = NULL;
static Layer* s_bt_layer = NULL;
static GPathInfo BT_PATH_INFO = {
  .num_points = 6,
  .points = (GPoint[]){{0, 2}, {4, 6}, {2, 8}, {2, 0}, {4, 2}, {0, 6}}
};

static void draw_bt(Layer* layer, GContext* ctx) {
  if (bluetooth_connection_service_peek()) {
    graphics_context_set_stroke_color(ctx, GColorWhite);
    gpath_draw_outline_open(ctx, s_bt_path);
  }
}

static void on_bluetooth(bool connected) {
  layer_mark_dirty(s_bt_layer);
}

Layer* bluetooth_init() {
  s_bt_layer = layer_create(GRect(25, 4, 20, 20));
  layer_set_update_proc(s_bt_layer, draw_bt);

  s_bt_path = gpath_create(&BT_PATH_INFO);

  bluetooth_connection_service_subscribe(on_bluetooth);
  return s_bt_layer;
}

void bluetooth_deinit() {
  bluetooth_connection_service_unsubscribe();
  gpath_destroy(s_bt_path);
  layer_destroy(s_bt_layer);
}
