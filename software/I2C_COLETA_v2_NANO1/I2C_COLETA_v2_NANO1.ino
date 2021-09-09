#include <Wire.h>
#define SLAVE 0x02

// usar sempre menos de 255, pois a gente guarda o índice num char
#define NPONTOS 200
#define NPICOS 2


unsigned long sinal[NPONTOS];//Variáveis responsáveis por memorizar o valor da tensão gerado por PWM, serão usadas para descobrir o momento de queda de tensão. 
int Signpin = A2; //Porta responsável por ler os sinais da PWM.
int envio = 7; //Envia um byte para avisar que terminou a leitura.
int mediu = 0; // Variável que informa a medida
int partida = 0;
int teste = 0;
int cont = 0; //Contador de medidas.
char kont[NPICOS]; //Vetor que armazena a contagem.

void setup() {
  Serial.begin(9600); //Inicializa o SerialMonitor.
  pinMode(Signpin, INPUT); //Configura o pino digital 4 como entrada. 
  Wire.begin(SLAVE);  //Inicializa a comunicação I2C, ingressando no barramento.
  Wire.onReceive(receiveEvent); // Registra evento.
  Wire.onRequest(receiveRequest); //Registra request.
  mediu = 0;
}

void loop() {
  if (partida == 200){ //Verifica sinal de início de leitura.   
    for (int idx = 0; idx < NPONTOS; idx++){
      sinal[idx] = analogRead(Signpin); //Armazena a tensão (sampling aprox. 9600Hz)
      //delayMicroseconds(10); // para alterar a taxa de aquisição, sem muito controle
    }
  partida = 0;  
  }
  cont = 0;
  if (partida = 0){
    for (int idx = 1; idx < NPONTOS-1; idx++){
      if(sinal[idx] > 1000){
        kont[cont] = (char)idx;
        cont++;
      }
      if (cont >=NPICOS) break;
    }
    if (cont < NPICOS){
      for (int idx = cont; idx < NPICOS; idx++)
        kont[idx] = (char)0;
    }
  }
  
}

void receiveEvent(){
  partida = Wire.read();
}

void receiveRequest(){
  Wire.write(kont); //Envia o número de medidas para o master.
}
