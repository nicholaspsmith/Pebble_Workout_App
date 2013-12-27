//******************************************//
//        Workout App for Pebble            //
//    (hard)coded by Nicholas P Smith       //
//              12/27/2013                  //
//
//  TODO: write instructions
//
//
//
//
//******************************************//


#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0xF2, 0x3C, 0x9A, 0x6A, 0x70, 0xE0, 0x41, 0x2A, 0xA8, 0x39, 0xAE, 0x17, 0x70, 0x40, 0xD1, 0x83 }
PBL_APP_INFO(MY_UUID,
             "Workout App",
             "Nicholas P Smith",
             1,
0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;

TextLayer textLayer;

typedef char * string;

// STRING_LENGTH = number of exercises
const int STRING_LENGTH = 7;
// Have to manually re-enter length here (can't use STRING_LENGTH) because C is stupid
string exercises[7];

string reps[7];

static string disp;


static int i = 0;

// Push select button
void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  static int select = 0;
  if (select == 0 && strlen(reps[i]) > 0) {
    text_layer_set_text(&textLayer, reps[i]);
    select = 1;
  } else if (select == 1) {
    text_layer_set_text(&textLayer, exercises[i]);
    select = 0;
  }
}



void click_config_provider(ClickConfig **config, Window *window) {

  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;

}


void handle_init(AppContextRef ctx) {

  window_init(&window, "Workouts");
  window_stack_push(&window, true /* Animated */);


  //*****  Hard code exercises  *****//

  // So you know where workout begins
  exercises[0] = "Begin!";
  exercises[1] = "Run";
  exercises[2] = "Situps";
  exercises[3] = "Cable Crunches";
  exercises[4] = "Cable Rotation";
  exercises[5] = "Hanging Leg Raises";
  exercises[6] = "End!";
  //*********************************//



  // Hard code reps
  reps[0] = "Get going!";
  reps[1] = "10 min";
  reps[2] = "3x30";
  reps[3] = "4x15";
  reps[4] = "4x12";
  reps[5] = "3x20";
  reps[6] = "You're done!";


  text_layer_init(&textLayer, GRect(0, 65, 144, 30));
  text_layer_set_text_alignment(&textLayer, GTextAlignmentCenter);
  text_layer_set_text(&textLayer, "Workouts:");
  text_layer_set_font(&textLayer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  layer_add_child(&window.layer, &textLayer.layer);
  window_set_click_config_provider(&window, (ClickConfigProvider) click_config_provider);
}
void handle_button_down(AppContextRef ctx, PebbleButtonEvent *event) {
  (void)ctx;
  switch (event->button_id) {
    default: break;
    case BUTTON_ID_UP:
      if (i == 0){
        i = STRING_LENGTH-1;
      } else {
        i = (i - 1) % STRING_LENGTH;
      }
      disp = exercises[i];
    break;
    case BUTTON_ID_SELECT:
      disp = reps[i];
    break;
    case BUTTON_ID_DOWN:
      i = (i + 1) % STRING_LENGTH;
      disp = exercises[i];
    break;
  }
  // Update the layer
  text_layer_set_text(&textLayer, disp);
}

void handle_button_up(AppContextRef ctx, PebbleButtonEvent *event) {
  (void)ctx;
  switch (event->button_id) {
    default: break;
    case BUTTON_ID_SELECT:
      disp = exercises[i];
      break;
  }
  // Update the layer
  text_layer_set_text(&textLayer, disp);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .input_handlers = {
      .buttons = {
        .down = &handle_button_down,
        .up = &handle_button_up
      }
    }
  };
  app_event_loop(params, &handlers);
}
