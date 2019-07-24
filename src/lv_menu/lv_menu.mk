CSRCS += lv_menu.c

DEPPATH += --dep-path $(LVGL_DIR)/lv_apps/lv_menu
VPATH += :$(LVGL_DIR)/lv_apps/lv_menu

CFLAGS += "-I$(LVGL_DIR)/lv_apps/lv_menu"
