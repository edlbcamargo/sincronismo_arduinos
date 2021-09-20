#include <Wire.h>

#define SLAVE1 0x02
#define SLAVE2 0x03

#define NPONTOS 250   // usar sempre menos de 254, pois guardamos o índice em um char
#define NPICOS_MAX 20
int NPICOS = 2;

#define END_PIN 4     // D4
#define SIGN_PIN A2   //Porta responsável por ler os sinais analógicos

int sinal[NPONTOS];   //Variáveis responsáveis por memorizar o valor da tensão gerado por PWM, serão usadas para descobrir o momento de queda de tensão. 
int mediu = 0;        // Variável que informa a medida
int partida = 0;
char kont[NPICOS_MAX];    //Vetor que armazena a contagem.
char ENDERECO;

void setup(){
  pinMode(END_PIN, INPUT_PULLUP); // Pino que define endereço do Slave
  delay(50);
  if (digitalRead(END_PIN)){      // Se END_PIN não estiver aterrado...
    ENDERECO = SLAVE1;
  }
  else {                          // Se END_PIN estiver aterrado...
    ENDERECO = SLAVE2;
  }
  Wire.begin(ENDERECO);           // Inicializa a comunicação I2C, ingressando no barramento.
  Wire.onReceive(receiveEvent);   // Registra evento.
  Wire.onRequest(receiveRequest); // Registra request.

  pinMode(SIGN_PIN, INPUT);       // Configura o pino analógico SIGN_PIN como entrada. 
  
  mediu = 0;
}

void loop() {
  if (partida == 200){                  // Verifica sinal de início de leitura.   
    for (int idx = 0; idx < NPONTOS; idx++){
      sinal[idx] = analogRead(SIGN_PIN); // Armazena a tensão (sampling aprox. 9600Hz)
    }

    int cont = 0;
    for (int idx = 0; idx < NPONTOS-1; idx++){
      if( (sinal[idx] < 512) && (sinal[idx+1] >= 512)){ // detecta subida
        kont[cont] = (char)idx;
        cont++;
      }
      if (cont >=NPICOS) break;          // se encontrou NPICOS subidas, termina procura
    }
    if (cont < NPICOS){                  // se não encontrou NPICOS subidas, completa com 255
      for (int idx = cont; idx < NPICOS; idx++)                     
        kont[idx] = (char)255;
    }
    partida = 0;
    mediu = 1;
  }
  
}

void receiveEvent(){
  NPICOS = Wire.read();
  if (NPICOS > NPICOS_MAX)
    NPICOS = NPICOS_MAX;
  partida = 200;
}

void receiveRequest(){
  if (mediu == 1){
    Wire.write(kont, NPICOS); //Envia o número de medidas para o master.
    mediu = 0;
    for (int idx = 0; idx < NPICOS; idx++) // para garantir que não vai enviar o mesmo valor novamente
      kont[idx] = (char)255;

  }
}
