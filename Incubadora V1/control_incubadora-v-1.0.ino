// A0 --> LM35
// D6 --> HS1101
// D3 --> PWM de la temperatura
// D5 --> PWM de la humedad
// D7 --> Ventilador temperatura
// D8 --> Ventilador humedad
// A2 --> Interrupcion por puerta

//LM35
const int sensorPin = A0;
float celsius;

//HS1101
int input=2;
int high_time;
int low_time;
float time_period;
float frecuency;
float Humedad;

//Monitor serial
int opc=0; //almacena el dato de serie
int tem=0;
int hum=0;

//Contor de PWM
int control1= 5; // pin digital 5 para resistencia de temperatura
int PWMT= 0;
int control2= 6; // pin digital 6 para resistencia de temperatura
int PWMH= 0;

//control ventiladores
int ventilador1= 7; // pin digital 7 para ventilador de temperatura
int ventilador2= 8; // pin digital 8 para ventilador estractor de Humedad

void setup() 
{
  pinMode(control1, OUTPUT);    //Pin digital para contolar PWM de la temperatura
  pinMode(control2, OUTPUT);    //Pin digital para contolar PWM de la humedad
  pinMode(ventilador1, OUTPUT); //Pin digital para contolar ventilador de la temperatura
  pinMode(ventilador2, OUTPUT); //Pin digital para contolar ventilador estractor de la humedad
  pinMode(input,INPUT);         //pin digital para leer la humedad
  Serial.begin(9600);
}
void loop() 
{
 // etiqueta de inicio para seleccion de parametros de la incubadora
  inicio:
  // inicio comunicacion por puerto serial
  Serial.print("Selecciono variable a controlar");
  Serial.println("  ");
  Serial.print("1 --> temperatura");
  Serial.println("  ");
  Serial.print("2 --> Humedad");
  Serial.println("  ");
  delay (5000); //retardo de 5 seg.
  if(Serial.available() > 0) //Lee la opcion digitada
  {
    opc = Serial.read();
    Serial.print("Su opcion fue: ");
    opc=opc-48;
    Serial.println (opc,DEC);
    Serial.println("  ");
    if(opc==1)
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
      delay(5000);
      if(Serial.available() > 0)
      {
        //Humedad constante
        //temperatura variable
        tem = Serial.read();
        tem=tem-48;
        if(tem==1){tem=34;}
        if(tem==2){tem=35;}
        if(tem==3){tem=36;}
        if(tem==4){tem=37;}
        if(tem==5){tem=38;}
        if(tem==6){tem=39;}
        if(tem==7){tem=40;}
        Serial.print("Su opcion fue: ");
        Serial.println (tem,DEC);
        Serial.println("  ");
        hum=75;
        goto lecturas;
      }
    }
    if(opc==2)
    {
      Serial.println("1 = 70 %");
      Serial.println("2 = 75 %");
      Serial.println("3 = 80 %");
      Serial.println("4 = 85 %");
      Serial.println("5 = 90 %");
      Serial.println("Recuerde humedad ideal para incubadora es menor al 80%");
      Serial.println("Digite opcion: ");
      delay(5000);
      if(Serial.available() > 0)
      {
        //temperatura constante
        //Humedad variable
        hum = Serial.read();
        hum = hum-48;
        if(hum==1){hum=70;}
        if(hum==2){hum=75;}
        if(hum==3){hum=80;}
        if(hum==4){hum=85;}
        if(hum==5){hum=90;}
        Serial.print("Su opcion fue: ");
        Serial.println (hum,DEC);
        Serial.println("  ");
        tem=37;
        goto lecturas;
      }
    }

    // lectura de los sensores
    lecturas:
    int p = analogRead(A2);
    //Serial.println(p);
    if(p > 900)
    {
    //lectura de temperatura ------------------------------------------------------------
    int value = analogRead(sensorPin);
    //float millivolts = (value / 1023.0) * 5000;
    //float celsius = millivolts / 10; 
    float celsius=value*0.485;
    //Serial.print(celsius);
    //Serial.print(" C");
    //Serial.print(" Temperatura realtiva en el domo");
    //Serial.println("  ");
    //delay(2000);
    celsius = celsius-3;

    //-----Control temperatura 
    if(celsius < (tem) )
    {
      analogWrite(control1, 255); //Deja el PWM en 255
      //Serial.println("---Alerta--- Temperatura baja");
      analogWrite(ventilador1,10); //Deja el ventilador en 0
    }
    if(celsius > (tem) )
    {
      analogWrite(control1,10); //Deja el PWM en 0
      //Serial.println("---Alerta--- Temperatura Alta");
      analogWrite(ventilador1,255); //Deja el ventilador en 255
    }
    //-----Alarma temperatura
    if(celsius < (tem-2) )
    {
      //digitalWrite(control1, HIGH); //Deja el PWM en 255
      Serial.println("---Alerta--- Temperatura baja");
      //digitalWrite(ventilador1,LOW); //Deja el ventilador en 0
    }
    if(celsius > (tem+2) )
    {
      //digitalWrite(control1,LOW); //Deja el PWM en 0
      Serial.println("---Alerta--- Temperatura Alta");
      //digitalWrite(ventilador1,HIGH); //Deja el ventilador en 255
    }

    //lectura de humedad ----------------------------------------------------------------
    high_time=pulseIn(input,HIGH); //mide el tiempo de subida en microsegundos
    low_time=pulseIn(input,LOW);   //mide el tiempo de bajada en microsegundos
  
    time_period=high_time+low_time; //periodo es lasuma de los tiempos de subida y de bajada
    time_period=time_period/1000;   //1000*1000 para pasar de microsegundos a segundos. En dos veces por redondeo
    frecuency=1000/time_period;     //frecuencia es la inversa del periodo
    
    if(frecuency <= 6186)
    {
      Humedad =(frecuency-7563)/(-15.3);
    }
    if((frecuency <= 6330)and(frecuency > 6186))
    {
      Humedad =(frecuency-7482)/(-14.4);;
    }
    if((frecuency <= 6468 )and(frecuency > 6330))
    {
      Humedad =(frecuency-7434)/(-13.8);;
    }
    if((frecuency <= 6600)and(frecuency > 6468))
    {
      Humedad =(frecuency-7392)/(-13.2);;
    }
    if((frecuency <= 6728)and(frecuency > 6600))
    {
      Humedad =(frecuency-7368)/(-12.8);;
    }
    if((frecuency <= 6853)and(frecuency > 6728))
    {
      Humedad =(frecuency-7553)/(-12.5);;
    }
    if((frecuency <= 6976)and(frecuency > 6853))
    {
      Humedad =(frecuency-7345)/(-12.3);;
    }
    if((frecuency <= 7100)and(frecuency > 6976))
    {
      Humedad =(frecuency-7348)/(-12.4);;
    }
    if((frecuency <= 7224)and(frecuency > 7100))
    {
      Humedad =(frecuency-7348)/(-12.4);;
    }
    if(frecuency >= 7224)
    {
      Humedad =(frecuency-7551)/(-12.7);
    }
    Humedad= Humedad-75;
    //Serial.print(Humedad);
    //Serial.print(" % ");
    //Serial.print(" Humedad realtiva en el domo");
    //Serial.println("  ");
    //delay (2000);
    
    //-----Control Humedad
    if(Humedad < (hum) )
    {
      analogWrite(control2,255); //Deja el PWM en 255
      //Serial.println("---Alerta--- Humedad baja");
      analogWrite(ventilador2,10); //Deja el ventilador en 0
    }
    if(Humedad > (hum) )
    {
      analogWrite(control2,10); //Deja el PWM en 0
      //Serial.println("---Alerta--- Humedad alta");
      analogWrite(ventilador2,125); //Deja el ventilador en 255
    }
    //-----Alarma Humedad
    if(Humedad < (hum-10) )
    {
      //digitalWrite(control2,HIGH); //Deja el PWM en 255
      Serial.println("---Alerta--- Humedad baja");
      //digitalWrite(ventilador2,LOW); //Deja el ventilador en 0
    }
    if(Humedad > (hum+10) )
    {
      //digitalWrite(control2,LOW); //Deja el PWM en 0
      Serial.println("---Alerta--- Humedad alta");
      //digitalWrite(ventilador2,HIGH); //Deja el ventilador en 255
    }
    
    Serial.print("T= ");
    Serial.print(celsius);
    Serial.print(" ºC");
    Serial.print("  H= ");
    Serial.print(Humedad);
    Serial.println(" %");
    Serial.println("  ");
    delay (1000);
    goto lecturas;
  }
  }
  else
  {
    Serial.println("  ");
    Serial.println("---Alerta--- Cierre la incubadora");
    Serial.println("Digite valores nuevamente");
    Serial.println("  ");
    digitalWrite(control1,LOW); //Deja el PWM en 0
    digitalWrite(control2,LOW); //Deja el PWM en 0
    digitalWrite(ventilador1,LOW); //Deja el ventilador en 0
    digitalWrite(ventilador2,HIGH); //Deja el ventilador en 0
    delay (2000);
  }
}

