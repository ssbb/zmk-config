/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include "../font_custom.h"
#include "output_status.h"
#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/endpoints.h>
#include <zmk/event_manager.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_selection_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/usb.h>

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct output_status_state {
  enum zmk_endpoint selected_endpoint;
  bool active_profile_connected;
  bool active_profile_bonded;
  uint8_t active_profile_index;
};

static struct output_status_state get_state(const zmk_event_t *_eh) {
  return (struct output_status_state){
      .selected_endpoint = zmk_endpoints_selected(),
      .active_profile_connected = zmk_ble_active_profile_is_connected(),
      .active_profile_bonded = !zmk_ble_active_profile_is_open(),
      .active_profile_index = zmk_ble_active_profile_index()};
  ;
}

static void set_status_symbol(lv_obj_t *label,
                              struct output_status_state state) {
  char text[11] = {};

  switch (state.selected_endpoint) {
  case ZMK_ENDPOINT_USB:
    strcat(text, MY_SYMBOL_USB);
    break;
  case ZMK_ENDPOINT_BLE:
    char *icon;

    if (state.active_profile_index == 0) {
      icon = MY_SYMBOL_SQUARE_1;
    } else if (state.active_profile_index == 1) {
      icon = MY_SYMBOL_SQUARE_2;
    } else if (state.active_profile_index == 2) {
      icon = MY_SYMBOL_SQUARE_3;
    } else if (state.active_profile_index == 3) {
      icon = MY_SYMBOL_SQUARE_4;
    } else if (state.active_profile_index == 4) {
      icon = MY_SYMBOL_SQUARE_5;
    } else {
      icon = MY_SYMBOL_SQUARE_QUESTION;
    }

    if (state.active_profile_bonded) {
      if (state.active_profile_connected) {
        snprintf(text, sizeof(text), MY_SYMBOL_BLUETOOTH "%s", icon);
      } else {
        // snprintf(text, sizeof(text), MY_SYMBOL_BLUETOOTH "%s", icon);
        snprintf(text, sizeof(text), MY_SYMBOL_BLUETOOTH "%s " MY_SYMBOL_XMARK,
                 icon);
      }
    } else {
      snprintf(text, sizeof(text), MY_SYMBOL_BLUETOOTH "%s " MY_SYMBOL_GEAR,
               icon);
    }
    break;
  }

  lv_label_set_text(label, text);
}

static void output_status_update_cb(struct output_status_state state) {
  struct zmk_widget_output_status *widget;
  SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
    set_status_symbol(widget->obj, state);
  }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_output_status, struct output_status_state,
                            output_status_update_cb, get_state)
ZMK_SUBSCRIPTION(widget_output_status, zmk_endpoint_selection_changed);

#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
ZMK_SUBSCRIPTION(widget_output_status, zmk_usb_conn_state_changed);
#endif
#if defined(CONFIG_ZMK_BLE)
ZMK_SUBSCRIPTION(widget_output_status, zmk_ble_active_profile_changed);
#endif

int zmk_widget_output_status_init(struct zmk_widget_output_status *widget,
                                  lv_obj_t *parent) {
  widget->obj = lv_label_create(parent);

  sys_slist_append(&widgets, &widget->node);

  widget_output_status_init();
  return 0;
}

lv_obj_t *
zmk_widget_output_status_obj(struct zmk_widget_output_status *widget) {
  return widget->obj;
}
