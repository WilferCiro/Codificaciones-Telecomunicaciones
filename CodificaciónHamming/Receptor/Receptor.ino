/*
   Wilfer Daniel Ciro Maya
   Andrés Jiménez García
   Santiago Mejía Oquendo

   Campos y ondas 2, universidad del Quindío, Armenia Quindío
*/

/*
  Receptor Hamming con paridad par
*/

int error[] = {0, 0, 0, 0};
int check[6];
//int dato[11];

int j = 0;
int p = 0;
int out = 0;
int sumTest = 0;
int bitErroneo = 0;
int k = 0;
int i = 0;
int dato[12];
int datoOut[8];
boolean hayError = false;


void leerBits(int *dato) {
  int t = 400;  //Tiempo de transmision en ms
  delay(t / 2);
  delay(t);
  for (int i = 0; i < 12; i++) {
    dato[i] = digitalRead(8);
    Serial.print(dato[i]);
    delay(t);
  }
  Serial.print("--");
}

void setup() {

  Serial.begin(9600);
  pinMode(8, INPUT);
  delay(2000);
}

void loop() {
  for (int i = 0; i < 4; i++) {
    error[i] = 0;
  }
  j = 0;
  p = 0;
  out = 0;
  sumTest = 0;
  bitErroneo = 0;
  i = 0;

  if (digitalRead(8) == 1) {
  }
  else {
    leerBits(dato);
    k = random(0, 11);
    dato[k] = negar(dato[k]);
    Serial.print(k);
    Serial.print("--");
    k = 0;

    //Deteccion de errores
    for ( i = 0; i < 4; i++) { //Recorre las paridades
      sumTest = 0;
      out = 0;
      p = potencia(2, i);
      j = 0;
      k = p - 1;
      while (out == 0) {
        j = 0;
        while (j < p && out == 0) { //Toma valores
          sumTest = dato[k] + sumTest;
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
        error[i] = 1;
        hayError = true;
      }
      else {
        error[i] = 0;
      }
    }

    //Corrección de errores
    if (hayError == true) {
      for ( i = 0; i < 4; i++) { //Obtener numero en dec del bit erroneo
        bitErroneo = error[i] * potencia(2, i) + bitErroneo;
      }
      bitErroneo--;

      Serial.print(bitErroneo);
      Serial.print("--");
      for ( i = 0; i < 12; i++) { //Recorrer dato para corregir bit erroneo
        if (i == bitErroneo) {
          dato[i] = negar(dato[i]);
        }
        else {
          dato[i] = dato[i];
        }
      }
    }


    //Decodificacion
    for (i = 0, j = 0, k = 0; i < 12; i++) {
      if (i == potencia(2, k) - 1) {
        k++;
      }
      else {
        datoOut[j] = dato[i];
        j++;
      }
    }
    for (i = 0; i < 8; i++) {
      Serial.print(datoOut[i]);
    }
    Serial.println();
  }
}

int negar(int a) {
  if (a == 1) {
    a = 0;
  }
  else {
    a = 1;
  }
  return a;
}

int potencia(int a, int b) {
  int resultado = 1;
  while (b > 0) {
    resultado = resultado * a;
    b--;
  }
  return resultado;
}





