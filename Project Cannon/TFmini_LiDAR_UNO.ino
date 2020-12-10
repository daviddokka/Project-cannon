
#include <SoftwareSerial.h>

SoftwareSerial Serial1(2, 3);  // RX, TX

void setup() {
  Serial.begin( 9600 );
  Serial1.begin( 115200 );

  delay( 500 );
  Serial.println( "Starting..." );
}

void loop() {
  // If a data arrives from debug Serial, perform a readings per seconds test. 
  if ( Serial.available() > 0 ) {
    speedTest();
  }
  else {
    // Perform one distance reading and show it on Serial
    unsigned int distance = readLIDAR( 2000 );
    if ( distance > 0 ) {
      Serial.print("  Distance (m): "); Serial.print(distance * .01);
      Serial.print( " Distance (cm): "); Serial.print(distance );
      Serial.print( " Distance (in): "); Serial.print(distance * .39);
      Serial.print( " (feet): "); Serial.println((distance*.39)/12);
    }
    else {
      Serial.println( "Timeout reading LIDAR" );
    }
  }
}
void speedTest() {
  while ( Serial.available() > 0 ) {
    Serial.read();
  }
  Serial.print( "\n\nPerforming speed test...\n" );
  long t0 = millis(); 
  #define NUM_READINGS 1000
  long accum = 0;
  for ( int i = 0; i < NUM_READINGS; i++ ) {
    accum += readLIDAR( 2000 );
  }
  long t1 = millis();
  float readingsPerSecond = NUM_READINGS * 1000.0f / ( t1 - t0 );
  float meanDistance = ((float)accum) / NUM_READINGS;

  Serial.println( "\n\nSpeed test:" );
  Serial.print(readingsPerSecond); Serial.println( " readings per second.");
  Serial.print(meanDistance); Serial.println( " mean read distance." );
  Serial.println( "\n\nHit another key to continue reading the sensor distance." );

  while ( Serial.available() == 0 ) {
    delay( 10 );
  }
  while ( Serial.available() > 0 ) {
    Serial.read();
  }
}
unsigned int readLIDAR( long timeout ) {
  unsigned char readBuffer[ 9 ];
  long t0 = millis();
  while ( Serial1.available() < 9 ) {
    if ( millis() - t0 > timeout ) {
      // Timeout
      return 0;
    }
    delay( 10 );
  }
  for ( int i = 0; i < 9; i++ ) {
    readBuffer[ i ] = Serial1.read();
  }
  while ( ! detect_Frame( readBuffer ) ) {
    if ( millis() - t0 > timeout ) {
      // Timeout
      return 0;
    }
    while ( Serial1.available() == 0 ) {
      delay( 10 );
    }
    for ( int i = 0; i < 8; i++ ) {
      readBuffer[ i ] = readBuffer[ i + 1 ];
    }
    readBuffer[ 8 ] = Serial1.read();
  }
  // Distance is in bytes 2 and 3 of the 9 byte frame.
  unsigned int distance = ( (unsigned int)( readBuffer[ 2 ] ) ) |
                          ( ( (unsigned int)( readBuffer[ 3 ] ) ) << 8 );

  return distance;
}
bool detect_Frame( unsigned char *readBuffer ) {
  return  readBuffer[ 0 ] == 0x59 &&
          readBuffer[ 1 ] == 0x59 &&
          (unsigned char)(
            0x59 +
            0x59 +
            readBuffer[ 2 ] + 
            readBuffer[ 3 ] + 
            readBuffer[ 4 ] +
            readBuffer[ 5 ] + 
            readBuffer[ 6 ] + 
            readBuffer[ 7 ]
          ) == readBuffer[ 8 ];
}
