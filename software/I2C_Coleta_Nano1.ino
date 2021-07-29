#include <Wire.h>
#define slave 0x02

unsigned long sinal[149];//Variáveis responsáveis por memorizar o valor da tensão gerado por PWM, serão usadas para descobrir o momento de queda de tensão. 
int Signpin = 4; //Porta responsável por ler os sinais da PWM.
int envio = 7; //Envia um byte´para avisar que terminou a leitura.
int mediu = 0; // Variável que informa a medida
int partida = 0;
int i = 0;
int cont = 0; //Contador de medidas.
int kont[50]; //Vetor que armazena a contagem.

void setup() {
  Serial.begin(9600); //Inicializa o SerialMonitor.
  pinMode(Signpin, INPUT); //Configura o pino digital 4 como entrada. 
  Wire.begin(slave);  //Inicializa a comunicação I2C, ingressando no barramento.
  Wire.onReceive(receiveEvent); // Registra evento.
  Wire.onRequest(receiveRequest); //Registra request.
}

void loop() {
  if (partida == 200){ //Verifica sinal de início de leitura.   
    sinal[cont] = analogRead(Signpin); //Armazena a tensão. 
    cont++; //Contagem de medidas.
    if ( sinal[cont] > sinal[cont + 1]){
      mediu = 1;
      kont[i] = cont; //Armazena a contagem de medidas.
      i++;
    }
  }
}

void receiveEvent(){
  partida = Wire.read();
}

void receiveRequest(){
   if(mediu == 0){
   }
   if(mediu == 1){
   Wire.write(kont[i-1]); //Envia o número de medidas para o master.
   }
}
