/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include "../font_custom.h"
#include "layer_status.h"
#include <zmk/display.h>
#include <zmk/endpoints.h>
#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/keymap.h>

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct layer_status_state {
  uint8_t index;
  const char *label;
};

static void set_layer_symbol(lv_obj_t *label, struct layer_status_state state) {
  if (state.label == NULL) {
    char text[7] = {};

    sprintf(text, MY_SYMBOL_KEYBOARD " %i", state.index);

    lv_label_set_text(label, text);
  } else {
    char text[13] = {};
    // char tmp_label[13] = {};
    // char *system, *name, *tok_state;

    // strncpy(tmp_label, state.label, sizeof(tmp_label));
    // system = strtok_r(tmp_label, ":", &tok_state);
    // name = strtok_r(NULL, ":", &tok_state);

    // if (strcmp(system, "MAC") == 0) {
    //     snprintf(text, sizeof(text), MY_SYMBOL_APPLE " %s", name);
    // } else if (strcmp(system, "NIX") == 0) {
    //     snprintf(text, sizeof(text), MY_SYMBOL_LINUX " %s", name);
    // } else {
    //     snprintf(text, sizeof(text), MY_SYMBOL_KEYBOARD " %s", state.label);
    // }

    snprintf(text, sizeof(text), MY_SYMBOL_KEYBOARD " %s", state.label);

    lv_label_set_text(label, text);
  }
}

static void layer_status_update_cb(struct layer_status_state state) {
  struct zmk_widget_layer_status *widget;
  SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
    set_layer_symbol(widget->obj, state);
  }
}

static struct layer_status_state layer_status_get_state(const zmk_event_t *eh) {
  uint8_t index = zmk_keymap_highest_layer_active();
  return (struct layer_status_state){.index = index,
                                     .label = zmk_keymap_layer_name(index)};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_layer_status, struct layer_status_state,
                            layer_status_update_cb, layer_status_get_state)

ZMK_SUBSCRIPTION(widget_layer_status, zmk_layer_state_changed);

int zmk_widget_layer_status_init(struct zmk_widget_layer_status *widget,
                                 lv_obj_t *parent) {
  widget->obj = lv_label_create(parent);
  sys_slist_append(&widgets, &widget->node);
  widget_layer_status_init();

  return 0;
}

lv_obj_t *zmk_widget_layer_status_obj(struct zmk_widget_layer_status *widget) {
  return widget->obj;
}
