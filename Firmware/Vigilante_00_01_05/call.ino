boolean call_ShotCaller()
{ 
  for(int p = 0; p < MAXIMUMNUMBEROFPHONES; p++)
  {
    if(phoneNumberTics_[p] < alarmTics)
    {
      call_Operator(phoneNumberList_[p]);
    }
  }
  return true;
}

boolean call_Operator(String number)
{
  if(number != "0") //0 é número vazio
  {
    char charNumber[number.length() + 1]; //it only accepts character arrays
    number.toCharArray(charNumber, sizeof(charNumber));
    if(voice.getvoiceCallStatus() != IDLE_CALL)
    {
      delay(5000);
    }
    if(voice.voiceCall(charNumber))
    {
      // Wait for some input from the line
      unsigned long callStartTime = millis();
      while(voice.getvoiceCallStatus() == CALLING)
      {
        if(millis() - callStartTime > CALLTIMELIMIT)
        {
          return false;
        }
      }
      voice.hangCall();
      return true;
    }
  }
  else
  {
    return false;
  }
}
