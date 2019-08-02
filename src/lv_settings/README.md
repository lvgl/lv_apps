# Settings menu

**A collections of ready to use components to create smartphone like menus**

![Settings menu with LittlevGL](../../examples/lv_ex_settings/lv_ex_settings_2.gif)

## Overview

The menu build up from 
1. A *root item* which opens the menu on click
2. Pages of the menu

There is a back button every page to go back to the previous page or on the last page to close the menu.

These descriptor element of menu items is `lv_settings_item_t`. It has the following fields:
- **type** of the item (`LV_SETTINGS_TYPE_...`). See below.
- **name** name of the item as a string
- **value** current value of the item as a string
- **state** the current or last state of the item as an integer (e.g. slider's value)
- **event_cb** event callback to inform the user is something happened with the item
- **user_date** a `void *` pointer to store any custom data
- **cont** pointer to the created lvgl object if exists. (Used internally)

In the event callbacks the caller item can get with
```c
lv_settings_item_t * act_item = (lv_settings_item_t *)lv_event_get_data();
```

### Item types

#### List button
Referred as  `LV_SETTINGS_TYPE_LIST_BTN`,

A large button. Usually used to open new pages in the menu.  
![List button in settings application - LittlevGL](https://github.com/littlevgl/lv_apps/raw/master/misc/lv_settings_list_btn.png)

It displays the `name` and value of the `item`. If a theme is set `value` will use `theme.style.label.hint` style.

The list buttons don't use `state` so it can be freely if required to keep track of the state of something.

In `event_cb`
- `LV_EVENT_CLICKED`, `LV_EVENT_SHORT_CLICKED`, `LV_EVENT_LONG_PRESSED` is sent when the list button is clicked 
- `LV_EVENT_REFRESH` is sent when the new page, open by this button, needs to be created again. (When go to the created page with *Back* button)

#### Button
Referred as  `LV_SETTINGS_TYPE_BTN`,

Displays `name` and a button with `value` text on it.
![Button in settings application - LittlevGL](https://github.com/littlevgl/lv_apps/blob/master/misc/lv_settings_btn.gif)

The buttons don't use `state` so it can be freely if required to keep track of the state of something.

In `event_cb` `LV_EVENT_CLICKED` is sent when the button is clicked 

#### Switch
Referred as  `LV_SETTINGS_TYPE_SW`,

Displays a switch with `name` and `value` texts.
![Switch in settings application - LittlevGL](https://github.com/littlevgl/lv_apps/blob/master/misc/lv_settings_sw.gif)

The current state is stored in the `state` filed.

In `event_cb` `LV_EVENT_VALUE_CHANGED` is sent when the switch changes state.

#### Drop down list
Referred as  `LV_SETTINGS_TYPE_DDLIST`,

Displays a Drop down list with `name` as title and options stored in `value`.
![Drop down list in settings application - LittlevGL](https://github.com/littlevgl/lv_apps/blob/master/misc/lv_settings_ddlist.gif)

The current options id is stored in the `state` filed.

In `event_cb` `LV_EVENT_VALUE_CHANGED` is sent when a new option is selected.

#### Number set
Referred as  `LV_SETTINGS_TYPE_NUMSET`,

Displays the `name` as title, a `value` and `+`, `-` buttons.
![Number set in settings application - LittlevGL](https://github.com/littlevgl/lv_apps/blob/master/misc/lv_settings_numset.gif)

The current value is stored in the `state` filed.

In `event_cb` `LV_EVENT_VALUE_CHANGED` is sent when a new option is selected.


#### Slider
Referred as  `LV_SETTINGS_TYPE_SLIDER`,

Displays the `name` as title and `value` above a slider.
![Drop down list in settings application - LittlevGL](https://github.com/littlevgl/lv_apps/blob/master/misc/lv_settings_slider.gif)

The sliders range is `[0..255]`. The current value is stored in the `state` filed.

In `event_cb` `LV_EVENT_VALUE_CHANGED` is sent when a new option is selected.

## Create a menu

### Initialize items

To initialize an item you have to set its fields.
```c
static lv_settigns_item_t item1;
item1.type = LV_SETTINGS_TYPE_SLIDER;
item1.name = "Slider1";
item1.value = "30 %";
item1.state = (50 * 256) / 100;  /*30 %, default slider range is 256*/
item2.event_cb = slider_event_cb;
```

It's a good practice to set an array for `value` if it will change later.
```c
static char slider2_value[32] = {"50 %"};
static lv_settigns_item_t item2;
item2.type = LV_SETTINGS_TYPE_SLIDER;
item2.name = "Slider2";
item2.value = slider_value;
item2.state = (50 * 256) / 100 ;  /*50 %*/
item2.event_cb = slider_event_cb;
``` 

The items can be initialized compile time too.
```c
static char slider3_value[32] = {"70 %"};
static const lv_settigns_item_t item3 = {
  .type = LV_SETTINGS_TYPE_SLIDER;
  .name = "Slider3";
  .value = slider3_value;
  .state = (70 * 256) / 100 ;  /*70 %*/
  .event_cb = slider_event_cb;
};
```

And the item can be grouped into an array too.
```c
static char slider_values[3][32] = {"90 %",  "91 %",  "92 %"};
static const lv_settigns_item_t items[3] = {
  { .type = LV_SETTINGS_TYPE_SLIDER, .name = "Slider4_1",  .value = slider_value[0] .state = (90 * 256) / 100,.event_cb = slider_event_cb},
  { .type = LV_SETTINGS_TYPE_SLIDER, .name = "Slider4_2",  .value = slider_value[1] .state = (91 * 256) / 100 .event_cb = slider_event_cb},
  { .type = LV_SETTINGS_TYPE_SLIDER, .name = "Slider4_3",  .value = slider_value[2] .state = (92 * 256) / 100 .event_cb = slider_event_cb},
};
```

### Root element
The root element the button which opens the settings menu. It also has to be an item desriptor:
```c
static lv_settings_item_t root_item = {.name = "Settings", .value = "", .event_cb = root_event_cb};
```
 The root element receives `LV_EVENT_CLICKED` and `LV_EVENT_REFRESH` to create its menu.


### Create a page and add items
In the root element's or the list buttons event callback in `LV_EVENT_CLICKED` and `LV_EVENT_REFRESH` and new menu page shall be created like
```c
void root_event_cb(lv_obj_t * btn, lv_event_t e)
{
    (void)btn;  /*Unused*/

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
```

As it's shown in the example a new page is created with `lv_settings_create_page` from the `act_item`. 
`act_item`'s `name` will be the title of the new page. 
If further pages are opened from this page, and the "Back" button in child on a child page,  `LV_EVENT_REFRESH` will be sent to `act_item` to create the page and items again.

### Item events
Typically, items other than `LV_SETTINGS_TYPE_LIST_BTN`, are not used to create new pages. 
The event callback of the items receive `LV_EVENT_CLICKED` or `LV_EVENT_VALUE_CHANGE` depending on the type of the item.

It's up to the developer how to organize the events but there are few typical patterns: 
1. Create an event function for every item. For example `slider1_event_cb`, `slider2_event_cb`, `slider3_event_cb`...
2. Create events for a group of items. For example `slider_event_cb` and find the item like this:
```c

static void slider_event_cb(lv_obj_t * btn, lv_event_t e)
{
    (void)btn;  /*Unused*/

    /*Get the caller item*/
    lv_settings_item_t * act_item = (lv_settings_item_t *)lv_event_get_data();

    /*If the item in the main menu was clicked or it asks refresh create a Motor settings menu*/
    if(e == LV_EVENT_VALUE_CHANGED) {
        if(strcmp("Slider1", act_item->name) == 0) {
            /*Do somethind*/
        }
        else if(strcmp("Slider2", act_item->name) == 0) {
            /*Do something*/
        }
        
        ...
    }
}
```
3. Use the `user_data` to process the event. 

## Refresh items' data
In the event function probably some data of the item should be refreshed. 
Mainly the `value` string formatted as required and the `state` if it's needs to be limited.
The items can be refreshed with `lv_settings_refr(act_item);`.

For example for a "Number setting" item:
```c
static void motor_menu_event_cb(lv_obj_t * btn, lv_event_t e)
{
    (void)btn;  /*Unused*/

    /*Get the caller item*/
    lv_settings_item_t * act_item = (lv_settings_item_t *)lv_event_get_data();

    /*Handle the events of the Motor settings items*/
    if(e == LV_EVENT_VALUE_CHANGED) {
        if(strcmp("Max Current", act_item->name) == 0) {
            /*Limit the values*/
            if(act_item->state > 10) act_item->state = 10;
            if(act_item->state < 1) act_item->state = 1;
     
            /*Format the value. Assuming `item.value` is set to an array when initialized*/
            sprintf(act_item->value, "%dA", act_item->state);
            
            /*Refresh the item with the new `state` and `value`*/
            lv_settings_refr(act_item);
        }
    }
}
```

## Examples

See the [lv_ex_settings](https://github.com/littlevgl/lv_apps/tree/master/examples/lv_ex_settings) folder for examples.
 
