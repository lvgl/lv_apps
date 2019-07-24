CSRCS += lv_wifi.c

DEPPATH += --dep-path $(LVGL_DIR)/lv_apps/lv_wifi
VPATH += :$(LVGL_DIR)/lv_apps/lv_wifi

CFLAGS += "-I$(LVGL_DIR)/lv_apps/lv_wifi"
