void eeprom_Verification()
{
  if (!EEPROM.isValid())
  {
    //encher matriz dos números de telefone com 0's
    //encher flash com 0's nos números
    for (int f = 0; f < MAXIMUMNUMBEROFPHONES; f++)
    {
      EEPROM.write(f * 100, 1);
      EEPROM.write((f * 100) + 1, 48);
    }
    EEPROM.commit();
  }
  else
  {
    eeprom_Loader();
  }
}

void eeprom_Saver(int phone, String &phoneString)
{
  int o = phoneString.length();
  EEPROM.write(phone * 100, o);
  for(int p = 1; p <= o; p++)
  {
    EEPROM.write((phone * 100) + p, char(phoneString[p - 1]));
  }
  EEPROM.commit();
}

void eeprom_Loader()
{
  for (int f = 0; f < MAXIMUMNUMBEROFPHONES; f++)
  {
    String amalgama;
    amalgama.reserve(50);
    amalgama = "";
    int flashLength = EEPROM.read(f * 100);
    for (int o = 1; o <= flashLength; o++)
    {
      char c;
      c = char(EEPROM.read((f * 100) + o));
      amalgama += String(c);
    }
    phoneNumberList_[f] = amalgama;
  }
}
