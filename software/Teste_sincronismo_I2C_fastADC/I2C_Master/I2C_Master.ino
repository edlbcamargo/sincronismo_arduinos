#include <Wire.h>

#define SLAVE1 0x02
#define SLAVE2 0x03
#define NPICOS 5
#define PWMpin 6   //Porta responsável por gerar o PWM.

// Velocidade de aquisicao ajustada nos Slaves:
//#define SAMPLEFREQ 9600.0 // PS_128
//#define SAMPLEFREQ 19200.0 // PS_64
//#define SAMPLEFREQ 38500.0 // PS_32
#define SAMPLEFREQ 76900.0 // PS_16
//#define SAMPLEFREQ 153800.0 // PS_8

byte partida = NPICOS; 
int val1[NPICOS], val2[NPICOS]; //Armazena o valor lido pelos slaves e imprime no serialMonitor.

void setup() {
  Serial.begin(115200); //Inicializa o SerialMonitor.
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println("Inicio da leitura"); //Marca o início dos dados.
  
  pinMode(PWMpin, OUTPUT); //Define o PWMpin como saída, emissor da PWM.
  digitalWrite(PWMpin, LOW);

  Wire.begin(); 
  delay(1000);
}

void loop() {
  ComecaLeitura();
  
  delayMicroseconds(1800); // controla início do envio do sinal, para pegar 1a onda
  EnviaOnda();

  // A ideia destas funções aqui é pegar a medida dos picos que cada slave fez pela quantidade.
  delay(10); // aguarda slaves fazerem as contas
  ReceiveData(SLAVE1, NPICOS);
  ReceiveData(SLAVE2, NPICOS);

  ImprimeInfo();
  
  delay(1000);
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


}
void ComecaLeitura(){ // Não pode ter serial aqui pois atrasa o sincronismo
  //Primeiro.
  Wire.beginTransmission(SLAVE1);
  Wire.write(partida);
  Wire.endTransmission();
  //Segundo.
  Wire.beginTransmission(SLAVE2);
  Wire.write(partida);
  Wire.endTransmission();  
}

void EnviaOnda(){
  // número de pontos considerando amostragem a 9600Hz em funcao
  // do valor nas funções "delay()"
  // valor(us)   Frequencia(kHz)   Periodo(Pontos a 76.9kSps):
  // 500         0.985             78
  // 250         1.971             39
  // 100         4.806             16
  // 50          9.612             8
  // 25          19.225            4                           
  for (int k = 0; k<100; k++){
    digitalWrite(PWMpin, HIGH);
    delayMicroseconds(100);
    digitalWrite(PWMpin, LOW);
    delayMicroseconds(100);
  }

}

void ImprimeInfo(){
  int diff; //Usada para imprimir a diferença entre os arduinos.
  
  //Calcula a diferença dos picos e manda pela serial
  for (int num = 0; num < NPICOS; num++){
    diff = abs(val1[num] - val2[num]);
    Serial.print(num);
    Serial.print(": ");
    Serial.print(val1[num]);
    Serial.print('\t'); // Espaçamento equivalente a um Tab
    Serial.print(val2[num]);
    Serial.print('\t');
    Serial.print(diff);
    Serial.print("\tAtraso:\t");
    Serial.print((1000.0/SAMPLEFREQ)*diff);
    Serial.println("ms");
  }
  for (int num = 1; num < NPICOS; num++){
    int periodo1 = (float)val1[num]-(float)val1[num-1];
    int periodo2 = (float)val2[num]-(float)val2[num-1];
    Serial.print(num);
    Serial.print(": Periodo\t");
    Serial.print(periodo1);
    Serial.print('\t'); // Espaçamento equivalente a um Tab
    Serial.print(periodo2);
    Serial.print("\tFreq Sinal:\t");
    Serial.print(SAMPLEFREQ/(float)periodo1);
    Serial.print('\t'); // Espaçamento equivalente a um Tab
    Serial.println(SAMPLEFREQ/(float)periodo2);
  }
  Serial.println(" ");

}
