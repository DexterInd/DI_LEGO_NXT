
task main(){

  long long_number = 315;
  writeDebugStreamLine("%x", long_number);
  byte short_number = (byte)(long_number);
  writeDebugStreamLine("%x", short_number);
  long_number = long_number >> 8;
  writeDebugStreamLine("%x", long_number);

}
