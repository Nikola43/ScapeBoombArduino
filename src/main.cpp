
/*
http://audio.online-convert.com/convert-to-wav
Go to The Link
Upload your audio you want to convert to WAV
Change bit resolution to " 8bit ".
Change sampling rate to " 16000Hz ".
Change audio channels " mono " .
Click on " Show advanced options ".
PCM format " unsigned 8 bit ".
Convert file.
At next page click on "direct download link"
 */


/*
 CS ---------------------->> 10
SCK ---------------------->> 13
MOSI -------------------->> 11
MISO--------------------->> 12
VCC --------------------->> +5v
GND--------------------->> Arduino's Ground
 */


#include <Arduino.h>
#include <string.h>
#include "SevenSegmentTM1637.h"
#include <SD.h>
#include "TMRpcm.h"
#include <SPI.h>

#define DELAY 1000
#define SD_ChipSelectPin 10

const byte PIN_CLK = 5;
const byte PIN_DIO = 4;

TMRpcm tmrpcm;

SevenSegmentTM1637 display(PIN_CLK, PIN_DIO);
int gameDuration = 3600;

char initMusic[] = "test.wav";

char parseData(unsigned long n, uint8_t base) {
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = n % base;
    n /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);
  return str[0];
}

const char * parseTime(int gameDuration)
{
  static char value[5];
  char m = gameDuration / 60;
  char s = gameDuration % 60;

  char minutes[2] = {0};
  char seconds[2]= {0};

  if (m >= 10)
  {
    minutes[0] = m / 10;
    minutes[1] = m % 10;
  }
  else
  {
    minutes[0] = 0;
    minutes[1] = m % 10;
  }

  if (s >= 10)
  {
    seconds[0] = s / 10;
    seconds[1] = s % 10;
  }
  else
  {
    seconds[0] = 0;
    seconds[1] = s % 10;
  }

  value[0] = parseData(minutes[0],10);
  value[1] = parseData(minutes[1],10);
  value[2] = parseData(seconds[0],10);
  value[3] = parseData(seconds[1],10);
  value[4] = '\0';

  return value;
}

void win(){
  // play win music

  //delay(30000);
  while(true) {

  }
}
void lose(){
  // play win music

  //
  display.print("XXXX");
  display.blink();

  // wait
  //delay(30000);
  while(true) {

  }
}

void checkStatus() {

  // check wire
  if (1 == 0)
  {
    win();
  }


  // check
  if (gameDuration == 0)
  {
    lose();
  }
}


// run setup code
void setup()
{
  Serial.begin(9600);
  display.begin();
  display.setBacklight(100); // set the brightness to 100 %

  tmrpcm.speakerPin=9;
  if(!SD.begin(SD_ChipSelectPin))
  {
    Serial.println("SD fail");
    return;
  }

  tmrpcm.setVolume(6);
  tmrpcm.play(initMusic);
};

void loop()
{
  bool colonOn = false;


  //
  do
  {

    display.print(parseTime(gameDuration));
    display.setColonOn(colonOn);

    colonOn = !colonOn;
    gameDuration--;
    delay(DELAY);


  } while (gameDuration > 0);
};
