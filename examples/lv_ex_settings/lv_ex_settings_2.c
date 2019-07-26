/**
 * @file lv_ex_settings_2.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_ex_settings_2.h"
#include "../../src/lv_settings/lv_settings.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void root_event_cb(lv_obj_t * btn, lv_event_t e);
static void motor_menu_event_cb(lv_obj_t * btn, lv_event_t e);
static void misc_menu_event_cb(lv_obj_t * btn, lv_event_t e);
static void time_menu_event_cb(lv_obj_t * btn, lv_event_t e);

/**********************
 *  STATIC VARIABLES
 **********************/

/*Declare items*/
static lv_settings_item_t root_item = {.name = "Settings", .value = "", .event_cb = root_event_cb};

static lv_settings_item_t main_menu_items[] =
{
       {.type = LV_SETTINGS_TYPE_LIST_BTN, .name="Motor settings", .value="Current, RPM, etc", .event_cb = motor_menu_event_cb},
       {.type = LV_SETTINGS_TYPE_LIST_BTN, .name="Misc. settings", .value="Time, brightness, etc.", .event_cb = misc_menu_event_cb},
       {.type = LV_SETTINGS_TYPE_INV},     /*Mark the last item*/
};

static lv_settings_item_t motor_menu_items[] = {
        {.type = LV_SETTINGS_TYPE_SLIDER, .name = "Max RPM", .value = "0RPM", .state = 0, .event_cb = motor_menu_event_cb},
        {.type = LV_SETTINGS_TYPE_NUMSET, .name = "Max Current", .value = "1A", .state = 1, .event_cb = motor_menu_event_cb},
        {.type = LV_SETTINGS_TYPE_DDLIST, .name = "Motor type", .value = "Type 1\n"
                                                                         "Type 2\n"
                                                                         "Type 3",
         .event_cb = motor_menu_event_cb},
        {.type = LV_SETTINGS_TYPE_SW, .name = "Short circuit protect", .value = "Enabled", .event_cb = motor_menu_event_cb},
        {.type = LV_SETTINGS_TYPE_BTN, .name = "Test motor", .value = "Start", .event_cb = motor_menu_event_cb},
        {.type = LV_SETTINGS_TYPE_INV},     /*Mark the last item*/
};


static char time_buf[16] = {"12:30"};
static lv_settings_item_t misc_menu_items[] =
{
        {.type = LV_SETTINGS_TYPE_LIST_BTN, .name="Time", .value=time_buf, .event_cb = time_menu_event_cb},
        {.type = LV_SETTINGS_TYPE_SLIDER, .name="Brightness", .value="70%", .state = 70*256/100, .event_cb = misc_menu_event_cb},
        {.type=LV_SETTINGS_TYPE_INV},       /*Mark the last item*/
};

static lv_settings_item_t time_menu_items[] =
{
        {.type = LV_SETTINGS_TYPE_NUMSET, .name="Hour", .value="12", .state = 12, .event_cb = time_menu_event_cb},
        {.type = LV_SETTINGS_TYPE_NUMSET, .name="Minute", .value="30", .state = 30, .event_cb = time_menu_event_cb},
        {.type=LV_SETTINGS_TYPE_INV},    /*Mark the last item*/
};



/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_ex_settings_2(void)
{
    lv_theme_t *th = lv_theme_material_init(210, NULL);

    /*Try with different theme too*/
//    lv_theme_t *th = lv_theme_material_init(10, NULL);
//    lv_theme_t *th = lv_theme_night_init(40, NULL);

    lv_theme_set_current(th);

    lv_settings_create(&root_item);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void root_event_cb(lv_obj_t * btn, lv_event_t e)
{
   /*If the root element was clicked or asks for refresh create a main menu*/
    if(e == LV_EVENT_CLICKED || e == LV_EVENT_REFRESH) {
        /*Get the caller item*/
        lv_settings_item_t * act_item = (lv_settings_item_t *)lv_event_get_data();

        /*Create a new page in the menu*/
        lv_obj_t * page = lv_settings_create_page(act_item);

        /*Add the items*/
        uint32_t i;
        for(i = 0; main_menu_items[i].type != LV_SETTINGS_TYPE_INV; i++) {
            lv_settings_add(page, &main_menu_items[i]);
        }
    }
}

static void motor_menu_event_cb(lv_obj_t * btn, lv_event_t e)
{
    /*Get the caller item*/
    lv_settings_item_t * act_item = (lv_settings_item_t *)lv_event_get_data();

    /*If the item in the main menu was clicked or it asks refresh create a Motor settings menu*/
    if((e == LV_EVENT_REFRESH || e == LV_EVENT_CLICKED) && strcmp(act_item->name, "Motor settings") == 0) {
        /*Create a new page in the menu*/
        lv_obj_t * page = lv_settings_create_page(act_item);

        /*Add the motor_menu_items*/
        uint32_t i;
        for(i = 0; motor_menu_items[i].type != LV_SETTINGS_TYPE_INV; i++) {
            lv_settings_add(page, &motor_menu_items[i]);
        }

        return;
    }

    /*Handle the events of the Motor settings items*/
    if(e == LV_EVENT_VALUE_CHANGED) {
        if(strcmp("Max RPM", act_item->name) == 0) {
            static char buf[32];
            sprintf(buf, "%dRPM", act_item->state);
            act_item->value = buf;
            lv_settings_refr(act_item);
        }
        if(strcmp("Max Current", act_item->name) == 0) {
            static char buf[32];
            if(act_item->state > 10) act_item->state = 10;
            if(act_item->state < 1) act_item->state = 1;
            sprintf(buf, "%dA", act_item->state);
            act_item->value = buf;
            lv_settings_refr(act_item);
        }
        else if(strcmp("The drop down", act_item->name) == 0) {
            printf("ddlist: %d\n", act_item->state);
        }
        else if(strcmp("Short circuit protect", act_item->name) == 0) {
            if(act_item->state) act_item->value = "Protected";
            else act_item->value = "Not protected";
            lv_settings_refr(act_item);
        }
    }
    else if(e == LV_EVENT_CLICKED) {
        if(strcmp("Test motor", act_item->name) == 0) {
            act_item->name = "Testing";
            act_item->value = "Stop";
            lv_settings_refr(act_item);
        }

        else if(strcmp("Testing", act_item->name) == 0) {
            act_item->name = "Test motor";
            act_item->value = "Start";
            lv_settings_refr(act_item);
        }
    }
}

static void misc_menu_event_cb(lv_obj_t * btn, lv_event_t e)
{
    /*Get the caller act_item*/
    lv_settings_item_t * act_item = (lv_settings_item_t *)lv_event_get_data();

    /*If the item in the main menu was clicked or it asks refresh create a Motor settings menu*/
    if((e == LV_EVENT_REFRESH || e == LV_EVENT_CLICKED) && strcmp(act_item->name, "Misc. settings") == 0) {
                /*Create a new page in the menu*/
        lv_obj_t * page = lv_settings_create_page(act_item);

        /*Add the misc_menu_items*/
        uint32_t i;
        for(i = 0; misc_menu_items[i].type != LV_SETTINGS_TYPE_INV; i++) {
            lv_settings_add(page, &misc_menu_items[i]);
        }

        return;
    }

    /*Handle the events of the Motor settings items*/
    if(e == LV_EVENT_VALUE_CHANGED) {
        if(strcmp("Brightness", act_item->name) == 0) {
            static char buf[32];
            sprintf(buf, "%d%%", (act_item->state * 100) / 256);
            act_item->value = buf;
            lv_settings_refr(act_item);
        }
    }
}

static void time_menu_event_cb(lv_obj_t * obj, lv_event_t e)
{
    /*Get the caller act_item*/
    lv_settings_item_t * act_item = (lv_settings_item_t *)lv_event_get_data();

    /*If the item in the main menu was clicked or it asks refresh create a Motor settings menu*/
    if((e == LV_EVENT_REFRESH || e == LV_EVENT_CLICKED) && strcmp(act_item->name, "Time") == 0) {

        /*Create a new page in the menu*/
        lv_obj_t * page = lv_settings_create_page(act_item);

        /*Add the items*/
        uint32_t i;
        for(i = 0; time_menu_items[i].type != LV_SETTINGS_TYPE_INV; i++) {
            lv_settings_add(page, &time_menu_items[i]);
        }

        return;
    }

    /*Handle the events of the Motor settings items*/
    if(e == LV_EVENT_VALUE_CHANGED) {
        if(strcmp("Hour", act_item->name) == 0) {
            static char buf[32];
            if(act_item->state > 23) act_item->state = 0;
            if(act_item->state < 0) act_item->state = 23;

            sprintf(buf, "%d", act_item->state);
            act_item->value = buf;
            lv_settings_refr(act_item);

            sprintf(misc_menu_items[0].value, "%02d:%02d", time_menu_items[0].state, time_menu_items[1].state);

        }
        else if(strcmp("Minute", act_item->name) == 0) {
            static char buf[32];
            if(act_item->state > 59) act_item->state = 0;
            if(act_item->state < 0) act_item->state = 59;

            sprintf(buf, "%d", act_item->state);
            act_item->value = buf;
            lv_settings_refr(act_item);

            sprintf(misc_menu_items[0].value, "%02d:%02d", time_menu_items[0].state, time_menu_items[1].state);
        }
    }
}
