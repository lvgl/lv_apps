/**
 * @file lv_settings.h
 *
 */

#ifndef LV_SETTINGS_H
#define LV_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    const char * name;
    const char * value;
    lv_event_cb_t event_cb;
}lv_settings_item_t;


/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_settings_create(lv_settings_item_t * main_menu_item);
lv_settings_item_t * lv_settings_get_menu_item(lv_obj_t * btn);
void lv_settings_create_list(const char * title, lv_settings_item_t * items);
lv_obj_t * lv_settings_create_page(const char * title);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_TEMPL_H*/
