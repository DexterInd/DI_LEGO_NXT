//----------------------------------------------------------------------
// crosstalk.h - XBee transmit and receive functions for byte and ASCII
//               strings.
//
/// Created by Dexter Industries, 2011.
/// www.dexterindustries.com
//----------------------------------------------------------------------

#pragma platform(NXT)

////////////////////////////////////////////////////////////////////////
//  Constants
////////////////////////////////////////////////////////////////////////

//The maximum message size.
const int MAX_MSG_SIZE = 127;

//The ASCII string type code.  ASCII strings will be preceded by this
//byte value.
const int ASCII_STRING_TYPE = 0;

//The NON-ASCII string type code.  Non-ASCII strings will be preceded by
//this byte value.
const int BYTE_STRING_TYPE = 1;

//The byte value used to set the high-speed byte rate.
const int HS_BYTE = 9600;


////////////////////////////////////////////////////////////////////////
//  Global variables.
////////////////////////////////////////////////////////////////////////
//The buffer used to store received byte string from XBee
ubyte receiveBuffer[MAX_MSG_SIZE];

//The buffer used to store a byte string prior to transmission from XBee
ubyte transmitBuffer[MAX_MSG_SIZE];

////////////////////////////////////////////////////////////////////////
//  setupHighSpeedLink
//  Purpose:  Sets the XBee port S$ to the high speed mode.
////////////////////////////////////////////////////////////////////////
void setupHighSpeedLink()
{
	// Initialize port S$ to "high speed" mode.
	nxtEnableHSPort();
	nxtSetHSBaudRate(HS_BYTE);
  nxtHS_Mode = hsRawMode;
}

////////////////////////////////////////////////////////////////////////
//  Function:  GetStringFromBuffer
//  Purpose:  Converts the receiveBuffer contents to a string
//  Parameters: int buffersize (value)
//              string& bufferString (reference).
////////////////////////////////////////////////////////////////////////
void GetStringFromBuffer(int bufferSize, string& bufferString)
{
   string sTemp = "";
   bufferString = "";
   for (int ndx = 0; ndx < bufferSize; ++ndx)
   {
 	   StringFormat(sTemp, "%c", receiveBuffer[ndx]);
     bufferString += sTemp;
   }
}

////////////////////////////////////////////////////////////////////////
//  Function:  Receive
//  Purpose:  Poll XBee and return the formatted message in the
//            receiveBuffer
//  Parameters: bool& isASCII (reference)
//              true if an ASCII string is read, false otherwise.
//  Returns: The count of bytes read; zero, if no bytes read.
////////////////////////////////////////////////////////////////////////
int Receive(bool& isASCII)
{
  //A temporary buffer
	ubyte buffer[MAX_MSG_SIZE + 1];

	//Read bytes into the temporary buffer.
  int bytesRead = nxtReadRawHS(buffer[0], sizeof(buffer));

  //If we have read anything, process it.
	if (bytesRead > 0)
	{
	  //Determine if this is an ascii msg (first byte = ASCII_STRING_TYPE.
    isASCII = buffer[0] == ASCII_STRING_TYPE;

    //Copy everything except the first byte into receive buffer.
    memcpy(receiveBuffer[0], buffer[1], bytesRead);
  }

	return bytesRead - 1;
}


////////////////////////////////////////////////////////////////////////
//  TransmitASCII
//  Purpose:  Transmit an ASCII string using XBee.
//  Parameters: string asciiString (value)
//               The ASCII string to transmit.
//  Returns: The count of bytes sent.
////////////////////////////////////////////////////////////////////////
int TransmitASCII(string asciiString)
{
  //Set the first byte, indicating that this is an ASCII string.
  transmitBuffer[0] = ASCII_STRING_TYPE;

  //Copy the ASCII string to the buffer, starting with the second byte
  //of the buffer.
  memcpy(transmitBuffer[1], asciiString, sizeof(asciiString));

  //Transmit the buffer.
  int retVal = nxtWriteRawHS(transmitBuffer[0], sizeof(asciiString));

  //Subtract one from the string count.
  return retVal - 1;
}

////////////////////////////////////////////////////////////////////////
//  TransmitBYTES
//  Purpose:  Transmit a byte string using XBee.
//  Parameters: ubyte& buffer (reference)
//               The buffer of bytes to transmit.
//              int bufferSize (value)
//               The size of the buffer.
//  Returns: The count of bytes sent.
////////////////////////////////////////////////////////////////////////
int TransmitBytes(ubyte& buffer, int bufferSize)
{
  transmitBuffer[0] = BYTE_STRING_TYPE;
  memcpy(transmitBuffer[1], buffer, bufferSize);
  int retVal = nxtWriteRawHS(transmitBuffer[0], bufferSize+1);
  //Subtract one from the string count.
  return retVal - 1;
}
