
/*
   Wilfer Daniel Ciro Maya
   Andrés Jiménez García
   Santiago Mejía Oquendo

   Campos y ondas 2, universidad del Quindío, Armenia Quindío
*/

/*
  Emisor Hamming con paridad par
*/


int dato[8];// = {0, 1, 1, 0, 1, 0, 1, 0};
int datoOut[12];
int p = 0;
int j = 0;
int pot = 0;
int k = 0;
int sumTest = 0;
int out = 0;
char car = 0;
void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  delay(2000);
}
void loop() {

  car = 0;
  digitalWrite(8, HIGH);

  if (Serial.available() > 0) {
    car = Serial.read();
  }
  if (car != 0) {
    for (byte j = 0; j <= 7; j++) {
      dato[j] = bitRead(car, 7 - j);
      Serial.print(dato[j]);
    }
    Serial.print("--");

    k = 0;
    sumTest = 0;
    out = 0;

    //Organiza los bits de dato
    p = 0;
    j = 0;
    pot = 0;
    for (int i = 0; i < 12; i++) {
      pot = potencia(2, p) - 1;
      if (i == pot) {
        p++;
        datoOut[i] = 0;
      }
      else {
        datoOut[i] = dato[j];
        j++;
      }
    }

    //Calculo de paridades

    for (int i = 0; i < 4; i++) {
      p = potencia(2, i);
      k = p - 1;
      out = 0;
      sumTest = 0;
      while (out == 0) {
        j = 0;
        while (j < p && out == 0) { //Toma valores
          sumTest = datoOut[k] + sumTest;
          j++;
          k++;
          if (k == 12) {
            out = 1;
          }
        }
        j = 0;
        while (j < p && out == 0) { //No toma valores
          j++;
          k++;

          if (k == 12) {
            out = 1;
          }
        }
      }
      if (sumTest % 2 == 1) {
        datoOut[p - 1] = 1;
      }
      else {
        datoOut[p - 1] = 0;
      }
    }

    //  //Muestra el dato de salida
    //  for (int i = 0; i < 12; i++) {
    //    digitalWrite(12 - i, datoOut[i]);
    //  }
    enviarDato(datoOut);
    digitalWrite(8, HIGH);
    delay(10);
  }
}


void enviarDato(int datoOut[]) {
  int  t = 400;    //Tiempo de envio de bits, en ms
  digitalWrite(8, LOW);
  delay(t);
  for (int i = 0; i < 12; i++) {
    digitalWrite(8, datoOut[i]);
    Serial.print(datoOut[i]);
    delay(t);
  }
  digitalWrite(8, HIGH);
  Serial.println();
}




int potencia(int a, int b) {
  int resultado = 1;
  while (b > 0) {
    resultado = resultado * a;
    b--;
  }
  return resultado;
}

