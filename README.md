# SparkBox V1.00
SparkBox is a BT pedal for the Positive Grid Spark 40.  Now with expression pedal input and a web interface for adding up to 12 banks for four presets. 

![alt text](https://github.com/happyhappysundays/SparkBox/blob/main/Pictures/SparkBox_final.jpg?raw=true)

# Functions
- Expression pedal input on GPIO34 for altering the current parameter or on/off switch
- Select 12 banks for four presets from the pedal.
- Uses BLE so that it can be used with music function of the Spark app
- Allows connection of the app for full simultaneous control
- Supports the most common DIY display types: SSD1306 and SH1106
- Switch presets either on footswitch, app or Spark to update display
- Switch on and off all four major effects dynamically
- Graphically display the effect state on the display
- Supports 4-button pedals
- Battery level indicator on UI
- Inter-operable with both conventional and Heltec ESP32 modules. Also tested on WEMOS LOLIN32 Lite with battery support.
- Stand-by mode added to reduce power when disconnected - light or deep ESP32 sleep mode will be configured automatically depending on GPIOs of the buttons and logical levels choosen in your build.

# Pedal functions
- Long press (more than 1s) BUTTON 1 to switch between Effect mode and Preset mode
- Long press BUTTON 3 to adjust current effect parameters: use BUTTONS 2 and 4 to decrement/increment, BUTTON 1 to cycle thru parameters and long press BUTTON 3 to save your edits back to the amp.
- Now with remote guitar TUNER display! Long press BUTTONS 1 and 2 simultaneously, or turn it ON from the app or on the amp.
- Bypass mode, invoked by long pressing BUTTONS 3 and 4 simultaneously, allows adjusting effect levels to match your raw pickup output. 
- Holding BUTTON 1 during boot will switch the pedal to WiFi mode.

# Preset bank functions
- Long pressing BUTTON 2 or BUTTON 4 invokes Bank Select menu, continue using BUTTONS 2 and 4 to decrease/increase bank number. If you stop scrolling for a few seconds, the selected bank will be uploaded to the amp.
- On the first run your current set of hardware presets will be saved to the bank 000.
- Added WiFi support w/file manager. Holding BUTTON 1 during boot will switch the pedal to WiFi mode.
- Initially the pedal will launch a WiFi Access Point (AP), SSID is "SparkBox" by default.
- Connect to this WiFi, using your mobile or PC or whatever.
- Direct your browser to http://192.168.4.1
- Submit your local WiFi network credentials (SSID and password), so the pedal could connect to your home WiFi.
- If everything is done correctly, holding BUTTON 1 on boot again will connect your pedal to your home WiFi network, and the OLED display on the pedal will show the address of the filemanager site, so you can access the pedal from any device connected to your local wireless network.
- Use the filemanager to upload your tone presets to the banks folders, 4 presets per folder.
- Format of *.json preset files is the same as backed up files, used by the Spark app (usually you can find them zipped in your Dropbox).

![alt text](https://github.com/happyhappysundays/SparkBox/blob/main/Pictures/web-file-manager.png?raw=true)

# Arduino libraries and board versions

Note: Currently there may be a problem with LittleFS compiling on the Arduino IDE 2.x. No issues on 1.x.

Under Files->Preferences->Additional Boards Manager URLs, enter the following:
- https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
- https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series/releases/download/0.0.5/package_heltec_esp32_index.json

Under Tools->Board->Board manager ensure that you have the following version:
- Heltec ESP32 Dev-Boards 0.0.5 (Heltec - WiFi Kit 32) **OR**
- ESP32 by Espressif 2.0.2 (ESP32)

Under Tools->Manage Libraries ensure that you have the following libraries and versions:
- ThingPulse SSD1306 driver 4.3.0 (ESP32)
- NimBLE-Arduino 1.4.1
- ArduinoJson 6.19.4
- Heltec ESP 1.1.1 (only for Heltec boards)
For Heltec WiFi v3 boards do not install ThingPulse SSD1306 driver 4.3.0 (ESP32)

The program won't fit into a standard APP partition. The cure is easy though: 
- in Arduino IDE choose Tools->Partition Scheme->No-OTA(Large APP), or something that gives you around 2MB APP partition along with enough (also 2MB) of SPIFFS space, cause presets are stored there. 
- Note, that some boards in Arduino IDE don't have Partition Scheme settings, in this case it's recommended to choose some other ESP32 board (ESP Dev Module, Heltec WiFi Kit 32, WEMOS LOLIN32, etc.) which has this menu.

# Compile options (in config.h)

- **#define CLASSIC**

Uncomment this to use with Android devices that are happier with classic BT code.

- **#define BATT_CHECK_0**

You have no mods to monitor the battery, so it will show empty (default).

- **#define BATT_CHECK_1**

You are monitoring the battery via a 2:1 10k/10k resistive divider to GPIO23.
You can see an accurate representation of the remaining battery charge and a kinda-sorta
indicator of when the battery is charging. Maybe.

- **#define BATT_CHECK_2**

You have the battery monitor mod described above AND you have a connection between the 
CHRG pin of the charger chip and GPIO 33. Go you! Now you have a guaranteed charge indicator too.

- **#define EXPRESSION_PEDAL**

Expression pedal define. Comment this out if you DO NOT have the expression pedal mod.

- **#define DUMP_ON**

Dump preset define. Comment out if you'd prefer to not see so much text output

- **#define CLASSIC**

Uncomment for better Bluetooth compatibility with Android devices

- **#define HELTEC_WIFI**

Uncomment when using a Heltec module as their implementation doesn't support setMTU()

- **#define HELTEC_SCREEN** OR **#define SSD1306** OR **#define SH1106**

Choose and uncomment the type of OLED display that you use: 0.96" Heltec builtin or SSD1306 or 1.3" SH1106 

- **#define TWOCOLOUR**

Uncomment if two-colour OLED screens are used. Offsets some text and shows an alternate tuner

- **#define STALE_NUMBER**

Uncomment if you want preset number to scroll together with the name, otherwise it'll be locked in place

- **#define NOSLEEP**

Uncomment if you'd prefer not to use the power-saving sleep modes

- **#define RETURN_TO_MASTER**

When adjusting the level of effects, always start with Master level settings. Comment this line out if you like it to remember your last choice

- **#define ACTIVE_HIGH**

Comment out if your buttons connect to the GND rather than to VCC, this will engage internal pullup and sleep routines also. If you make a decision on the build right now, it's recommended to connect buttons to VCC and to use ACTIVE_HIGH directive.

- **#define NUM_SWITCHES 4**

How many switches do we have

- **#define ALTERNATE_CHARGE_AIN**

Use IO35 instead of IO33. Just for Paul.

- **uint8_t switchPins[]{25,26,27,14};**

GPIOs of the buttons in your setup in the form of switchPins[]{GPIO_for_button1, GPIO_for_button2, GPIO_for_button3, GPIO_for_button4, ... }. Note that GPIOs 25,26,27 and 14 are recommended ones if you want to get the least battery drain in the stand-by mode. 

- **#define ANIMATION_1 or ANIMATION_2**

Until one or the other is voted as a winner you can choose between two animations at startup.

- **#define LEDS_USED**

Option visual aid to see which tone has been selected. Lights one of four LEDs on pins defined by ledPins[].
 
- **const uint8_t ledPins[]{46,45,47,48};**

Specify LED Pins

# Heltec module version
![alt text](https://github.com/happyhappysundays/SparkBox/blob/main/Pictures/Dev_board.jpg?raw=true)
![alt text](https://github.com/happyhappysundays/SparkBox/blob/main/Pictures/Charge_detect.jpg?raw=true)
![alt text](https://github.com/happyhappysundays/SparkBox/blob/main/Pictures/SparkBox_Heltec_Exp_2.png?raw=true)

# ESP32 version
![alt text](https://github.com/happyhappysundays/SparkBox_old/blob/main/Pictures/thumbnail_IMG_6791.jpg?raw=true)
![alt text](https://github.com/happyhappysundays/SparkBox_old/blob/main/Pictures/thumbnail_IMG_6785.jpg?raw=true)
![alt text](https://github.com/happyhappysundays/SparkBox_old/blob/main/Pictures/thumbnail_IMG_6786.jpg?raw=true)
![alt text](https://github.com/happyhappysundays/SparkBox_old/blob/main/Pictures/thumbnail_IMG_6994.jpg?raw=true)
![alt text](https://github.com/happyhappysundays/SparkBox/blob/main/Pictures/meter_during.jpg?raw=true)
![alt text](https://github.com/happyhappysundays/SparkBox/blob/main/Pictures/thumbnail_IMG_7475.jpg?raw=true)
![alt text](https://github.com/happyhappysundays/SparkBox_old/blob/main/Pictures/V0_4.jpg?raw=true)
![alt text](https://github.com/happyhappysundays/SparkBox/blob/main/Pictures/SparkBox_final.jpg?raw=true)
![alt text](https://github.com/happyhappysundays/SparkBox_old/blob/main/Pictures/SparkBox_Heltec_Exp.png?raw=true)
![alt text](https://github.com/happyhappysundays/SparkBox/blob/main/Pictures/SparkBox_Battery.png?raw=true)

# ESP32 pedal parts list

| Item | Description           | Link               |
| -----| ----------------------|--------------------|
|   1  | Box                   |https://www.aliexpress.com/item/32693268669.html?spm=a2g0s.9042311.0.0.27424c4dlzGiUH
|   2  | Stomp switch          |https://www.aliexpress.com/item/32918205335.html?spm=a2g0s.9042311.0.0.27424c4dszp4Ie
|   3  | ESP-WROOM-32U module  |https://www.aliexpress.com/item/32864722159.html?spm=a2g0s.9042311.0.0.27424c4dlzGiUH
|   4  | LCD screen            |https://www.ebay.com.au/itm/333085424031 (small) or https://www.aliexpress.com/item/32950307344.html (large)
|   5  | BT antenna            |https://www.aliexpress.com/item/4001054693109.html?spm=a2g0s.9042311.0.0.27424c4dlzGiUH and https://www.ebay.com.au/itm/233962468558
|   6  | USB extension         |https://www.aliexpress.com/item/32808991941.html?spm=a2g0s.9042311.0.0.27424c4dlzGiUH
|   7  | Power switch          |https://www.jaycar.com.au/dpdt-miniature-toggle-switch-solder-tag/p/ST0355
|   8  | DC input jack         |https://www.jaycar.com.au/2-5mm-bulkhead-male-dc-power-connector/p/PS0524
|   9  | Pedal jack            |https://www.jaycar.com.au/6-5mm-stereo-enclosed-insulated-switched-socket/p/PS0184
|  10  | LiPo battery          |https://www.ebay.com.au/itm/133708965813
|  11  | LiPo charger          |https://www.ebay.com.au/itm/161821599467
|  12  | LiPo booster          |https://www.jaycar.com.au/arduino-compatible-5v-dc-to-dc-converter-module/p/XC4512
|  13  | 9V to 5V converter    |https://www.ebay.com.au/itm/303839459634
|  14  | Glass window (opt)    |https://www.aliexpress.com/item/4000377316108.html?spm=a2g0s.12269583.0.0.1a1e62440DlgU2
