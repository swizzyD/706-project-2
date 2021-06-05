#include"pitches.h"

int melody[] = {
NOTE_CS5, NOTE_CS5, NOTE_DS5, 
NOTE_F5, NOTE_F5, NOTE_F5,0,
NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_DS5, NOTE_CS5,NOTE_DS5,
NOTE_CS5, NOTE_AS4, 0, NOTE_F5, NOTE_DS5, NOTE_F5, 0, 0, NOTE_CS5, NOTE_CS5, NOTE_DS5, 
NOTE_F5, NOTE_GS5, NOTE_F5, NOTE_CS5, NOTE_CS5,NOTE_DS5,
NOTE_F5, NOTE_F5, NOTE_GS5, NOTE_GS5, NOTE_F5, NOTE_F5,NOTE_DS5,
NOTE_CS5, NOTE_AS4, 0, NOTE_F5, NOTE_DS5, NOTE_F5, 0, 0, NOTE_CS5, NOTE_CS5, NOTE_CS5, 
NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5,
NOTE_CS5, NOTE_AS4, NOTE_C5, 0, NOTE_CS5, NOTE_CS5, NOTE_CS5,
NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_AS4,
NOTE_CS5, NOTE_DS5, 0, NOTE_CS5, NOTE_CS5, NOTE_CS5,
NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5,
NOTE_CS5, NOTE_AS4, NOTE_C5, 0, NOTE_CS5, NOTE_CS5, NOTE_CS5,
NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_CS5, NOTE_AS4,
NOTE_CS5, NOTE_DS5, 0, 0,
NOTE_AS4, NOTE_CS5, NOTE_F4, NOTE_GS4,
NOTE_AS4, NOTE_CS5, NOTE_F4, NOTE_GS4,
NOTE_FS4, NOTE_CS5, NOTE_FS4, NOTE_F4,
NOTE_DS4, NOTE_DS4, NOTE_DS4, NOTE_DS4, NOTE_GS4
,END
};

// note durations: 8 = quarter note, 4 = 8th note, etc.
float noteDurations[] = {       //duration of the notes
4,4,4,
8,8,8,8,
4,4,4,4,4,4,4,4,
8,8,8,4,8,4,8,4,4,4,4,
8,8,4,4,4,4,
4,4,4,4,8,4,4,
8,8,8,4,8,4,8,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,8,4,4,4,4,
4,4,4,4,4,4,4,4,
8,8,4,4,4,4,
4,4,4,4,4,4,4,4,
4,4,8,4,4,4,4,
4,4,4,4,4,4,4,4,
8,8,8,8,
8,8,8,8,
8,8,8,8,
8,8,8,8,
4,4,4,4,4
};

int speed=60;  //higher value, slower notes
void setup() {

Serial.begin(9600);
delay(1200);
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
