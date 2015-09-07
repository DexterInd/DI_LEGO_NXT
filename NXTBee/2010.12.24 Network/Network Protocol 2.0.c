////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Basic XBee Communication
// Based off 485 example code from ROBOTC
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma platform(NXT)

void setupHighSpeedLink(const bool bMaster)
{
	// Initialize port S$ to "high speed" mode.
	nxtEnableHSPort();
	int i = nxtSetHSBaudRate(9600);
	nxtHS_Mode = hsMsgModeMaster;
	return;
}

const int BYTETYPE_NOTSET = -1;
const int BYTETYPE_ASCII = 0;
const int BYTETYPE_NUMBER = 1;

int byteType = BYTETYPE_NOTSET;
int bytesRead = -1;
ubyte transmitList[128];
ubyte receiveList[128];
int count = 0;
ubyte headerList[1];

////////////////////////////////////////////////////////////////////////
//  TransmitASCII - Transmit an ASCII string.
//
//  ubyte list[] - The list of ASCII chars to send, defined as a list
//                 of unsigned bytes.
//
//  int count    - The number of characters to send.
////////////////////////////////////////////////////////////////////////
void TransmitASCII()
{
  headerList[0]= 1;
  headerList[0] |= 0x80;
  int i = nxtWriteRawHS(headerList[0], sizeof(headerList));
  i = nxtWriteRawHS(transmitList[0], sizeof(ubyte)* count);
  wait10Msec(10);
}

////////////////////////////////////////////////////////////////////////
//  TransmitBytes - Transmit a byte string as a list of raw numbers.
//
//  ubyte list[] - The list of raw numbers as unsigned bytes.
//
//  int count    - The number of bytes to send.
////////////////////////////////////////////////////////////////////////
void TransmitBytes()
{
  ubyte headerList[1];
  headerList[0]= count;
  nxtWriteRawHS(headerList, 1);
  nxtWriteRawHS(transmitList, count);
  wait10Msec(10);
}


////////////////////////////////////////////////////////////////////////
//  Receive - Receives a list of bytes.
//
//  ubyte** list - The received list.  The memory for this list is
//                 allocated from within the method.
//
//  int* bytesRead - The count of bytes read.
//
//  NOTES:
//    1. The list should be deallocated using the free method.
//    2. Check to ensure the list is not null before deallocating
//    3. Sets the global byteType attribute.
//
////////////////////////////////////////////////////////////////////////
void Receive()
{
  ubyte hByte = 0;

  if (byteType == BYTETYPE_NOTSET)
  {
     int ndx = nxtReadRawHS(headerList[0],sizeof(headerList));
     hByte = headerList[0];
     if ((hByte & 0x80) != 0)
     {
       //ASCII
       byteType = BYTETYPE_ASCII;
     }
     else
     {
       byteType = BYTETYPE_NUMBER;
     }
  }
  bytesRead = hByte & 0x7F;
  nxtReadRawHS(receiveList[0], bytesRead * sizeof(ubyte));
  byteType = BYTETYPE_NOTSET;
}

////////////////////////////////////////////////////////////////////////
//  Enter Command Mode Function
//  This will put us into command mode.
////////////////////////////////////////////////////////////////////////
void EnterCommandMode()
{
  // Note: "13" is ascii representation for carriage return.
  // Get into command mode.
  ubyte commandMode[] = {'+','+','+'};  // Enter Command mode.
	wait10Msec(100);                      // Wait 1 second.
	nxtWriteRawHS(commandMode[0], sizeof(commandMode));
	wait10Msec(100);                      // Wait 1 second.
}

////////////////////////////////////////////////////////////////////////
//  Write to non-volatile memory.
//  Puts the command or parameter just set into semi-permanent memory.
//  Will be in place when the Xbee is re-fired up.
////////////////////////////////////////////////////////////////////////
void WriteNonVol()
{
  // Note: "13" is ascii representation for carriage return.
  // Get into command mode.
  ubyte commandMode[] = {'A','T','W','R',13};  // Enter Command mode.
	nxtWriteRawHS(commandMode[0], sizeof(commandMode));
}

////////////////////////////////////////////////////////////////////////
//  Exit Command Mode Function
//  This will put us into command mode.
////////////////////////////////////////////////////////////////////////
void ExitCommandMode()
{
  // Note: "13" is ascii representation for carriage return.
  // Get into command mode.
  ubyte commandMode[] = {'A','T','C','N',13};  // Enter Command mode.
	nxtWriteRawHS(commandMode[0], sizeof(commandMode));
}

////////////////////////////////////////////////////////////////////////
//  AT Command Mode Command
//  This will Send an AT Command with no modifiers or parameters.
////////////////////////////////////////////////////////////////////////
void GiveCommand(char FirASCII, char SecASCII, char Param)
{
  ubyte command[] = {'A','T',' ',FirASCII,SecASCII,13};
  nxtWriteRawHS(command[0], sizeof(command));
  wait10Msec(10);
}

////////////////////////////////////////////////////////////////////////
//  Reads a Response from the XBee
//  Displays the response from the XBee
//  This reads any response from the XBee
////////////////////////////////////////////////////////////////////////
void ReadResponse(int line_to_display)
{
  byte BytesRead[8];                           // Array we'll be reading into.
	nxtReadRawHS(BytesRead[0], 8);      // Read the array.

	string sTemp = "";
	string sString = "";
  for (int i = 0; i < 8; ++i)
	{
  	StringFormat(sTemp, "%c", BytesRead[i]);
  	BytesRead[i] = ' ';
  	sString += sTemp;
  }
	nxtDisplayTextLine(line_to_display, sString);
}

////////////////////////////////////////////////////////////////////////
//  Transmit
//  Transmit something.
////////////////////////////////////////////////////////////////////////
void Transmit() // string xmit_pkg)
{
  ubyte command[] = {'I','H','E','A','R','T','H','K','K'};
  nxtWriteRawHS(command[0], sizeof(command));
  wait10Msec(10);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        Main Task
////////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{

	eraseDisplay();
	bNxtLCDStatusDisplay = true; // Enable top status line display

  setupHighSpeedLink(true);
	//
	// Configure the link for raw read and write. User program will have complete control over the link.
	// User program will be responsible for managing the half-duplex operation and must prevent collisions!
	//
	nxtHS_Mode = hsRawMode;

	while(true)
	{
    char ILOVEHKK[10] = {'I',' ','L','O','V','E',' ','H','K','K'};
	  transmitList = 'I';
	  count = 1;
	  TransmitASCII();
	  Receive();
	  string receivedcharacters;
	  StringFromChars(receivedcharacters, receiveList);
	  nxtDisplayString(30, receivedcharacters);
	  wait10Msec(100);
	  eraseDisplay();
	}
}
