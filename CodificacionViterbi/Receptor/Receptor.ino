/*
   Wilfer Daniel Ciro Maya
   Andrés Jiménez García
   Santiago Mejía Oquendo

   Campos y ondas 2, universidad del Quindío, Armenia Quindío
*/

/*
  Receptor viterbi
*/

/* I  EA  ES  OUT

   0  00  00  000
   1  00  10  111

   0  01  00  011
   1  01  10  100

   0  10  01  001
   1  10  11  110

   0  11  01  010
   1  11  11  101

*/


short Rx[72] /* = {0,0,0,1,1,1,1,1,0,0,1,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,0,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,0,1,1,0,0,1,0,1,0,0,0,1,0,0,1,0,0,1,1}*/;
short nuevo[24];
short in = 8;
short tam = 24;
short tiempo = 200;
String actual = "00";
short bit_leidos = 0;

void setup() {
  pinMode(in, INPUT);
  Serial.begin(9600);
  delay(400);
}

void loop() {
  if (digitalRead(in) == HIGH and bit_leidos == 0) {
  }
  else {
    delay(tiempo);
    if (bit_leidos == 0) {
      delay(tiempo / 2);
    }
    else {
      if (digitalRead(in) == HIGH) {
        Rx[bit_leidos] = 1;
      }
      else {
        Rx[bit_leidos] = 0;
      }
    }
    Serial.print(Rx[bit_leidos]);
    bit_leidos = bit_leidos + 1;
  }
  if (bit_leidos == tam * 3) {
    Serial.print("--");
    actual = "00";
    for (short i = 0; i <= 23; i++) {
      if (actual == "00") {
        int d1 = 0;
        int d2 = 0;
        d1 = Rx[i * 3] == 0 ? d1 : d1 + 1;
        d1 = Rx[i * 3 + 1] == 0 ? d1 : d1 + 1;
        d1 = Rx[i * 3 + 2] == 0 ? d1 : d1 + 1;

        d2 = Rx[i * 3] == 1 ? d2 : d2 + 1;
        d2 = Rx[i * 3 + 1] == 1 ? d2 : d2 + 1;
        d2 = Rx[i * 3 + 2] == 1 ? d2 : d2 + 1;

        nuevo[i] = d1 < d2 ? 0 : 1;
        actual = d1 < d2 ? "00" : "10";
      }

      else if (actual == "01") {
        int d1 = 0;
        int d2 = 0;
        d1 = Rx[i * 3] == 0 ? d1 : d1 + 1;
        d1 = Rx[i * 3 + 1] == 1 ? d1 : d1 + 1;
        d1 = Rx[i * 3 + 2] == 1 ? d1 : d1 + 1;

        d2 = Rx[i * 3] == 1 ? d2 : d2 + 1;
        d2 = Rx[i * 3 + 1] == 0 ? d2 : d2 + 1;
        d2 = Rx[i * 3 + 2] == 0 ? d2 : d2 + 1;

        nuevo[i] = d1 < d2 ? 0 : 1;
        actual = d1 < d2 ? "00" : "10";
      }

      else if (actual == "10") {
        int d1 = 0;
        int d2 = 0;
        d1 = Rx[i * 3] == 0 ? d1 : d1 + 1;
        d1 = Rx[i * 3 + 1] == 0 ? d1 : d1 + 1;
        d1 = Rx[i * 3 + 2] == 1 ? d1 : d1 + 1;

        d2 = Rx[i * 3] == 1 ? d2 : d2 + 1;
        d2 = Rx[i * 3 + 1] == 1 ? d2 : d2 + 1;
        d2 = Rx[i * 3 + 2] == 0 ? d2 : d2 + 1;

        nuevo[i] = d1 < d2 ? 0 : 1;
        actual = d1 < d2 ? "01" : "11";
      }

      else if (actual == "11") {
        int d1 = 0;
        int d2 = 0;
        d1 = Rx[i * 3] == 0 ? d1 : d1 + 1;
        d1 = Rx[i * 3 + 1] == 1 ? d1 : d1 + 1;
        d1 = Rx[i * 3 + 2] == 0 ? d1 : d1 + 1;

        d2 = Rx[i * 3] == 1 ? d2 : d2 + 1;
        d2 = Rx[i * 3 + 1] == 0 ? d2 : d2 + 1;
        d2 = Rx[i * 3 + 2] == 1 ? d2 : d2 + 1;

        nuevo[i] = d1 < d2 ? 0 : 1;
        actual = d1 < d2 ? "01" : "11";
      }

    }
    for (short i = 0; i <= 23; i++) {
      Serial.print(nuevo[i]);
    }
    Serial.println();
    bit_leidos = 0;
    delay(500);
  }

}
