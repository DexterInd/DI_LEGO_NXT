#pragma systemFile            // this eliminates warning for "unreferenced" functions

//***********************************************************************
//******************  Utility Functions for Serial Parsing **************
//***********************************************************************
bool isRealChar(char byteToCheck)
{
  //Function to check if I'm getting a real ASCII displayable character
  if(byteToCheck >= 0x20 && byteToCheck <= 0x7E) return true;
  return false;
}

#if defined(Arduino) || defined(IFI)
void getRFIDTag(string &sComplete, tSensors RFIDEnable, TUARTs currentUart)
{
  SetSensorType(RFIDEnable,sensorDigitalOut);

  char incomingChar = 0;
  ubyte badCharCount = 0;

  //Clear Incoming String
  sComplete = "";

  while(getChar(currentUart) != -1); //Flush Serial Buffer

  //Initially enable the RFID reader
  SensorValue[RFIDEnable] = 0;

  //Loop to wait for the first character
  while(true)
  {
    //See if there anything yet.
    incomingChar = getChar(currentUart);

    //Once I get a real byte, disable the RFID reader.
    if(incomingChar != -1)
    {
      SensorValue[RFIDEnable] = 1;  //Disable RFID
      break;
		}
  }

  //Check to see if we got an escape character, or a real one.
  //If real, add to our string.
  if(isRealChar(incomingChar)) sComplete = incomingChar;

  //Kludgy Loop to Wait Until I get all of the data.
  while(badCharCount < 20)
  {
    //Grab next character
    incomingChar = getChar(currentUart);

    //Check to see if it's real data - If not, increase "bad" counter.
    if(isRealChar(incomingChar))
    {
      //If it is real data, then add to my string.
      sComplete = sComplete + incomingChar;
      wait1Msec(5);
		}
		else
		{
		  badCharCount++;
		}
  }
}

void parseStringToSerial(string sIncoming, TUARTs currentUart)
{
  while(strlen(sIncoming))
  {
    //Kludge to Convert String to Char
    sendChar(currentUart, (char) sIncoming);
    StringDelete(sIncoming, 0, 1);
  }
}
#endif

//***********************************************************************
//******************  Math/String Utilities                **************
//***********************************************************************
float RC_pow(long x, byte p)
{
  float result = x;
  if(p == 0) return 1;
  if(x > 0 && p < 0) return pow(x, p);
  if(x < 0 && p < 0) return (pow(abs(x), p)) * -1;
  for(ubyte count = 0; count < p-1; count++)
  {
    result *= x;
  }
  return result;
}

int RC_atoi(string IncomingData)
{
	const ubyte charToInt = 48;
  int num = 0;
  int sign = 1;
	int digits = strlen(IncomingData);
	long magnitude = RC_pow(10, digits-1);
	if (StringFind(IncomingData, "-") == 0) {
	  PlaySound(soundLowBuzzShort);
	  sign	= -1;
	  digits--;
	  StringDelete(IncomingData, 0, 1);
	}

	for (int i = (digits - 1); i >= 0; i--)
	{
	  num += ((char)IncomingData - charToInt) * magnitude;
		magnitude /= 10;
		StringDelete(IncomingData, 0, 1);
	}
	return num * sign;
}

// to be done
int RC_atoix(string IncomingData)
{
	const ubyte charToInt = 48;
  int num = 0;
  int tmpNum = 0;
  int sign = 1;
	int digits = strlen(IncomingData);

	if (StringFind(IncomingData, "-") == 0) {
	  sign	= -1;
	  digits--;
	  StringDelete(IncomingData, 0, 1);
	}

	long magnitude = RC_pow(10, digits-1);

	for (int i = (digits - 1); i >= 0; i--)
	{
	  tmpNum = ((char)IncomingData - charToInt) * magnitude;
	  if (tmpNum > -1 || tmpNum < 10) {
	    num += tmpNum;
			magnitude /= 10;
		}
		StringDelete(IncomingData, 0, 1);
	}
	return num * sign;
}

long RC_atol(string IncomingData)
{
	const ubyte charToInt = 48;
  long num = 0;
	int digits = strlen(IncomingData);
	long magnitude = RC_pow(10, digits-1);

	for (int i = (digits - 1); i >= 0; i--)
	{
	  num += ((char)IncomingData - charToInt) * magnitude;
		magnitude /= 10;
		StringDelete(IncomingData, 0, 1);
	}
	return num;
}

float RC_atof(string IncomingData)
{
  const string delimiter  = ".";
  ubyte delimiterPosition;
  ubyte lengthOfString;
  float num               = 0.0;
  long  lIntegral         = 0;
  long  lFractional       = 0;
  string sIntegral        = "";
  string sFractional      = "";
  float fFractional       = 0.0;

  sIntegral   = IncomingData;
  sFractional = IncomingData;

  delimiterPosition = StringFind(IncomingData, delimiter);
  lengthOfString    = strlen(IncomingData);

  StringDelete(sIntegral, delimiterPosition, (lengthOfString - delimiterPosition));
  StringDelete(sFractional, 0, (delimiterPosition + 1));

  lIntegral   = RC_atol(sIntegral);
  lFractional = RC_atol(sFractional);

  num = (float)lIntegral;

  fFractional = (float) lFractional;
  fFractional /= RC_pow(10, strlen(sFractional));

  num += fFractional;

  return num;
}

float RC_tan(float x)
{
  float result = sin(x) / cos(x);
  return result;
}

float RC_tanDegrees(float x)
{
  float result = sinDegrees(x) / cosDegrees(x);
  return result;
}

//***********************************************************************
//******************  Parameter Parsing Utilities (Under Construction) **
//***********************************************************************

long LongDelStrWith3Parms(string sourceString, char Delimiter, ubyte Parameter)
{
  if(Parameter == 1)
  {
    ubyte DelimiterPos = StringFind(sourceString, (string) Delimiter);
    StringDelete(sourceString, DelimiterPos ,(strlen(sourceString) - DelimiterPos));
    return RC_atoi(sourceString);
  }
  else if(Parameter == 2)
  {
    StringDelete(sourceString, 0, StringFind(sourceString, (string) Delimiter) + 1);
    ubyte DelimiterPos = StringFind(sourceString, (string) Delimiter);
    StringDelete(sourceString, DelimiterPos ,(strlen(sourceString) - DelimiterPos));
    return RC_atoi(sourceString);
  }
  else if(Parameter == 3)
  {
    StringDelete(sourceString, 0, StringFind(sourceString, (string) Delimiter) + 1);
    StringDelete(sourceString, 0, StringFind(sourceString, (string) Delimiter) + 1);
    return RC_atoi(sourceString);
  }
  else
  {
    return 0;
  }
}

/*Currently figuring out how to supress warning messages
char CharDelStrWith3Parms(string sourceString, char Delimiter, ubyte Parameter)
{
  if(Parameter == 1)
  {
    ubyte DelimiterPos = StringFind(sourceString, (string) Delimiter);
    StringDelete(sourceString, DelimiterPos ,(strlen(sourceString) - DelimiterPos));
    return sourceString;
  }
  else if(Parameter == 2)
  {
    StringDelete(sourceString, 0, StringFind(sourceString, (string) Delimiter) + 1);
    ubyte DelimiterPos = StringFind(sourceString, (string) Delimiter);
    StringDelete(sourceString, DelimiterPos ,(strlen(sourceString) - DelimiterPos));
    return sourceString;
  }
  else if(Parameter == 3)
  {
    StringDelete(sourceString, 0, StringFind(sourceString, (string) Delimiter) + 1);
    StringDelete(sourceString, 0, StringFind(sourceString, (string) Delimiter) + 1);
    return sourceString;
  }
  else
  {
    return 0;
  }
}
*/
