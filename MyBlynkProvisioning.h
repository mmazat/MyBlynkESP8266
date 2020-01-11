/**************************************************************
 * This is a DEMO. You can use it only for development and testing.
 *
 * If you would like to add these features to your product,
 * please contact Blynk for Business:
 *
 *                  http://www.blynk.io/
 *
 **************************************************************/
 
 //#define USE_SPARKFUN_BLYNK_BOARD    // Uncomment the board you are using
//#define USE_NODE_MCU_BOARD        // Comment out the boards you are not using
//#define USE_WITTY_CLOUD_BOARD
#define USE_CUSTOM_BOARD          // For all other ESP8266-based boards -
#define APP_DEBUG        // Comment this out to disable debug prints
//#define BLYNK_SSL_USE_LETSENCRYPT   // Comment this out, if using public Blynk Cloud
#define BLYNK_PRINT Serial

#define PRODUCT_WIFI_SSID             "IOT"         // Name of the device, to be displayed during configuration. Should match export request info.



extern "C" {
  #include "user_interface.h"
}

#include "Settings.h"
#include <BlynkSimpleEsp8266_SSL.h>
#include "BlynkState.h"
#include "ConfigStore.h"
#include "ResetButton.h"
#include "ConfigMode.h"
#include "Indicator.h"
#include "OTA.h"
#include "ESP8266WiFi.h"
#include <WidgetRTC.h>



inline
void BlynkState::set(State m) {
  if (state != m && m < MODE_MAX_VALUE) {
    DEBUG_PRINT(String(StateStr[state]) + " => " + StateStr[m]);
    state = m;
  }
}

class Provisioning {

public:
  void begin()
  {
    DEBUG_PRINT("");
    DEBUG_PRINT("Hardware v" + String(BOARD_HARDWARE_VERSION));
    DEBUG_PRINT("Firmware v" + String(BOARD_FIRMWARE_VERSION));

    indicator_init();
    button_init();
    config_init();

    if (configStore.flagConfig) {
      BlynkState::set(MODE_CONNECTING_NET);
    } else {
      BlynkState::set(MODE_WAIT_CONFIG);
    }
  }

  void run() {
    switch (BlynkState::get()) {
    case MODE_WAIT_CONFIG:       
    case MODE_CONFIGURING:       enterConfigMode();    break;
    case MODE_CONNECTING_NET:    enterConnectNet();    break;
    case MODE_CONNECTING_CLOUD:  enterConnectCloud();  break;
    case MODE_RUNNING:           Blynk.run();          break;
    case MODE_OTA_UPGRADE:       enterOTA();           break;
    case MODE_SWITCH_TO_STA:     enterSwitchToSTA();   break;
    case MODE_RESET_CONFIG:      enterResetConfig();   break;
    default:                     enterError();         break;
    }
  }

};

Provisioning BlynkProvisioning;
