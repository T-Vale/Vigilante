boolean alarmValue_[MAXIMUMNUMBEROFALARMS];
int alarmPinout_[MAXIMUMNUMBEROFALARMS] = {A0, A1, A2};

unsigned long lastReset = 0;
unsigned long lastAlarmSent = 0;
boolean alarmActive = false;
boolean alarmAccepted = false;

unsigned long alarmStartTime = 0;
unsigned long lastCallTime = 0;

boolean alarmisticState = true;
int alarmTics = 0;
