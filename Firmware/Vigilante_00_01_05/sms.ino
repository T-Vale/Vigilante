void sms_Seeker()
{
  if(sms.available())
  {
    if(sms.remoteNumber(senderNumberArray, SENDERNUMBERSIZE))
    {
      senderNumberString = "";
      messageReceived = "";
      for(int a = 0; a < sizeof(senderNumberArray); a++)
      {
        if(senderNumberArray[a] == 0)
        {
          break;
        }
        senderNumberString = senderNumberString + senderNumberArray[a];
      }
      int c;
      while ((c = sms.read()) != -1 && messageReceived.length() < MESSAGELENGTHLIMIT) 
      {
        if(c >= 32 && c <= 127) //so concatena caracteres legiveis
        {
          messageReceived = messageReceived + ((char)c);
        }
      }
      sms.flush();
      messageReceived.toLowerCase();
      delay(1000);
      sms_Analyser();
    }
  }
}

boolean sms_Analyser()
{
  buff = "";
  messageToSend = "";
  messageToSend = commandReceived;
  sms_Courier(messageToSend, senderNumberString);
  if(messageReceived[messageReceived.length()-1] == endingChar) //tem que ser com '' caso contrário associa a uma string
  {
    if(messageReceived.indexOf("phone") >= 0)
    {
      for(int p = 0; p < MAXIMUMNUMBEROFPHONES; p++)
      {
        elementToSearch = "";
        elementToSearch.concat("phone");
        elementToSearch.concat(String(p));
        elementToSearch.concat(spaceCharStr);
        if(parse_ParameterReader(messageReceived, elementToSearch, buff, endingCharStr))
        {
          if(buff.indexOf("+") < 0 && buff != "0")
          {
            buff = "+351" + buff; //toma por padrao que o numero e portugues //tenho que mudar isto para um concat de alguma forma...
          }
          messageToSend = commandAccepted;
          sms_Courier(messageToSend, senderNumberString);
          messageToSend = "The number ";
          messageToSend.concat(senderNumberString);
          messageToSend.concat(" requested the changing of phone number ");
          messageToSend.concat(String(p));
          messageToSend.concat(" to ");
          messageToSend.concat(buff);
          messageToSend.concat(".");
          sms_Distributor(messageToSend);
          messageToSend = "";
          messageToSend.concat("Your number has been removed from the Vigilante system.");
          sms_Courier(messageToSend, phoneNumberList_[p]);
          phoneNumberList_[p] = buff;
          eeprom_Saver(p, buff);
          messageToSend = "";
          messageToSend.concat("Your number has been added to the Vigilante system. Answer back with \"help;\" to get the commands.");
          sms_Courier(messageToSend, phoneNumberList_[p]);
        }
      }
    }
    else if(messageReceived.indexOf("getnumbers") >= 0)
    {
      messageToSend = commandAccepted;
      sms_Courier(messageToSend, senderNumberString);
      messageToSend = "";
      for(int p = 0; p < MAXIMUMNUMBEROFPHONES; p++)
      {
        messageToSend.concat("Phone");
        messageToSend.concat(String(p));
        messageToSend.concat(": ");
        messageToSend.concat(phoneNumberList_[p]);
        if(p < MAXIMUMNUMBEROFPHONES - 1)
        {
          messageToSend.concat("\n");
        }
      }
      sms_Courier(messageToSend, senderNumberString);
    }
    else if(messageReceived.indexOf("startover") >= 0)
    {
      messageToSend = commandAccepted;
      sms_Courier(messageToSend, senderNumberString);
      digitalWrite(resetPin, LOW); //hard resets Arduino
    }
    else if(messageReceived.indexOf("getstatus") >= 0)
    {
      messageToSend = commandAccepted;
      sms_Courier(messageToSend, senderNumberString);
      alarm_Reader();
      messageToSend = "";
      for(int d = 0; d < MAXIMUMNUMBEROFALARMS; d++)
      {
        messageToSend.concat("Alarm");
        messageToSend.concat(String(d));
        messageToSend.concat(": ");
        messageToSend.concat(alarmValue_[d]);
        messageToSend.concat("\n");
      }
      messageToSend.concat("Alarmistic: ");
      messageToSend.concat(String(alarmisticState));
      messageToSend.concat("\n");
      messageToSend.concat("Signal [0-31]: ");
      messageToSend.concat(String(scanner.getSignalStrength()));
      sms_Courier(messageToSend, senderNumberString);
    }
    else if(messageReceived.indexOf("getsystem") >= 0)
    {
      messageToSend = commandAccepted;
      sms_Courier(messageToSend, senderNumberString);
      messageToSend = vigilanteVersion;
      messageToSend.concat("\n");
      messageToSend.concat("Signal [0-31]: ");
      messageToSend.concat(String(scanner.getSignalStrength()));
      messageToSend.concat("\n");
      messageToSend.concat("Interval between alarm call/sms: ");
      messageToSend.concat(String(CALLTIMEINTERVAL / 1000 / 60));
      messageToSend.concat(" min\n");
      messageToSend.concat("Time to reset: ");
      messageToSend.concat(String((SYSTEMRESTARTINTERVAL - (millis() - systemStarting))/3600000));
      messageToSend.concat(" hours");
      sms_Courier(messageToSend, senderNumberString);
    }
    else if(parse_ParameterReader(messageReceived, searchAlarmistic, buff, endingCharStr))
    {
      messageToSend = commandAccepted;
      sms_Courier(messageToSend, senderNumberString);
      if(buff == "enable")
      {
        alarmisticState = true;
        messageToSend = "The alarmistic was remotely turned ON by ";
        messageToSend.concat(senderNumberString);
        messageToSend.concat(".");
        sms_Distributor(messageToSend);
      }
      else if(buff == "smashed")
      {
        messageToSend = commandAccepted;
        sms_Courier(messageToSend, senderNumberString);
        alarmisticState = false;
        alarmTics = 0;
        messageToSend = "The alarmistic was remotely turned OFF by ";
        messageToSend.concat(senderNumberString);
        messageToSend.concat(". I repeat, OFF! It started ");
        messageToSend.concat(String((chrono_Calculator(alarmStartTime, millis()) / 1000) / 60));
        messageToSend.concat(" min ago. Turn it on again when solved.");
        sms_Distributor(messageToSend);
      }
      else if(buff == "disable")
      {
        messageToSend = commandAccepted;
        sms_Courier(messageToSend, senderNumberString);
        messageToSend = "Does not work. Go there and press the button! Quick";
        sms_Distributor(messageToSend);
      }
    }
    else if(messageReceived.indexOf("help") >= 0)
    {
      messageToSend = commandAccepted;
      sms_Courier(messageToSend, senderNumberString);
      messageToSend = "|Define phone number|\n";
      messageToSend.concat("phone# x;\n");
      messageToSend.concat("#: list Position\n");
      messageToSend.concat("x: phone number (0 to null)\n");
      messageToSend.concat("|Enable alarmistics|\n");
      messageToSend.concat("alarmistic enable;");
      sms_Courier(messageToSend, senderNumberString);
      messageToSend = "|Get alarm status|\n";
      messageToSend.concat("getstatus;\n");
      messageToSend.concat("|Get system status|\n");
      messageToSend.concat("getsystem;\n");
      messageToSend.concat("|Get phone number list|\n");
      messageToSend.concat("getnumbers;");
      sms_Courier(messageToSend, senderNumberString);
    }
    else if(messageReceived.indexOf("spiderman") >= 0)
    {
      messageToSend = commandAccepted;
      sms_Courier(messageToSend, senderNumberString);
      messageToSend = "alarmistic smashed;";
      sms_Courier(messageToSend, senderNumberString);
    }
    else if(messageReceived.indexOf("igc") >= 0)
    {
      messageToSend = commandAccepted;
      sms_Courier(messageToSend, senderNumberString);
      messageToSend = "Quite a nice place, ain't it?";
      sms_Courier(messageToSend, senderNumberString);
    }

  }
}



boolean sms_Distributor(String &message)
{
  for(int p = 0; p < MAXIMUMNUMBEROFPHONES; p++)
  {
    sms_Courier(message, phoneNumberList_[p]);
  }
  return true;
}

boolean sms_Courier(String &message, String &number)
{
  if(number != "0") //0 é número vazio
  {
    char charMessage[message.length() + 1]; //it only accepts character arrays
    char charNumber[number.length() + 1]; //it only accepts character arrays
    message.toCharArray(charMessage, sizeof(charMessage));
    number.toCharArray(charNumber, sizeof(charNumber));
    if(sms.beginSMS(charNumber))
    {
      sms.print(charMessage);
      sms.endSMS();
    }
    else
    {
      return false;
    }
    delay(1000); //so porque sim e me parece bem
    return true;
  }
  else
  {
    return false;
  }
}
