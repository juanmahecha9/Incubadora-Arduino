// A0 --> LM35
// A1 --> puerta
// D6 --> HS1101

// D3 --> PWM de la temperatura
// D5 --> PWM de la humedad

// D7 --> Ventilador temperatura
// D8 --> Ventilador humedad

//LM35
const int sensorPin = A0;
float celsius;

//HS1101
int input = 6;
int high_time;
int low_time;
float time_period;
float frecuency;
float Humedad;

//Monitor serial
int opc = 0; //almacena el dato de serie
int tem = 0; //Dato temp
int hum = 0; // dato humedad

//Alarma puerta
int puerta = 10 ;

//Contor de PWM
int control1 = 3; // pin digital 3 para resistencia de temperatura
int PWMT = 0; // Variable de control temperatura

int control2 = 5; // pin digital 5 para resistencia de humedad
int PWMH = 0; // Variable de control de la humedad

//control ventiladores
int ventilador1 = 9; // pin digital 7 para ventilador de temperatura
int ventilador2 = 10; // pin digital 8 para ventilador estractor de Humedad

//int P = 0; //valor inical continuidad de la puerta

// Preparar puertos

void setup()
{
  pinMode(sensorPin, INPUT); // sensor temperatura.
  pinMode(control1, OUTPUT);    //Pin digital para contolar PWM de la temperatura
  pinMode(control2, OUTPUT);    //Pin digital para contolar PWM de la humedad
  pinMode(ventilador1, OUTPUT); //Pin digital para contolar ventilador de la temperatura
  pinMode(ventilador2, OUTPUT); //Pin digital para contolar ventilador estractor de la humedad
  pinMode(puerta, OUTPUT);       //Alarma puerta
  pinMode(input, INPUT);        //pin digital para leer la humedad
  //pinMode(P, INPUT);
  Serial.begin(9600);
}

//

void loop()
{

inicio:
  Serial.print("Selecciono variable a controlar");
  Serial.println("  ");
  Serial.print("1 --> temperatura");
  Serial.println("  ");
  Serial.print("2 --> Humedad");
  Serial.println("  ");
  delay (3000);

  if (Serial.available() > 0) //Lee la opcion digitada
  {
    opc = Serial.read(); //lee serial
    Serial.print("Su opcion fue: ");
    opc = opc - 48; // conversion de ascii
    Serial.println (opc, DEC); //La vuelve decimal
    Serial.println("  ");
    if (opc == 1)
    {
      Serial.println("1 = 34 ºC");
      Serial.println("2 = 35 ºC");
      Serial.println("3 = 36 ºC");
      Serial.println("4 = 37 ºC");
      Serial.println("5 = 38 ºC");
      Serial.println("6 = 39 ºC");
      Serial.println("7 = 40 ºC");
      Serial.println("Recuerde teperatura ideal para incubadora es de 37ºC");
      Serial.println("Digite opcion: ");
      delay(8000);

      if (Serial.available() > 0)
      {
        //Humedad constante
        //temperatura variable
        tem = Serial.read();
        tem = tem - 48;
        if (tem == 1) {
          tem = 34;
        }
        if (tem == 2) {
          tem = 35;
        }
        if (tem == 3) {
          tem = 36;
        }
        if (tem == 4) {
          tem = 37;
        }
        if (tem == 5) {
          tem = 38;
        }
        if (tem == 6) {
          tem = 39;
        }
        if (tem == 7) {
          tem = 40;
        }
        Serial.print("Su opcion fue: ");
        Serial.println (tem, DEC);
        Serial.println("  ");
        hum = 75; //Constante
        goto lecturas;
      }
    }
    if (opc == 2)
    {
      Serial.println("1 = 70 %");
      Serial.println("2 = 75 %");
      Serial.println("3 = 80 %");
      Serial.println("4 = 85 %");
      Serial.println("5 = 90 %");
      Serial.println("Recuerde humedad ideal para incubadora es menor al 80%");
      Serial.println("Digite opcion: ");
      delay(4000);
      if (Serial.available() > 0)
      {
        //temperatura constante
        //Humedad variable
        hum = Serial.read();
        hum = hum - 48;
        if (hum == 1) {
          hum = 70;
        }
        if (hum == 2) {
          hum = 75;
        }
        if (hum == 3) {
          hum = 80;
        }
        if (hum == 4) {
          hum = 85;
        }
        if (hum == 5) {
          hum = 90;
        }
        Serial.print("Su opcion fue: ");
        Serial.println (hum, DEC);
        Serial.println("  ");
        tem = 37;
        goto lecturas;
      }
    }

    // Lectura de los sensores


lecturas:
    int P = analogRead(A1);
    if (P > 900) {
      //Puerta cerrada
      //lectura de temperatura ------------------------------------------------------------
      int value = analogRead(sensorPin);
      float celsius = value * 0.48;
      celsius = celsius ;

      //-----Control temperatura
      if (celsius < (tem) )
      {
        analogWrite(control1, 200); //Deja el PWM en 255
        analogWrite(ventilador1, 25); //Deja el ventilador en 10%
        analogWrite(ventilador2, 25);
      }

      if (celsius > (tem) )
      {
        analogWrite(control1, 0); //Deja el PWM en 0%
        //Serial.println("---Alerta--- Temperatura Alta");
        analogWrite(ventilador1, 255); //Deja el ventilador en 255
        analogWrite(ventilador2, 255); //Deja el ventilador en 255
      }
      //-----Alarma temperatura

      if (celsius < (tem - 1) )
      {
        Serial.println("---Alerta--- Temperatura baja");
      }
      if (celsius > (tem + 1) )
      {
        Serial.println("---Alerta--- Temperatura Alta");
      }

      //lectura de humedad ----------------------------------------------------------------
      high_time = pulseIn(input, HIGH); //mide el tiempo de subida en microsegundos
      low_time = pulseIn(input, LOW); //mide el tiempo de bajada en microsegundos

      time_period = high_time + low_time; //periodo es la suma de los tiempos de subida y de bajada
      time_period = time_period / 1000; //1000*1000 para pasar de microsegundos a segundos. En dos veces por redondeo
      frecuency = 1000 / time_period; //frecuencia es la inversa del periodo

      if (frecuency <= 6186)
      {
        Humedad = (frecuency - 7563) / (-15.3);
      }
      if ((frecuency <= 6330) and (frecuency > 6186))
      {
        Humedad = (frecuency - 7482) / (-14.4);;
      }
      if ((frecuency <= 6468 ) and (frecuency > 6330))
      {
        Humedad = (frecuency - 7434) / (-13.8);;
      }
      if ((frecuency <= 6600) and (frecuency > 6468))
      {
        Humedad = (frecuency - 7392) / (-13.2);;
      }
      if ((frecuency <= 6728) and (frecuency > 6600))
      {
        Humedad = (frecuency - 7368) / (-12.8);;
      }
      if ((frecuency <= 6853) and (frecuency > 6728))
      {
        Humedad = (frecuency - 7553) / (-12.5);;
      }
      if ((frecuency <= 6976) and (frecuency > 6853))
      {
        Humedad = (frecuency - 7345) / (-12.3);;
      }
      if ((frecuency <= 7100) and (frecuency > 6976))
      {
        Humedad = (frecuency - 7348) / (-12.4);;
      }
      if ((frecuency <= 7224) and (frecuency > 7100))
      {
        Humedad = (frecuency - 7348) / (-12.4);;
      }
      if (frecuency >= 7224)
      {
        Humedad = (frecuency - 7551) / (-12.7);
      }
      Humedad = Humedad - 75;

      //-----Control Humedad
      if (Humedad < (hum) )
      {
        analogWrite(control2, 255); //Deja el PWM en 200
        analogWrite(ventilador2, 0); //Deja el ventilador en 10%
      }
      if (Humedad > (hum) )
      {
        analogWrite(control2, 0); //Deja el PWM en 0
        analogWrite(ventilador2, 255); //Deja el ventilador en 255
      }

      //-----Alarma Humedad
      if (Humedad < (hum - 10) )
      {
        Serial.println("---Alerta--- Humedad baja");
      }
      if (Humedad > (hum + 10) )
      {
        Serial.println("---Alerta--- Humedad alta");
      }

      Serial.print("T= ");
      Serial.print(celsius);
      Serial.print(" ºC");
      Serial.println(" ");
      Serial.print("H= ");
      Serial.print(Humedad);
      Serial.println(" %");
      Serial.println("  ");
      delay (500);
      goto lecturas;
    }
if (P < 800)
   {
    digitalWrite(ventilador1, LOW);
    digitalWrite(ventilador2, LOW);
    digitalWrite(control1, LOW);
    digitalWrite(control2, LOW);
    Serial.println("Incubadora Abierta");
  }
goto inicio;
}
}
