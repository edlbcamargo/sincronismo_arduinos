#include <Wire.h>

#define SLAVE1 0x02
#define SLAVE2 0x03
#define NPICOS 4
#define PWMpin 6   //Porta responsável por gerar o PWM.

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
}

void loop() {
  ComecaLeitura();
  
  delay(4); // controla início do envio do sinal
  EnviaOnda();

  // A ideia destas funções aqui é pegar a medida dos picos que cada slave fez pela quantidade.
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
  // valor(ms)   Frequencia(Hz)    Periodo(Pontos):
  // 1           533               18
  // 2           266               36
  // 3           177               54
  // 4           133               72
  // 5           107               90
  for (int k = 0; k<10; k++){
    digitalWrite(PWMpin, HIGH);
    delay(1);
    digitalWrite(PWMpin, LOW);
    delay(1);
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
    Serial.print((1000.0/9600.0)*diff);
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
    Serial.print(9600.0/(float)periodo1);
    Serial.print('\t'); // Espaçamento equivalente a um Tab
    Serial.println(9600.0/(float)periodo2);
  }
  Serial.println(" ");

}
