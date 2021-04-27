//Circuito para medir la humedad relativa
//Usaremos un HS1101 conectado mediante
//un 555 en cofiguración ASTABLE
int input = 6; //Entrada que no es más que la salida del PIN3 del 555

int high_time;
int low_time;
float time_period;
float frequency;
float Humedad;

void setup()
{
  pinMode(input, INPUT);
  Serial.begin(9600);


}

void loop()  {

  Serial.print("Frecuencia: ");

  high_time = pulseIn(input, HIGH); //Mide el tiempo de subida en microsegundos
  low_time = pulseIn(input, LOW); //Mide le tiempo de bajada en microsegundos

  time_period = high_time + low_time; //Perdiodo es la suma de los tiempos de subiday de bajada
  time_period = time_period / 1000; //1000*1000 para pasar de micro a segundos. En 2 veces por redondeo.
  frequency = 1000 / time_period; //Frecuencia es la inversa del periodo

  //----------------------------------------------------
  //A apatir de la tabla de caracteristicas del HS1101
  //
  //
  //0     10    20    30    40    50    60    70    80    90    100
  //7351  7224  7100  6976  6853  6728  6600  6468  6330 6186  6033
  //

  if (frequency <= 6186)
  {
    Humedad = (frequency - 7563) / (-15.3);
  }
  if ((frequency <= 6330) and (frequency > 6186))
  {
    Humedad = (frequency - 7482) / (-14.4);;
  }
  if ((frequency <= 6468) and (frequency > 6330))
  {
    Humedad = (frequency - 7434) / (-13.8);;
  }
  if ((frequency <= 6600) and (frequency > 6468))
  {
    Humedad = (frequency - 7392) / (-13.2);;
  }
  if ((frequency <= 6728) and (frequency > 6600))
  {
    Humedad = (frequency - 7368) / (-12.8);;
  }
  if ((frequency <= 6853) and (frequency > 6728))
  {
    Humedad = (frequency - 7553) / (-12.5);;
  }
  if ((frequency <= 6976) and (frequency > 6853))
  {
    Humedad = (frequency - 7345) / (-12.3);;
  }
  if ((frequency <= 7100) and  (frequency > 6976))
  {
    Humedad = (frequency - 7348) / (-12.4);;
  }
  if ((frequency <= 7224) and (frequency > 7100))
  {
    Humedad = (frequency - 7348) / (-12.4);;
  }
  if (frequency >= 7224)
  {
    Humedad = (frequency - 7551) / (-12.7);;
  }
  //-----------------------------------------------
  Serial.print(frequency);
  Serial.println(" Hz");


  Serial.print("Humedad realtiva: ");
  Serial.print(Humedad);
  Serial.println(" %");

  delay(5000);
}

