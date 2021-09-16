#include <Wire.h>
//Este código não representa uma onda de fase completa (0 a 2pi), somente representa o crescimento de 0 a pi/2.
//Para tal, seria necessário definir outra entrada oposta à primeira no filtro passa baixa, além de definir os ângulos de "queda".

#define SLAVE1 0x02
#define SLAVE2 0x03

#define NPICOS 2

byte partida = 200; 
int PWMpin = 6; //Porta responsável por gerar o PWM.
int event = 10; //Porta que recebe um byte do slave dizendo que terminou a coleta de sinais. 
int diff; //Usada para imprimir a diferença entre os arduinos.
int val1[99], val2[99]; //Armazena o valor lido pelos slaves e imprime no serialMonitor.
int sinal = 0; //Representa a porcentagem do sinal da PWM, que varia de 0 a 255.
int cont = 0; //Contador para limitar a quantidade de leituras.

void setup() {
  Serial.begin(115200); //Inicializa o SerialMonitor.
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  pinMode(PWMpin, OUTPUT); //Define o PWMpin como saída, emissor da PWM.
  Serial.println("Inicio da leitura"); //Marca o início dos dados.
  Serial.flush();
  //analogWrite(PWMpin, sinal); //Envia o PWM.
  digitalWrite(PWMpin, LOW);

}

void loop() {
  ComecaLeitura();
  //Estes for's servem para aumentar o número de picos enviados e controlar a frequência de envio.
  //Desse jeito dá pra controlar a frequência utilizando a função delayMicroseconds. 
  for (int k = 0; k<10; k++){
    /*for (sinal = 5; sinal<255; sinal = sinal + 25){
      analogWrite(PWMpin, sinal); //Envia o PWM.
      delay(50); //Intervalo para controle da frequência. De acordo com a biblioteca, a função analogRead dos slaves leva 100 microsegundos para ler.
    }*/
    digitalWrite(PWMpin, HIGH);
    delay(5);
    digitalWrite(PWMpin, LOW);
    delay(5);
  }
  //analogWrite(PWMpin, 0);
  //delay(0); 
  // A ideia destas funções aqui é pegar a medida dos picos que cada slave fez pela quantidade.
  ReceiveData(SLAVE1, NPICOS);
  ReceiveData(SLAVE2, NPICOS);
//Calcula a diferença dos picos e manda pela serial
  
}

//Não precisa armazenar, pode enviar direto para a serial.
void ReceiveData(int placa, int i){
  Wire.requestFrom(placa, i);
  int a = 0, b = 0;
  if (placa == SLAVE1){ //armazena os valores lidos pelo slave.
    while (Wire.available()) {
      val1[a] = Wire.read();
      a++;
    } 
  }
  else{
    while (Wire.available()){
      val2[b] = Wire.read();
      b++;
    }
  }

  for (int num = 0; num < NPICOS; num++){
    diff = val1[num] - val2[num];
    Serial.print(val1[num]);
    Serial.flush();
    Serial.print('\t'); // Espaçamento equivalente a um Tab
    Serial.flush();
    Serial.print(val2[num]);
    Serial.flush();
    Serial.print('\t');
    Serial.flush();
    Serial.println(diff);
    Serial.flush(); 
  }
}
void ComecaLeitura(){
  Serial.println("1");
  Serial.flush();
  //Primeiro.
  Wire.beginTransmission(SLAVE1);
  Wire.write(partida);
  Wire.endTransmission();
  //Segundo.
  Serial.println("2");
  Serial.flush();
  Wire.beginTransmission(SLAVE2);
  Wire.write(partida);
  Wire.endTransmission();  
}
