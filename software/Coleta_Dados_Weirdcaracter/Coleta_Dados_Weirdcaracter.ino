//Este código é capaz de coletar os sinais provenientes de outro arduino por meio da função 
// pulseInLong. 
int Recpin = 4; //Porta responsável por ler os sinais da PWM.
unsigned long sinal; //Variável responsável por memorizar o valor da PWM.

void setup() {
  Serial.begin(9600); //Inicializa o SerialMonitor.
  pinMode(Recpin, INPUT); //Configura o pino digital 4 como entrada. 
   

}

void loop() {
  if (pulseInLong(Recpin, HIGH)){ //Verifica se há sinal a receber.
    sinal = pulseInLong(Recpin, HIGH); //Armazena o tempo (ms).
    Serial.println(sinal); //Exibe no SerialMonitor o tempo lido.
  }
  
}
