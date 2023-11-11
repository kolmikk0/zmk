zephyr_library()
zephyr_library_sources_ifdef(CONFIG_ZMK_LED_WIDGETS led_widgets.c)
zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
