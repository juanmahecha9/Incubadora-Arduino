int lm35 = A0;  // Puerto analógico donde se conecta el sensor
float temp;    // Variable que acepta punto decimal para la lectura de la temperatura

void setup ()
{
  Serial.begin (9600);  // Inicializa la comunicación serial a una velocidad de 9600 Bauds
}

void loop ()
{
  temp = analogRead (lm35);  // Realiza la Conversión analógica - digital
  temp = temp * 0.48;        // Transforma la lectura obtenida a un valor en grados centígrados
  Serial.print ("La temperatura actual es: ");  // Imprime en TERMINAL el valor de la 
  Serial.println (temp);                        // temperatura actual.
  delay (2000);  // Realiza una lectura cada 250 ms.
}

