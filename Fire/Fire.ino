#include"pitches.h"

int melody[] = {

NOTE_CS4, NOTE_CS4, NOTE_DS4, 
NOTE_F4, NOTE_F4, NOTE_F4,0,
NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_DS4, NOTE_CS4,NOTE_DS4,
NOTE_CS4, NOTE_AS3, 0, NOTE_F4, NOTE_DS4, NOTE_F4, 0, 0, NOTE_CS4, NOTE_CS4, NOTE_DS4, 
NOTE_F4, NOTE_GS4, NOTE_F4, NOTE_CS4, NOTE_CS4,NOTE_DS4,
NOTE_F4, NOTE_F4, NOTE_GS4, NOTE_GS4, NOTE_F4, NOTE_F4,NOTE_DS4,
NOTE_CS4, NOTE_AS3, 0, NOTE_F4, NOTE_DS4, NOTE_F4, 0, 0, NOTE_CS4, NOTE_CS4, NOTE_CS4, 
NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4,
NOTE_CS4, NOTE_AS3, NOTE_C4, 0, NOTE_CS4, NOTE_CS4, NOTE_CS4,
NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_AS3,
NOTE_CS4, NOTE_DS4, 0, NOTE_CS4, NOTE_CS4, NOTE_CS4,
NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4,
NOTE_CS4, NOTE_AS3, NOTE_C4, 0, NOTE_CS4, NOTE_CS4, NOTE_CS4,
NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_AS3,
NOTE_CS4, NOTE_DS4, 0, 0,
NOTE_AS3, NOTE_CS4, NOTE_F3, NOTE_GS3,
NOTE_AS3, NOTE_CS4, NOTE_F3, NOTE_GS3,
NOTE_FS3, NOTE_CS4, NOTE_FS3, NOTE_F3,
NOTE_DS3, NOTE_DS3, NOTE_DS3, NOTE_DS3, NOTE_GS3
,END
};

// note durations: 8 = quarter note, 4 = 8th note, etc.
float noteDurations[] = {       //duration of the notes
8,8,8,
4,4,4,0,
8,8,8,8,8,8,8,8,
4,4,4,8,4,8,4,8,8,8,8,
4,4,8,8,8,8,
8,8,8,8,4,8,8,
4,4,4,8,4,8,4,8,8,8,8,
8,8,8,8,8,8,8,8,
8,8,4,8,8,8,8,
8,8,8,8,8,8,8,8,
4,4,8,8,8,8,
8,8,8,8,8,8,8,8,
8,8,4,8,8,8,8,
8,8,8,8,8,8,8,8,
4,4,4,4,
4,4,4,4,
4,4,4,4,
4,4,4,4,
8,8,8,8,8
};

int speed=65;  //higher value, slower notes
void setup() {

Serial.begin(9600);

}

void loop() {
// need to repeat the melody.
for (int thisNote = 0; melody[thisNote]!=-1; thisNote++) {

int noteDuration = speed*noteDurations[thisNote];
tone(3, melody[thisNote],noteDuration*.95);
Serial.println(melody[thisNote]);

delay(noteDuration);

noTone(3);
}
}
