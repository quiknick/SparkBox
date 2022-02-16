void updateIcons() {
  
  // Read RSSI from Spark
  iRSSI = ble_getRSSI();
            
  // Show BT icon if connected
  // Use graduated scale based on the following
  // 0 bars (very poor) < -95db
  // 1 bar (poor) = -75db to -95db
  // 2 bars (fair) = -60db to -75db
  // 3 bars (good) = -40db to -60db
  // 4 bars (excellent) = > -40db
  if(spark_state == SPARK_SYNCED){
    oled.drawXbm(btlogo_pos, 0, bt_width, bt_height, bt_bits);
    // Display BT RSSI icon depending on actual signal
    if (iRSSI > -40) {
      oled.drawXbm(rssi_pos, 0, rssi_width, rssi_height, rssi_4);
    }
    else if (iRSSI > -60) {
      oled.drawXbm(rssi_pos, 0, rssi_width, rssi_height, rssi_3);
    }
    else if (iRSSI > -75) {
      oled.drawXbm(rssi_pos, 0, rssi_width, rssi_height, rssi_2);
    }
     else if (iRSSI > -95) {
      oled.drawXbm(rssi_pos, 0, rssi_width, rssi_height, rssi_1);
    }
    // else no bars 
    
    // Update drive status icons once data available
    // Drive icon
    if (presets[5].effects[2].OnOff){
      oled.drawXbm(drive_pos, 0, icon_width, icon_height, drive_on_bits);
    }
    else {
      oled.drawXbm(drive_pos, 0, icon_width, icon_height, drive_off_bits);   
    }
    // Mod icon
    if (presets[5].effects[4].OnOff) {
      oled.drawXbm(mod_pos, 0, icon_width, icon_height, mod_on_bits);
    }
    else {
       oled.drawXbm(mod_pos, 0, icon_width, icon_height, mod_off_bits);   
    }
    // Delay icon
    if (presets[5].effects[5].OnOff) {
      oled.drawXbm(delay_pos, 0, icon_width, icon_height, delay_on_bits);
    }
    else {
       oled.drawXbm(delay_pos, 0, icon_width, icon_height, delay_off_bits);   
    }
    // Reverb icon
    if (presets[5].effects[6].OnOff) {
      oled.drawXbm(rev_pos, 0, icon_width, icon_height, rev_on_bits);
    }
    else {
       oled.drawXbm(rev_pos, 0, icon_width, icon_height, rev_off_bits);
    }
  }
  // Battery icon control - measured periodically via a 1s timer
  // Average readings to reduce noise
  if (isTimeout) {
    vbat_result = analogRead(VBAT_AIN); // Read battery voltage

    // To speed up the display when a battery is connected from scratch
    // ignore/fudge any readings below the lower threshold
    if (vbat_result < BATTERY_LOW) vbat_result = BATTERY_LOW;
    temp = vbat_result;

    // While collecting data
    if (vbat_ring_count < VBAT_NUM) {
      vbat_ring_sum += vbat_result;
      vbat_ring_count++;
      vbat_result = vbat_ring_sum / vbat_ring_count;
    }
    // Once enough is gathered, do a decimating average
    else {
      vbat_ring_sum *= 0.9;
      vbat_ring_sum += vbat_result;
      vbat_result = vbat_ring_sum / VBAT_NUM;
    }

    chrg_result = analogRead(CHRG_AIN); // Check state of /CHRG output
    isTimeout = false;
  }

  // Start by showing the empty icon. drawXBM writes OR on the screen so care
  // must be taken not to graphically block out some symbols. This is why the
  // battery full but not charging is the last in the chain.

   // No battery monitoring so just show the empty symbol
  #ifdef BATT_CHECK_0
    oled.drawXbm(bat_pos, 0, bat_width, bat_height, bat00_bits);
  #endif
  
  // For level-based charge detection (not very reliable)
  #ifdef BATT_CHECK_1
    if (vbat_result >= BATTERY_CHRG) {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, batcharging_bits);
    }
  #endif
      
  // If advanced charge detection available, and charge detected
  #ifdef BATT_CHECK_2
    if (chrg_result < CHRG_LOW) {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, batcharging_bits);
    }
  #endif
  
  // Basic battery detection available. Coarse cut-offs for visual 
  // guide to remaining capacity. Surprisingly complex feedback to user.
  // No bars = 0% (should not be discharged further)
  // Full symbol = >85%
  #ifndef BATT_CHECK_0
    else if (vbat_result < BATTERY_LOW) {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, bat00_bits);
    }
    else if (vbat_result < BATTERY_10) {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, bat10_bits);
    }
    else if (vbat_result < BATTERY_20) {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, bat20_bits);
    }
    else if (vbat_result < BATTERY_30) {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, bat30_bits);
    }    
    else if (vbat_result < BATTERY_40) {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, bat40_bits);
    }
    else if (vbat_result < BATTERY_50) {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, bat50_bits);
    }
    else if (vbat_result < BATTERY_60) {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, bat60_bits);
    }
    else if (vbat_result < BATTERY_70) {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, bat70_bits);
    }        
    else if (vbat_result < BATTERY_80) {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, bat80_bits);
    }
    else if (vbat_result < BATTERY_90) {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, bat90_bits);
    }
    else {
      oled.drawXbm(bat_pos, 0, bat_width, bat_height, bat100_bits);
    } 
  #endif
}

// Print out the requested preset data
void dump_preset(SparkPreset preset) {
  int i,j;

  Serial.print(preset.curr_preset); Serial.print(" ");
  Serial.print(preset.preset_num); Serial.print(" ");
  Serial.print(preset.Name); Serial.print(" ");
  Serial.println(preset.Description);

  for (j=0; j<7; j++) {
    Serial.print("    ");
    Serial.print(preset.effects[j].EffectName); Serial.print(" ");
    if (preset.effects[j].OnOff == true) Serial.print(" On "); else Serial.print (" Off ");
    for (i = 0; i < preset.effects[j].NumParameters; i++) {
      Serial.print(preset.effects[j].Parameters[i]); Serial.print(" ");
    }
    Serial.println();
  }
}

// Pushbutton handling
void dopushbuttons(void)
{
  // Debounce and long press code
  for (i = 0; i < NUM_SWITCHES; i++) {
    // If the button pin reads HIGH, the button is pressed (VCC)
    if (digitalRead(sw_pin[i]) == HIGH)
    {
      // If button was previously off, mark the button as active, and reset the timer
      if (buttonActive[i] == false){
          buttonActive[i] = true;
          buttonTimer[i] = millis();
      }
       
      // Calculate the button press duration by subtracting the button time from the local time
      buttonPressDuration[i] = millis() - buttonTimer[i];
      
      // Mark the button as long-pressed if the button press duration exceeds the long press threshold
      // and is not already flagged as such
      if ((buttonPressDuration[i] > longPressThreshold) && (longPressActive[i] == false)) {
          longPressActive[i] = true;
          latchpress = true;
      }
    }
    // Else the button reads as LOW, so the button is released which means that  
    // the button either hasn't been pressed, or has just been released
    else {
      // Reset switch register here so that switch is not repeated    
      sw_val[i] = LOW; 
      
      // If the button was marked as active, it was recently pressed
      if (buttonActive[i] == true){
        
        // Reset the long press active state
        if (longPressActive[i] == true){
          longPressActive[i] = false;
          latchpress = false;
        }
        
        // Long press wasn't active. We either need to debounce/reject the press or register a normal/short press
        else {
          // if the button press duration exceeds our bounce threshold, then we register a short press
          if (buttonPressDuration[i] > debounceThreshold){
            sw_val[i] = HIGH;
          }
      }
        
        // Reset the button active status
        buttonActive[i] = false;
      }
      
    }  // The button either hasn't been pressed, or has just been released
    
  }  // Debounce and long press code loop

  // OR all the long press flags so any of the four main footswitches can switch modes
  AnylongPressActive = (longPressActive[0] || longPressActive[1] || longPressActive[2] || longPressActive[3]);
  AllPressActive = (longPressActive[0] && longPressActive[1] && longPressActive[2] && longPressActive[3]);

  // Have all buttons been held down? - toggle tuner mode
  if (AllPressActive && (latchpress == true)){
      Serial.println("Tuner mode");
      if (isTunerMode) {
        spark_msg_out.tuner_on_off(false);
      }
      else {
        spark_msg_out.tuner_on_off(true);
      }/*
      longPressActive[0] = false; // This doesn't work
      longPressActive[1] = false;
      longPressActive[2] = false;
      longPressActive[3] = false;*/
      latchpress = false;
  }
   
  // Has any button been held down?
  if (AnylongPressActive && (latchpress == true)){
      Serial.println("Switching pedal mode");
      isOLEDUpdate = true;
      latchpress = false;
      if (isPedalMode) isPedalMode = false;   // Toggle pedal mode
      else isPedalMode = true;
  }
}

// Refresh UI
void refreshUI(void)
{
    // Flip GUI flash bool
    if (isTimeout){
      flash_GUI = !flash_GUI;
    }
  
    // if a change has been made or the timer timed out and fully synched...
    if ((isOLEDUpdate || isTimeout) && (spark_state == SPARK_SYNCED)){
    isOLEDUpdate = false;  
    oled.clear();

    // Show tuner screen when requested by Spark
    if (isTunerMode) {
      
      // Default display - draw meter bitmap and label
      oled.drawXbm(0, Y5, tuner_width, tuner_height, tuner_bits); 
      oled.setFont(ArialMT_Plain_10);
      oled.setTextAlignment(TEXT_ALIGN_LEFT);
      oled.drawString(0,0,"Tuner");
      
      // If nothing to show
      if (msg.val == -1.0) {
        oled.setTextAlignment(TEXT_ALIGN_RIGHT);
        oled.drawString(127,0,"...");
      }
      // If something to show
      else {
        // Work out start and end-points of meter needle
        metervalue = int16_t(msg.val * 128);            // Span tuner's 0-1.0, to 0-127
        if (metervalue > 127) metervalue = 127;
        if (metervalue < 0) metervalue = 0;
        hubvalue = metervalue / 4;                      // Hub is 1/4 the size, so spans 0-32
        hub_x = hubvalue;

        // Work out the Y-values of the needle (both start and end points)
        // Righ-side calculations
        if (metervalue > 64)
        {
          hub_x = hub_x - 16;                           // Scale hub RHS
          hub_y = sqrt(256 - (hub_x * hub_x));          // Work out Y-value based on 16 pixel radius
          meter_x = metervalue - 64;                    // Scale meter RHS
          meter_y = sqrt(tuner_scale - (meter_x * meter_x));   // Work out Y-value based on 64 pixel radius
          oled.drawLine(hubvalue + 48, 64-hub_y, metervalue, 64-meter_y); // Draw line from hub to meter edge
        }
        // Left-side calculations
        else
        {
          hub_x = 16 - hub_x;
          hub_y = sqrt(256 - (hub_x * hub_x));
          meter_x = 64 - metervalue;
          meter_y = sqrt(tuner_scale - (meter_x * meter_x));        
          oled.drawLine(hubvalue + 48, 64-hub_y, metervalue, 64-meter_y);
        }

        meter_target = msg.param1; // Get note data

        // Show note names
        oled.setFont(ArialMT_Plain_16);
        switch (meter_target) {
        case 0:
          oled.setTextAlignment(TEXT_ALIGN_LEFT);
          //oled.drawString(16, 49, "B");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          oled.drawString(127, 0, "C");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          //oled.drawString(112, 49, "C#");
          break;
        case 1:
          oled.setTextAlignment(TEXT_ALIGN_LEFT);
          //oled.drawString(16, 49, "C");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          oled.drawString(127, 0, "C#");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          //oled.drawString(112, 49, "D");
          break;    
        case 2:
          oled.setTextAlignment(TEXT_ALIGN_LEFT);
          //oled.drawString(16, 49, "C#");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          oled.drawString(127, 0, "D");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          //oled.drawString(112, 49, "D#");
          break;
        case 3:
          oled.setTextAlignment(TEXT_ALIGN_LEFT);
          //oled.drawString(16, 49, "D");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          oled.drawString(127, 0, "D#");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          //oled.drawString(112, 49, "E");
          break;
        case 4:
          oled.setTextAlignment(TEXT_ALIGN_LEFT);
          //oled.drawString(16, 49, "D#");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          oled.drawString(127, 0, "E");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          //oled.drawString(112, 49, "F");
          break;
        case 5:
          oled.setTextAlignment(TEXT_ALIGN_LEFT);
          //oled.drawString(16, 49, "E");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          oled.drawString(127, 0, "F");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          //oled.drawString(112, 49, "F#");
          break;
        case 6:
          oled.setTextAlignment(TEXT_ALIGN_LEFT);
          //oled.drawString(16, 49, "F");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          oled.drawString(127, 0, "F#");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          //oled.drawString(112, 49, "G");
          break;         
        case 7:
          oled.setTextAlignment(TEXT_ALIGN_LEFT);
          //oled.drawString(16, 49, "F#");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          oled.drawString(127, 0, "G");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          //oled.drawString(112, 49, "G#");
          break;
        case 8:
          oled.setTextAlignment(TEXT_ALIGN_LEFT);
          //oled.drawString(16, 49, "G");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          oled.drawString(127, 0, "G#");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          //oled.drawString(112, 49, "A");
          break;
        case 9:
          oled.setTextAlignment(TEXT_ALIGN_LEFT);
          //oled.drawString(16, 49, "G#");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          oled.drawString(127, 0, "A");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          //oled.drawString(112, 49, "A#");
          break;
        case 10:
          oled.setTextAlignment(TEXT_ALIGN_LEFT);
          //oled.drawString(16, 49, "A");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          oled.drawString(127, 0, "A#");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          //oled.drawString(112, 49, "B");
          break;
        case 11:
          oled.setTextAlignment(TEXT_ALIGN_LEFT);
          //oled.drawString(16, 49, "A#");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          oled.drawString(127, 0, "B");
          oled.setTextAlignment(TEXT_ALIGN_RIGHT);
          //oled.drawString(112, 49, "C");
          break;
        default:
          break;
        }
      }     
    }
    
    // Otherwise normal display
    else {
      oled.setFont(ArialMT_Plain_16);
      oled.setTextAlignment(TEXT_ALIGN_LEFT);
      if (!isPedalMode) {
        oled.drawString(0, 20, "Preset mode");
      }
      else {
        oled.drawString(0, 20, "Effect mode");    
      }
      oled.setFont(Roboto_Mono_Bold_52);
      oled.setTextAlignment(TEXT_ALIGN_CENTER);
      
      if (flash_GUI || !setting_modified) {
        // In-joke to the "I saw 5 on the display!" folk
        if (display_preset_num > 3) {
          display_preset_num = 3; 
        }
        oled.drawString(110, 12, String(display_preset_num+1));
      }
      oled.setFont(ArialMT_Plain_10);
      oled.setTextAlignment(TEXT_ALIGN_LEFT);

      // Truncate string so that it never extends into preset number
      strcpy(TempString, presets[5].Name);
      i = STR_LEN;
      while (oled.getStringWidth(TempString) > 90) {
        TempString[i-1]= '\0';  // Rudely truncate
        i--;
      }

      oled.drawString(0, 50, TempString);
  
      // Flash "Connect App" message when no app connected
      if (flash_GUI && !conn_status[APP]){
        oled.setFont(ArialMT_Plain_10);
        oled.setTextAlignment(TEXT_ALIGN_LEFT);
        oled.drawString(15, 37, "Connect App");
      }
    
      updateIcons();
    }
    oled.display();
  }
  
  if (!connected_sp) {
    // Show reconnection message
    oled.clear();
    oled.setFont(ArialMT_Plain_16);
    oled.setTextAlignment(TEXT_ALIGN_CENTER);
    oled.drawString(X1, Y3, "Reconnecting");
    oled.setFont(ArialMT_Plain_16);
    oled.setTextAlignment(TEXT_ALIGN_CENTER);
    oled.drawString(X1, Y4, "Please wait");
    oled.display();
  }
}
