void alarm_LocalDeactivation()
{
  alarmisticState = false;
  alarmTics = 0;
  messageToSend = "The alarmistic was localy turned OFF. I repeat, OFF! Turn it on again when solved";
  sms_Distributor(messageToSend);
}

boolean alarm_Reader()
{
  for(int d = 0; d < MAXIMUMNUMBEROFALARMS; d++)
  {
    alarmValue_[d] = !(digitalRead(alarmPinout_[d]));
  }
  return true;
}

boolean alarm_Verifier()
{
  alarm_Reader();
  boolean alert = false;
  for(int d = 0; d < MAXIMUMNUMBEROFALARMS; d++)
  {
    alert += alarmValue_[d];
  }
  return alert;
}

boolean alarm_Guardian()
{
  if(alarmisticState)
  {
    if(alarm_Verifier() && (alarmTics <= 0)) //e que tal && alarmTicks == 0;?
    {
      lastCallTime = millis();
      alarmStartTime = millis();
      alarmTics++;
      messageToSend = "";
      messageToSend.concat("ALERT! Vigilante's alarm was set ON! Take your measures!\nStarted ");
      messageToSend.concat(String((chrono_Calculator(alarmStartTime, millis()) / 1000) / 60));
      messageToSend.concat(" min ago.\nRound ");
      messageToSend.concat(String(alarmTics));
      sms_Distributor(messageToSend);
      call_ShotCaller();
    }
    else if(alarmTics > 0 && (chrono_Trigger(lastCallTime, millis(), CALLTIMEINTERVAL)))
    {
      alarmTics++;
      lastCallTime = millis();
      sms_Distributor("ALERT! Vigilante's alarm was set ON! Take your measures!\nStarted " + String((chrono_Calculator(alarmStartTime, millis()) / 1000) / 60) + " min ago.\nRound " + String(alarmTics));
      call_ShotCaller();
      delay(1000); //a ligação serie cai se nao tiver isto...
    }  
    pininout_localButtonKeeper(); //só funciona quando o alarme está activo
  }
}
