/*
   Wilfer Daniel Ciro Maya
   Andrés Jiménez García
   Santiago Mejía Oquendo

   Campos y ondas 2, universidad del Quindío, Armenia Quindío
*/

/*
  Emisor viterbi
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

short out = 8;
String dato;
short texto[24]/* = {0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1}*/;
short bit2 = 0, bit3 = 0;
short tiempo = 200; // Dado en milisegundos
short tam = 24;
/*
   Bit1 -> entrada actual
   Bit2 -> Entrada anterior
   Bit3 -> Entrada doblemente anterior
*/

void codificar_bit(short bit1, short *para_enviar, short i) {
  short salida1 = bit1;
  short salida2 = bit3 ^ bit1;
  short salida3 = (bit1 ^ bit2) ^ bit3;

  para_enviar[i * 3] = salida1;
  para_enviar[i * 3 + 1] = salida2;
  para_enviar[i * 3 + 2] = salida3;

  /*Serial.print(para_enviar[i*3]);
    Serial.print(para_enviar[i*3+1]);
    Serial.println(para_enviar[i*3+2]);*/

  bit3 = bit2;
  bit2 = bit1;
}

void add_error(short *para_enviar) {
  short cantidad = random(3, 20);
  for (short i = 0; i <= cantidad; i++) {
    short modif = random(0, tam);
    if (para_enviar[modif * 3] == 1) {
      para_enviar[modif * 3] = 0;
    }
    else {
      para_enviar[modif * 3] = 1;
    }
    Serial.print(", ");
    Serial.print(modif * 3);
  }
  //Serial.println();
}

void enviar_bits(short *para_enviar) {
  digitalWrite(out, LOW);
  delay(tiempo);
  for (short i = 0; i <= (tam * 3) - 1; i++) {
    if (para_enviar[i] == 1) {
      digitalWrite(out, HIGH);
      Serial.print(1);
    }
    else {
      digitalWrite(out, LOW);
      Serial.print(0);
    }
    delay(tiempo);
  }
  Serial.println();
  digitalWrite(out, HIGH);
}


void setup() {
  pinMode(out, OUTPUT);
  digitalWrite(out, HIGH);
  Serial.begin(9600);
  delay(400);
}

void loop() {
  digitalWrite(out, HIGH);
  if (Serial.available() > 0) {
    while (Serial.available()) {
      delay(10);
      if (Serial.available() > 0) {
        char c = Serial.read();
        dato += c;
      }
    }
  }
  if (dato != "") {
    for (byte i = 0; i <= 2; i++) {
      for (byte j = 0; j <= 7; j++) {
        texto[i * 8 + (j)] = bitRead(dato[i], 7 - j);
      }
    }
    bit2 = 0;
    bit3 = 0;
    short para_enviar[tam * 3];
    for (short i = 0; i <= tam - 1; i++) {
      Serial.print(texto[i]);
      codificar_bit(texto[i], para_enviar, i);
    }
    //Serial.println();
    Serial.print("--");
    add_error(para_enviar);
    Serial.print("--");
    enviar_bits(para_enviar);
    delay(10);
  }
  dato = "";
}
