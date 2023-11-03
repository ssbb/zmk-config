/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include "kitsune_status_screen.h"
#include "font_custom.h"
#include "widgets/battery_status.h"
#include "widgets/layer_status.h"
#include "widgets/output_status.h"

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if IS_ENABLED(CONFIG_KITSUNE_BONGO_CAT)
#include "kitsune_bongo_cat_widget.h"

static struct kitsune_bongo_cat_widget bongo_widget;
#endif

#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_BATTERY_STATUS)
static struct zmk_widget_battery_status battery_status_widget;
#endif

#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_OUTPUT_STATUS)
static struct zmk_widget_output_status output_status_widget;
#endif

#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_LAYER_STATUS)
static struct zmk_widget_layer_status layer_status_widget;
#endif

lv_style_t global_style;

lv_obj_t *zmk_display_status_screen() {
  lv_obj_t *screen;
#if !IS_ENABLED(CONFIG_KITSUNE_BONGO_CAT)
  lv_obj_t *dont_label;
  lv_obj_t *panic_label;
#endif
  lv_obj_t *center_frame;

  lv_style_init(&global_style);
  lv_style_set_text_font(&global_style, &font_custom_26);
  lv_style_set_text_letter_space(&global_style, 1);
  lv_style_set_text_line_space(&global_style, 1);

  screen = lv_obj_create(NULL);
  lv_obj_add_style(screen, &global_style, LV_PART_MAIN);

#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_BATTERY_STATUS)
  zmk_widget_battery_status_init(&battery_status_widget, screen);
  lv_obj_align(zmk_widget_battery_status_obj(&battery_status_widget),
               LV_ALIGN_TOP_RIGHT, -5, 5);
#endif

#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_OUTPUT_STATUS)
  zmk_widget_output_status_init(&output_status_widget, screen);
  lv_obj_align(zmk_widget_output_status_obj(&output_status_widget),
               LV_ALIGN_TOP_LEFT, 5, 5);
#endif

  center_frame = lv_obj_create(screen);
  lv_obj_align(center_frame, LV_ALIGN_CENTER, 0, 0);
  lv_obj_center(center_frame);

#if IS_ENABLED(CONFIG_KITSUNE_BONGO_CAT)
  kitsune_bongo_cat_widget_init(&bongo_widget, center_frame);
#else
  dont_label = lv_label_create(center_frame);
  lv_label_set_text(dont_label, "Don't");

  panic_label = lv_label_create(center_frame);
  lv_label_set_text(panic_label, "Panic");
  lv_obj_update_layout(dont_label); // otherwise proper height is not known
  lv_obj_set_y(panic_label, lv_obj_get_height(dont_label));
#endif // IS_ENABLED(CONFIG_KITSUNE_BONGO_CAT)

  lv_obj_set_size(center_frame, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

#if IS_ENABLED(CONFIG_CUSTOM_WIDGET_LAYER_STATUS)
  zmk_widget_layer_status_init(&layer_status_widget, screen);
  lv_obj_align(zmk_widget_layer_status_obj(&layer_status_widget),
               LV_ALIGN_BOTTOM_LEFT, 5, -5);
  /* lv_obj_set_size(zmk_widget_layer_status_obj(&layer_status_widget), */
  /*                 LV_SIZE_CONTENT, LV_SIZE_CONTENT); */

#endif

  return screen;
}
