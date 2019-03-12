#include <FlashAsEEPROM.h>
#include <MKRGSM.h>
#include "_wardrobe.h"
#include "_constants.h"
#include "_var_alarm.h"
#include "_var_chrono.h"
#include "_var_phone.h"
#include "_var_string.h"

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;
GSMVoiceCall voice;
GSMScanner scanner;

int resetPin = 0;

void setup()
{
  pininout_Initialiser();
  while(gsmAccess.begin(PINNUMBER) != GSM_READY)
  {
    delay(1000);
  }
  digitalWrite(LED_BUILTIN, LOW);
  senderNumberString.reserve(SENDERNUMBERSIZE);
  buff.reserve(SMSMAXIMUMSIZE);
  messageToSend.reserve(SMSMAXIMUMSIZE);
  elementToSearch.reserve(ELEMENTTOSEARCHMAXSIZE);
  endingCharStr.reserve(endingCharStr.length());
  spaceCharStr.reserve(spaceCharStr.length());
  searchAlarmistic.reserve(searchAlarmistic.length());
  for(int p = 0; p < MAXIMUMNUMBEROFPHONES; p++)
  {
    phoneNumberList_[p].reserve(PHONENUMBERMAXIMUMSIZE);
  }
  
  eeprom_Verification();
  messageToSend = "The Vigilante System has been initialised";
  sms_Distributor(messageToSend);
  messageToSend = "";
}

void loop()
{
  if(watchdog_everythingIsNeat()) //restarts the Arduino every x days
  {
    sms_Seeker();
    alarm_Guardian();
    chrono_Watcher();
  }
}

