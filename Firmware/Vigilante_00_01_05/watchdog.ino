boolean watchdog_everythingIsNeat()
{
  if(chrono_Trigger(systemStarting, millis(), SYSTEMRESTARTINTERVAL))
  {
    digitalWrite(resetPin, LOW); //hard resets Arduino
    return false;
  }
  return true;
}
