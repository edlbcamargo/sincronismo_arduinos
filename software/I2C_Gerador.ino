#include <Wire.h>
//Este código não representa uma onda de fase completa (0 a 2pi), somente representa o crescimento de 0 a pi/2.
//Para tal, seria necessário definir outra entrada oposta à primeira no filtro passa baixa, além de definir os ângulos de "queda".

#define slave1 0x02
#define slave2 0x03

byte partida = 200;
int PWMpin = 9; //Porta responsável por gerar o PWM.
int event = 10; //Porta que recebe um byte do slave dizendo que terminou a coleta de sinais. 
int diff; //Usada para imprimir a diferença entre os arduinos.
int val[199]; //Armazena o valor lido pelos slaves e imprime no serialMonitor.
int sinal = 5; //Representa a porcentagem do sinal da PWM, que varia de 0 a 255.
int cont = 0; //Contador para limitar a quantidade de leituras.
int mediu = 0; //Armazena o sinal do slave indicando que mediu.

void setup() {
  Serial.begin(9600); //Inicializa o SerialMonitor.
  pinMode(PWMpin, OUTPUT); //Define o PWMpin como saída, emissor da PWM.
  Serial.println("Início da leitura"); //Marca o início dos dados.
  analogWrite(PWMpin, sinal); //Envia o PWM.
  

}

void loop() {
  ComecaLeitura();
  //Estes for's servem para aumentar o número de picos enviados e controlar a frequência de envio.
  //Desse jeito dá pra controlar a frequência utilizando a função delayMicroseconds. 
  for (int k = 0; k<100; k++){
    for (sinal = 5; sinal<255; sinal = sinal + 25){
      analogWrite(PWMpin, sinal); //Envia o PWM.
      delayMicroseconds(100); //Intervalo para controle da frequência.
    }
  }
  delay(1000); 
    // A ideia destas funções aqui é pegar a medida dos picos que cada slave fez pela quantidade.
    ReceiveData(slave1, cont);
    ReceiveData(slave2, cont);
    cont = cont +2;
}

//Não precisa armazenar, pode enviar direto para a serial.
void ReceiveData(int placa, int i){
  Wire.requestFrom(placa, 1);

  mediu = Wire.read();
  if (mediu == 1){
    val[i] = Wire.read();
    
    if(i%2 != 0){
      Serial.print(val[i-1]);
      Serial.print(" ");
      Serial.println(val[i]);
      mediu = 0; 
    }
  }
}
void ComecaLeitura(){
  //Primeiro.
  Wire.beginTransmission(slave1);
  Wire.write(partida);
  Wire.endTransmission();
  //Segundo.
  Wire.beginTransmission(slave2);
  Wire.write(partida);
  Wire.endTransmission();  
}
