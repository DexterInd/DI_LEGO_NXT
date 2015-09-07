
const int BYTETYPE_NOTSET = -1;
const int BYTETYPE_ASCII = 0;
const int BYTETYPE_NUMBER = 1;

int byteType = BYTETYPE_NOTSET;
int bytesRead = -1;
ubyte transmitList[128];
ubyte receiveList[128];

////////////////////////////////////////////////////////////////////////
//  TransmitASCII - Transmit an ASCII string.
//
//  ubyte list[] - The list of ASCII chars to send, defined as a list
//                 of unsigned bytes.
//
//  int count    - The number of characters to send.
////////////////////////////////////////////////////////////////////////
void TransmitASCII(int count)
{
  ubyte headerList[1];
  headerList[0]= count;
  headerList[0] |= 0x80;
  nxtWriteRawHS(headerList, 1);
  nxtWriteRawHS(transmitList, count);
  wait10Msec(10);
}

////////////////////////////////////////////////////////////////////////
//  TransmitBytes - Transmit a byte string as a list of raw numbers.
//
//  ubyte list[] - The list of raw numbers as unsigned bytes.
//
//  int count    - The number of bytes to send.
////////////////////////////////////////////////////////////////////////
void TransmitBytes(int count)
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
  byte HeaderByte[1];

  if (byteType == BYTETYPE_NOTSET)
  {
     nxtReadRawHS(HeaderByte[0],1);
     byte hByte = HeaderByte[0];
     if ((hByte & 0x80) != 0)
     {
       //ASCII
       byteType = BYTETYPE_ASCII;
     }
     else
     {
       byteType = BYTETYPE_NUMBER;
     }

     bytesRead = byteType & 0x7F;
     nxtReadRawHS(receiveList[0], bytesRead);
  }
}
