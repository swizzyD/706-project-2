#include"pitches.h"

// TOKYOOOO DRIFFTT DU DU DU DU DUDUDU 

int melody[] = {

NOTE_AS4, 0, NOTE_AS4, 0, NOTE_AS4, 0, NOTE_AS4, 0,

NOTE_AS4, NOTE_B4, NOTE_DS5, NOTE_AS4, 0, NOTE_AS4, 0,

NOTE_AS4, NOTE_B4, NOTE_DS5, NOTE_AS4, 0, NOTE_AS4, 0,

NOTE_AS4, NOTE_B4, NOTE_DS5, NOTE_F5, 0, NOTE_F5, 0,

NOTE_GS5, NOTE_FS5, NOTE_F5, NOTE_DS5, 0, NOTE_DS5, 0,

NOTE_GS5, NOTE_FS5, NOTE_F5, NOTE_DS5, 0, NOTE_DS5, 0, 

NOTE_AS4, NOTE_B4, NOTE_DS5, NOTE_AS4, 0, NOTE_AS4, 0,

NOTE_AS4, NOTE_B4, NOTE_DS5, NOTE_AS4, 0, NOTE_AS4, 0,END

};

// note durations: 8 = quarter note, 4 = 8th note, etc.
float noteDurations[] = {       //duration of the notes
4,4,4,4,4,4,4,4,
16/3,16/3,4,4,4,4,4,
16/3,16/3,4,4,4,4,4,
16/3,16/3,4,4,4,4,4,
16/3,16/3,4,4,4,4,4,
16/3,16/3,4,4,4,4,4,
16/3,16/3,4,4,4,4,4,
16/3,16/3,4,4,4,4,4,

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
