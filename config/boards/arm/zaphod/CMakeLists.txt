add_compile_definitions(LV_LVGL_H_INCLUDE_SIMPLE)

if(CONFIG_ZMK_DISPLAY)
  target_sources_ifdef(CONFIG_CUSTOM_WIDGET_BATTERY_STATUS app PRIVATE widgets/battery_status.c)
  target_sources_ifdef(CONFIG_CUSTOM_WIDGET_OUTPUT_STATUS app PRIVATE widgets/output_status.c)
  target_sources_ifdef(CONFIG_CUSTOM_WIDGET_LAYER_STATUS app PRIVATE widgets/layer_status.c)
endif()

zephyr_library()
zephyr_library_include_directories(${ZEPHYR_LVGL_MODULE_DIR})
zephyr_library_include_directories(${ZEPHYR_BASE}/lib/gui/lvgl/)

zephyr_library_sources_ifdef(CONFIG_ZMK_DISPLAY font_custom_26.c)
zephyr_library_sources_ifdef(CONFIG_ZMK_DISPLAY zaphod_status_screen.c)
zephyr_library_sources_ifdef(CONFIG_ZAPHOD_BONGO_CAT zaphod_bongo_cat_widget.c)
zephyr_library_sources_ifdef(CONFIG_ZAPHOD_BONGO_CAT zaphod_bongo_cat_images.c)
zephyr_library_sources(${ZEPHYR_BASE}/misc/empty_file.c)
zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
zephyr_library_include_directories(${ZEPHYR_BASE}/drivers)
