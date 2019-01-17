#include <stdio.h>
#include <stdlib.h>
#include "system.h"
#include "tinygl.h"
#include "navswitch.h"
#include "led.h"
#include "task.h"
#include "../../fonts/font3x5_1.h"

#define DISPLAY_RATE 400
#define BUTTON_RATE 100
#define COUNT_RATE 1

static int count = 0;
static bool running = false;
// static int led_state = 0;
static int secs = 0;


static void display_task(__unused__ void *data)
{
    char str[3];
    str[0] = ((count / 10) % 10) + '0';
    str[1] = (count % 10) + '0';
    str[2] = 0;

    tinygl_text(str);
    tinygl_update();
}



static void button_task(__unused__ void *data)
{
    if (!running) {
        navswitch_update();
        if (navswitch_push_event_p(NAVSWITCH_WEST) && count < 99) {
            count++;
        } else if (navswitch_push_event_p(NAVSWITCH_EAST) && count > 0) {
            count--;
        }

        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            running = true;
        }
    }
}



static void count_task(__unused__ void *data)
{
    secs++;
    if (secs >= 60 && running) {
        count--;
        secs = 0;
        led_set(LED1, true);
    }

    if (count <= 0) {
        running = false;
    }
}



int main(void)
{
    count = 0;
    task_t tasks[] =
    {
        {.func = count_task, .period = TASK_RATE / COUNT_RATE, .data = 0},
        {.func = display_task, .period = TASK_RATE / DISPLAY_RATE, .data = 0},
        {.func = button_task, .period = TASK_RATE / BUTTON_RATE, .data = 0}
    };


    system_init();
    navswitch_init();
    led_init();
    led_set(LED1, false);
    tinygl_init(DISPLAY_RATE);
    tinygl_font_set(&font3x5_1);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_mode_set(TINYGL_TEXT_MODE_STEP);


    task_schedule(tasks, ARRAY_SIZE (tasks));
    return 0;
}
