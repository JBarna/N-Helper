// Pin 13 has the LED on Teensy 3.0

#define DEBUG 1

const int led = 13;
const char DELIM = ':';

enum programState {
  waiting,
  reading,
  playback
};

programState state = waiting;
String in;
long timeRecord[5000] = { 0 };
unsigned int keyRecord[5000] = { 0 };
short pressRecord[5000] = { 0 };
int recordIndex = 0;

// the setup routine runs once when you press reset:
void setup() {                
  Serial.begin(9600);
  
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT); 
  digitalWrite(led, HIGH);   
  delay(1000);
  digitalWrite(led, LOW);
}

unsigned int translateKey(char in){
  switch( in ){
    case 'j':
      return KEY_Z;
    case 'l':
      return KEY_N;
    case 'r':
      return KEY_M;
    case 'p':
      return KEY_ESC;
  }
  return 0;
}

void parse(){
  char buf [10] = {0};
  char * bufPointer;
  int i;
  int curIndex = 0;

  #if DEBUG
  Serial.println(in);
  #endif
  
  // read in number of records
  for (i = 1, bufPointer = buf; i < in.length(); i++, bufPointer++){
    *bufPointer = in.charAt(i);
    if (*bufPointer == DELIM){
      *bufPointer = '\0'; // null terminate
      i++;
      break;
    }
  }
  
  recordIndex = atoi(buf);
  
  // start filling in our memory
  while ( i < in.length() ){
    
    keyRecord[curIndex] = translateKey(in.charAt(i));
    i += 2;
    
    pressRecord[curIndex] = in.charAt(i) == '0' ? 0 : 1;
    i += 2; 
    
    for( bufPointer = buf; in.charAt(i) != DELIM; i++, bufPointer++ ){
      *bufPointer = in.charAt(i);
    }
    
    *bufPointer = '\0'; // null terminate
    i++;
    
    timeRecord[curIndex] = atoi( buf );

    #if DEBUG
      Serial.print(keyRecord[curIndex]);
      Serial.print(DELIM);
      Serial.print(pressRecord[curIndex]);
      Serial.print(DELIM);
      Serial.print(timeRecord[curIndex]);
      Serial.println(DELIM);
    #endif
    
    curIndex++;
    
  }

  if (curIndex != recordIndex){
    Serial.print("ERROR: curIndex less than recordIndex: ");
    Serial.print(curIndex); Serial.print(" < "); Serial.println(recordIndex);
  }
}

void replay(){
  
  long startTime = millis();

  for (short curIndex = 0; curIndex < recordIndex; curIndex++){

    // kill time until the time record is equal to current time
    while( timeRecord[curIndex] > millis() - startTime );

    // check for pause
    if ( keyRecord[curIndex] == KEY_ESC ){
      Keyboard.release( KEY_Z );
      Keyboard.release( KEY_M );
      Keyboard.release( KEY_N );
      Keyboard.press( KEY_ESC );
      delay( 200 );
      Keyboard.release( KEY_ESC );
      break;
    }
    
    if ( pressRecord[curIndex] )
      Keyboard.press( keyRecord[curIndex] );
    else
      Keyboard.release( keyRecord[curIndex] );
  }
  
}

// the loop routine runs over and over again forever:
void loop() {
  String chunk;
  if (Serial.available()){
    digitalWrite(led, HIGH);
    chunk = Serial.readString();
    
    if (state == waiting){
      if (chunk.startsWith("START")){
        in = "";
        state = reading;

        // cut off the start
        chunk = chunk.substring(5);
      } else if (chunk.startsWith("PLAYBACK")){
        Serial.println("We're playing back, folks");
        state = playback;
        replay();
        state = waiting;
      }
    }

    if (state == reading){
      
      // see if this is the end
      if (chunk.endsWith("END:")){
        
        state = waiting;
        // remove end
        chunk = chunk.substring( 0, chunk.length() - 4 );
        in.concat(chunk);
        parse();
      }
      else
        in.concat(chunk);
    }
    
    digitalWrite(led, LOW);
  }
}
