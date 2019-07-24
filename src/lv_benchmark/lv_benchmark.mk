CSRCS += lv_benchmark.c
CSRCS += lv_benchmark_bg.c

DEPPATH += --dep-path $(LVGL_DIR)/lv_apps/lv_benchmark
VPATH += :$(LVGL_DIR)/lv_apps/lv_benchmark

CFLAGS += "-I$(LVGL_DIR)lv_apps/lv_benchmark"
