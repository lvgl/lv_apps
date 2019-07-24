/**
 * @file lv_settings.c
 *
 */

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
}menu_item_t;

typedef struct
{
    lv_btn_ext_t btn;
    menu_item_t * menu_item;
}liste_ext_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void main_menu_btn_event_cb(lv_obj_t * btn, lv_event_t e);
static void misc_menu_btn_event_cb(lv_obj_t * btn, lv_event_t e);
static void slider_menu_btn_event_cb(lv_obj_t * btn, lv_event_t e);

static void list_create(const char * title, menu_item_t * items, lv_event_cb_t back_event_cb);
static void list_add_item(lv_obj_t * list, menu_item_t * menu_item);
static void settings_exit_event_cb(lv_obj_t * obj, lv_event_t e);
static lv_obj_t * settings_cont_create(void);
static lv_obj_t * header_create(const char * title, lv_event_cb_t back_event_cb);

/**********************
 *  STATIC VARIABLES
 **********************/
static menu_item_t main_menu_items[] =
{
        {.name="Max. Motor RPM", .value="800", .event_cb = slider_menu_btn_event_cb},
        {.name="Max. Power", .value="1000 W", .event_cb = NULL},
        {.name="Misc. settings", .value="", .event_cb = misc_menu_btn_event_cb},
        {.name="Dummy1", .value="123", .event_cb = NULL},
        {.name="Dummy2", .value="123", .event_cb = NULL},
        {.name="Dummy3", .value="123", .event_cb = NULL},
        {.name="Dummy4", .value="123", .event_cb = NULL},
        {.name="Dummy5", .value="123", .event_cb = NULL},
        {.name="Dummy6", .value="123", .event_cb = NULL},
        {.name=NULL}
};

static menu_item_t misc_menu_items[] =
{
        {.name="Time", .value="12:30", .event_cb = NULL},
        {.name="Date", .value="03.09.2018", .event_cb = NULL},
        {.name="Brightness", .value="70%", .event_cb = NULL},
        {.name=NULL}
};

static lv_obj_t * act_menu_cont;
static lv_event_cb_t act_back_event_cb;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 *
 */
void lv_settings_create(void)
{
#if LV_USE_THEME_MATERIAL
    lv_theme_t *th = lv_theme_material_init(280, NULL);
    lv_theme_set_current(th);
#endif

    lv_obj_t * menu_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_btn_set_fit(menu_btn, LV_FIT_TIGHT);
    lv_obj_set_event_cb(menu_btn, main_menu_btn_event_cb);

    lv_obj_t * menu_label = lv_label_create(menu_btn, NULL);
    lv_label_set_text(menu_label, LV_SYMBOL_LIST);

    lv_obj_set_pos(menu_btn, 0, 0);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void main_menu_btn_event_cb(lv_obj_t * btn, lv_event_t e)
{
    if(e == LV_EVENT_CLICKED) {
        list_create("Settings", main_menu_items, settings_exit_event_cb);
    }
}

static void misc_menu_btn_event_cb(lv_obj_t * btn, lv_event_t e)
{
    if(e == LV_EVENT_CLICKED) {
        list_create("Misc. settings", misc_menu_items, main_menu_btn_event_cb);
    }
}

static void slider_menu_btn_event_cb(lv_obj_t * btn, lv_event_t e)
{
    if(e == LV_EVENT_CLICKED) {
        liste_ext_t * ext = lv_obj_get_ext_attr(btn);
        menu_item_t * menu_item = ext->menu_item;       /*Save the menu item because the button will be deleted in `settings_cont_create` and `ext` will be invalid */
        settings_cont_create();
        lv_obj_t * header = header_create(menu_item->name, main_menu_btn_event_cb);

        lv_obj_t * label = lv_label_create(act_menu_cont, NULL);
        lv_label_set_text(label, menu_item->value);
        lv_obj_set_pos(label, 20, 100);
    }
}

static void list_create(const char * title, menu_item_t * items, lv_event_cb_t back_event_cb)
{
    settings_cont_create();
    lv_obj_t * header = header_create(title, back_event_cb);

    lv_obj_t * list = lv_list_create(act_menu_cont, NULL);
    lv_obj_set_size(list, lv_obj_get_width(act_menu_cont), lv_obj_get_height(lv_scr_act()) - lv_obj_get_height(header));
    lv_obj_align(list, header, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    uint32_t i;
    for(i = 0; items[i].name; i++) {
        list_add_item(list, &items[i]);
    }
}

static void list_add_item(lv_obj_t * list, menu_item_t * menu_item)
{
    lv_obj_t * liste = lv_list_add_btn(list, NULL, menu_item->name);
    lv_btn_set_layout(liste, LV_LAYOUT_COL_L);
    lv_obj_set_event_cb(liste, menu_item->event_cb);
    liste_ext_t * ext = lv_obj_allocate_ext_attr(liste, sizeof(liste_ext_t));
    ext->menu_item = menu_item;

    lv_obj_t * value_label = lv_label_create(liste, NULL);
    lv_label_set_text(value_label, menu_item->value);
}

static void settings_exit_event_cb(lv_obj_t * obj, lv_event_t e)
{
    if(e == LV_EVENT_CLICKED) {
        if(act_menu_cont) lv_obj_del(act_menu_cont);
        act_menu_cont = NULL;
    }
}

static lv_obj_t * settings_cont_create(void)
{
    lv_coord_t w = LV_MATH_MIN(lv_obj_get_width(lv_scr_act()), 200);

    if(act_menu_cont) lv_obj_del(act_menu_cont);

    act_menu_cont = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(act_menu_cont, w, lv_obj_get_height(lv_scr_act()));

    return act_menu_cont;
}

static lv_obj_t * header_create(const char * title, lv_event_cb_t back_event_cb)
{
    lv_obj_t * header = lv_cont_create(act_menu_cont, NULL);
    lv_cont_set_style(header, LV_CONT_STYLE_MAIN, &lv_style_transp_fit);
    lv_cont_set_fit2(header, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_cont_set_layout(header, LV_LAYOUT_ROW_M);
    lv_obj_set_width(header, lv_obj_get_width(act_menu_cont));

    lv_obj_t * header_back_btn = lv_btn_create(header, NULL);
    lv_btn_set_fit(header_back_btn, LV_FIT_TIGHT);
    lv_obj_set_event_cb(header_back_btn, back_event_cb);

    lv_obj_t * header_back_label = lv_label_create(header_back_btn, NULL);
    lv_label_set_text(header_back_label, LV_SYMBOL_LEFT);

    lv_obj_t * header_title = lv_label_create(header, NULL);
    lv_label_set_text(header_title, title);

    lv_obj_set_pos(header, 0, 0);
    
    return header;
}

