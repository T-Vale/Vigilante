void pininout_Initialiser()
{
  pinMode(resetPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  for(int d = 0; d < MAXIMUMNUMBEROFALARMS; d++)
  {
    pinMode(alarmPinout_[d], INPUT_PULLUP);
  }
  pinMode(localButtonPinout, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}


void pininout_localButtonKeeper()
{
  if (!digitalRead(localButtonPinout))
  {
    alarm_LocalDeactivation();
  }
}
