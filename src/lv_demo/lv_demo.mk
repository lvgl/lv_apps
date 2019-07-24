CSRCS += lv_demo.c
CSRCS += lv_img_bubble_pattern.c

DEPPATH += --dep-path $(LVGL_DIR)lv_apps/lv_demo
VPATH += :$(LVGL_DIR)/lv_apps/lv_demo

CFLAGS += "-I$(LVGL_DIR)/lv_apps/lv_demo"
