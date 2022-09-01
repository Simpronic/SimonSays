/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "ssd1306.h"
#include "stdio.h"
#include "image.c"

#define BUZZER PAL_LINE(GPIOA,9)
#define BUFF_SIZE   20
char buff[BUFF_SIZE];

static const I2CConfig i2ccfg = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2,
};

static const SSD1306Config ssd1306cfg = {
  &I2CD1,
  &i2ccfg,
  SSD1306_SAD_0X78,
};

static SSD1306Driver SSD1306D1;

static THD_WORKING_AREA(waOledDisplay, 512);
static THD_FUNCTION(OledDisplay, arg) {
  (void)arg;

  chRegSetThreadName("OledDisplay");
  int x = 0;
  int y = 0;
  int c = 0;



  /*
   * Initialize SSD1306 Display Driver Object.
   */
  ssd1306ObjectInit(&SSD1306D1);

  /*
   * Start the SSD1306 Display Driver Object with
   * configuration.
   */
  ssd1306Start(&SSD1306D1, &ssd1306cfg);

  ssd1306FillScreen(&SSD1306D1, 0x00);

  for(y = 0; y<SSD1306_HEIGHT; y++){
    for(x = 0; x<SSD1306_WIDTH; x++){
      ssd1306DrawPixel(&SSD1306D1, x, y, image_data[c]);
      c++;
      }
    ssd1306UpdateScreen(&SSD1306D1);
  }
  ssd1306UpdateScreen(&SSD1306D1);
  chThdSleepMilliseconds(1500);
  ssd1306FillScreen(&SSD1306D1, SSD1306_COLOR_BLACK);


  while (true) {

    ssd1306GotoXy(&SSD1306D1, 0, 1);
    chsnprintf(buff, BUFF_SIZE, "Welcome to");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);

    ssd1306GotoXy(&SSD1306D1, 0, 20);
    chsnprintf(buff, BUFF_SIZE, "Simon Says");
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_BLACK);

    ssd1306UpdateScreen(&SSD1306D1);
    chThdSleepMilliseconds(500);
  }

}

static THD_WORKING_AREA(waBuzzerThread1, 128);
static THD_FUNCTION(BuzzerThread, arg) {
  sysinterval_t beep_interval = *(sysinterval_t*) arg;
  chRegSetThreadName("Buzzer Beep");
  palSetLineMode(BUZZER,PAL_MODE_OUTPUT_PUSHPULL);
  while(true){
    palToggleLine(BUZZER);
    chThdSleepMilliseconds(beep_interval);
  }
}

int main(void) {

  halInit();
  chSysInit();

  /* Configuring I2C related PINs */
   palSetLineMode(LINE_ARD_D15, PAL_MODE_ALTERNATE(4) |
                  PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST |
                  PAL_STM32_PUPDR_PULLUP);
   palSetLineMode(LINE_ARD_D14, PAL_MODE_ALTERNATE(4) |
                  PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST |
                  PAL_STM32_PUPDR_PULLUP);

  chThdCreateStatic(waOledDisplay, sizeof(waOledDisplay), NORMALPRIO, OledDisplay, NULL);

  sysinterval_t BeepDuration = 1000;
  chThdCreateStatic(waBuzzerThread1, sizeof(waBuzzerThread1), NORMALPRIO + 1, BuzzerThread, &BeepDuration);

 /* Spegnimento schermo, per farlo premere il tasto user sulla nucleo */
  while (true) {
    uint32_t flag = 0;

    if(palReadLine(LINE_BUTTON) == PAL_LOW) {
      while(palReadLine(LINE_BUTTON) == PAL_LOW){
        chThdSleepMilliseconds(20);
      }
      flag = 1;
    }

    if (flag == 1) {
      uint8_t txbuf[] = { 0x00, 0xAE };
         i2cAcquireBus(&I2CD1);
         i2cStart(&I2CD1, &i2ccfg);
         i2cMasterTransmitTimeout(&I2CD1, SSD1306_SAD_0X78, txbuf, sizeof(txbuf), NULL, 0, TIME_INFINITE);
         i2cReleaseBus(&I2CD1);
    }

    chThdSleepMilliseconds(50);
  }
}
