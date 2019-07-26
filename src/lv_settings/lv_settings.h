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

typedef enum {
    LV_SETTINGS_TYPE_LIST_BTN,
    LV_SETTINGS_TYPE_BTN,
    LV_SETTINGS_TYPE_SW,
    LV_SETTINGS_TYPE_DDLIST,
    LV_SETTINGS_TYPE_NUMSET,
    LV_SETTINGS_TYPE_SLIDER,

    LV_SETTINGS_TYPE_INV = 0xff,
}lv_settings_type_t;

typedef struct {
    lv_settings_type_t type;
    const char * name;          /*Name or title of the item*/
    const char * value;         /*The current value as string*/
    int32_t state;              /*The current state, e.g. slider's value, switch state as a number */
    lv_event_cb_t event_cb;
    lv_obj_t * cont;
}lv_settings_item_t;


/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a settings application
 * @param root_item descriptor of the settings button. For example:
 * `lv_settings_menu_item_t root_item = {.name = "Settings", .event_cb = root_event_cb};`
 */
void lv_settings_create(lv_settings_item_t * root_item);

/**
 * Create a new settings page for an item
 * @param item pointer to a an `lv_settings_item_t` item.
 * `item->name` will be the title of the page.
 * `LV_EVENT_REFRESH` will be sent to `item->event_cb` to create the page again when the back button is pressed.
 * @return pointer to the created page
 */
lv_obj_t * lv_settings_create_page(lv_settings_item_t * item);

/**
 * Add a list element to the page. With `item->name` and `item->value` texts.
 * @param page pointer to a menu page created by `lv_settings_create_page`
 * @param item pointer to a an `lv_settings_item_t` item.
 */
void lv_settings_add(lv_obj_t * page, lv_settings_item_t * item);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_TEMPL_H*/
