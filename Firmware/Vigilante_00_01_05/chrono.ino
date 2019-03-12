void chrono_Watcher()
{
  if(chrono_Trigger(previousSystemStatus, millis(), SYSTEMSTATUSINTERVAL))
  {
    sms_Distributor("Occasional News! The Vigilante keeps working! It's been like this for " + String(chrono_Calculator(systemStarting, millis()) / (3600 * 24 * 1000)) + " days. The alarmistic system is " + String(alarmisticState) + ".");
    previousSystemStatus = millis();    
  }
}

boolean chrono_Trigger(unsigned long previousTime, unsigned long actualTime, unsigned long intervalTime)
{
  if(chrono_Calculator(previousTime, actualTime) >= intervalTime)
  {
    return true;
  }
  return false;
}

unsigned long chrono_Calculator(unsigned long previousTime, unsigned long actualTime)
{
  return (actualTime - previousTime);
}

