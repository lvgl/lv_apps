/**
 * @file lv_settings.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_settings.h"

/*********************
 *      DEFINES
 *********************/
#define ANIM_TIME   300
#define MIN_WIDTH   250

/**********************
 *      TYPEDEFS
 **********************/

typedef struct
{
    lv_btn_ext_t btn;
    lv_settings_item_t * item;
}liste_ext_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_obj_t * empty_menu_create(const char * title);
static void list_add_item(lv_obj_t * list, lv_settings_item_t * menu_item);
static void list_btn_open_event_cb(lv_obj_t * btn, lv_event_t e);
static void header_back_event_cb(lv_obj_t * btn, lv_event_t e);
static void old_cont_del_cb(lv_anim_t * a);

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_settings_item_t * root_menu_item;
static lv_obj_t * act_cont;
static lv_style_t style_menu_bg;
static lv_style_t style_bg;
static lv_ll_t history_ll;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create a settings application
 * @param main_menu_item descriptor of the main menu item. For example:
 * `lv_settings_menu_item_t root_menu_item = {.name = "Settings", .event_cb = root_event_cb};`
 */
void lv_settings_create(lv_settings_item_t * _root_menu_item)
{
    root_menu_item = _root_menu_item;

    lv_theme_t *th = lv_theme_get_current();
    if(th) {
        lv_style_copy(&style_menu_bg, th->style.cont);
    } else {
        lv_style_copy(&style_menu_bg, &lv_style_pretty);
    }

    lv_style_copy(&style_bg, &lv_style_transp_tight);
    style_menu_bg.body.radius = 0;

    lv_obj_t * menu_btn = lv_btn_create(lv_scr_act(), NULL);
    lv_btn_set_fit(menu_btn, LV_FIT_TIGHT);
    liste_ext_t * ext = lv_obj_allocate_ext_attr(menu_btn, sizeof(liste_ext_t));
    ext->item = root_menu_item;

    lv_obj_set_event_cb(menu_btn, list_btn_open_event_cb);

    lv_obj_t * menu_label = lv_label_create(menu_btn, NULL);
    lv_label_set_text(menu_label, LV_SYMBOL_LIST);

    lv_obj_set_pos(menu_btn, 0, 0);

    lv_ll_init(&history_ll, sizeof(lv_settings_item_t));
}

lv_settings_item_t * lv_settings_get_menu_item(lv_obj_t * btn)
{
    liste_ext_t * ext = lv_obj_get_ext_attr(btn);
    return ext->item;
}


void lv_settings_create_list(const char * title, lv_settings_item_t * items)
{
    lv_obj_t * header = empty_menu_create(title);

    lv_obj_t * list = lv_list_create(act_cont, NULL);
    lv_list_set_style(list, LV_LIST_STYLE_BG, &style_bg);
    lv_list_set_edge_flash(list, true);
    lv_obj_set_size(list, lv_obj_get_width(act_cont), lv_obj_get_height(lv_scr_act()) - lv_obj_get_height(header));
    lv_obj_align(list, header, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);

    uint32_t i;
    for(i = 0; items[i].name; i++) {
        list_add_item(list, &items[i]);
    }
}

lv_obj_t * lv_settings_create_page(const char * title)
{
    lv_obj_t * header = empty_menu_create(title);

    lv_obj_t * page = lv_page_create(act_cont, NULL);
    lv_page_set_style(page, LV_PAGE_STYLE_BG, &style_bg);
    lv_list_set_edge_flash(page, true);
    lv_obj_set_size(page, lv_obj_get_width(act_cont), lv_obj_get_height(lv_scr_act()) - lv_obj_get_height(header));
    lv_obj_align(page, header, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_page_set_scrl_layout(page, LV_LAYOUT_COL_L);

    return page;
}

void lv_settings_add_slider(lv_obj_t * page, lv_settings_item_t * item)
{
    lv_obj_t * slider = lv_slider_create(page, NULL);
    lv_obj_set_height(slider, LV_DPI / 4);
    lv_obj_t * label = lv_label_create(page, NULL);
    lv_label_set_text(label, item->value);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void list_btn_open_event_cb(lv_obj_t * btn, lv_event_t e)
{
    if(e != LV_EVENT_CLICKED) return;

    /*Save the old container*/
    lv_obj_t * old_menu_cont = act_cont;

    /*Save the menu item because the button will be deleted in `menu_cont_create` and `ext` will be invalid */
    liste_ext_t * ext = lv_obj_get_ext_attr(btn);

    /*If the button doesn't handle events do nothing.*/
    if(ext->item->event_cb == NULL) return;

    /*Call the button's event handler to create the menu*/
    lv_event_send_func(ext->item->event_cb, NULL, LV_EVENT_REFRESH, ext->item);

    lv_settings_item_t * prev_node = lv_ll_ins_head(&history_ll);
    memcpy(prev_node, ext->item, sizeof(lv_settings_item_t));

    /*Delete the old menu container after some time*/
    if(old_menu_cont) {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_exec_cb(&a, old_menu_cont, NULL);
        lv_anim_set_values(&a, 0, 1);
        lv_anim_set_path_cb(&a, lv_anim_path_step);
        lv_anim_set_time(&a, ANIM_TIME, 0);
        lv_anim_set_ready_cb(&a, old_cont_del_cb);
        lv_anim_create(&a);
    }

    /*Float in the new menu*/
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, act_cont, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_values(&a, -lv_obj_get_width(act_cont), 0);
    lv_anim_set_time(&a, ANIM_TIME, 0);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in);
    lv_anim_create(&a);

}

static void header_back_event_cb(lv_obj_t * btn, lv_event_t e)
{
    if(e != LV_EVENT_CLICKED) return;

    lv_obj_t * old_menu_cont = act_cont;

    lv_settings_item_t * prev_item;

    /*Delete the current item form the history. The goal is the previous.*/
    prev_item = lv_ll_get_head(&history_ll);
    if(prev_item) {
        lv_ll_rem(&history_ll, prev_item);
        lv_mem_free(prev_item);

        /*Get the real previous item and open it*/
        prev_item =  lv_ll_get_head(&history_ll);

        if(prev_item) lv_event_send_func(prev_item->event_cb, NULL, LV_EVENT_REFRESH, prev_item);
        else act_cont = NULL;

    }

    /*Float out the old menu container*/
    if(old_menu_cont) {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_exec_cb(&a, old_menu_cont, (lv_anim_exec_xcb_t)lv_obj_set_x);
        lv_anim_set_values(&a, 0, -lv_obj_get_width(old_menu_cont));
        lv_anim_set_path_cb(&a, lv_anim_path_ease_in);
        lv_anim_set_time(&a, ANIM_TIME, 0);
        lv_anim_set_ready_cb(&a, old_cont_del_cb);
        lv_anim_create(&a);

        /*Move the old menu to the for ground. */
        lv_obj_move_foreground(old_menu_cont);
    }

}

static lv_obj_t * empty_menu_create(const char * title)
{
    lv_coord_t w = LV_MATH_MIN(lv_obj_get_width(lv_scr_act()), MIN_WIDTH);

    act_cont = lv_cont_create(lv_scr_act(), NULL);
    lv_cont_set_style(act_cont, LV_CONT_STYLE_MAIN, &style_menu_bg);
    lv_obj_set_size(act_cont, w, lv_obj_get_height(lv_scr_act()));

    lv_obj_t * header = lv_cont_create(act_cont, NULL);
    lv_cont_set_style(header, LV_CONT_STYLE_MAIN, &lv_style_transp_fit);
    lv_cont_set_fit2(header, LV_FIT_NONE, LV_FIT_TIGHT);
    lv_cont_set_layout(header, LV_LAYOUT_ROW_M);
    lv_obj_set_width(header, lv_obj_get_width(act_cont));

    lv_obj_t * header_back_btn = lv_btn_create(header, NULL);
    lv_btn_set_fit(header_back_btn, LV_FIT_TIGHT);
    lv_obj_set_event_cb(header_back_btn, header_back_event_cb);

    lv_obj_t * header_back_label = lv_label_create(header_back_btn, NULL);
    lv_label_set_text(header_back_label, LV_SYMBOL_LEFT);

    lv_obj_t * header_title = lv_label_create(header, NULL);
    lv_label_set_text(header_title, title);

    lv_obj_set_pos(header, 0, 0);
    
    return header;
}

static void list_add_item(lv_obj_t * list, lv_settings_item_t * menu_item)
{
    lv_obj_t * liste = lv_list_add_btn(list, NULL, menu_item->name);
    lv_btn_set_layout(liste, LV_LAYOUT_COL_L);
    lv_obj_set_event_cb(liste, list_btn_open_event_cb);

    liste_ext_t * ext = lv_obj_allocate_ext_attr(liste, sizeof(liste_ext_t));
    ext->item = menu_item;

    lv_obj_t * value_label = lv_label_create(liste, NULL);
    lv_label_set_text(value_label, menu_item->value);

    lv_theme_t * th = lv_theme_get_current();
    if(th) {
        lv_label_set_style(value_label, LV_LABEL_STYLE_MAIN, th->style.label.hint);
    }
}

static void old_cont_del_cb(lv_anim_t * a)
{
    lv_obj_del(a->var);
}

