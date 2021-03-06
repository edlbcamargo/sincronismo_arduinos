//O objetivo deste código é gerar um PWM pelo pino 9 e receber o valor em um pino digital, no
//caso pelo pino 4.
int Recpin = 4; //Porta responsável por ler os sinais da PWM.
int PWMpin = 9; //Porta responsável por gerar o PWM.
unsigned long taxa; //Variável responsável por memorizar o valor da PWM.
int sinal = 50; //Representa a porcentagem do sinal da PWM, que varia de 0 a 255.

void setup() {
  Serial.begin(9600); //Inicializa o SerialMonitor.
  pinMode(Recpin, INPUT); //Define o Recpin (8) como entrada que lerá os sinais.
  pinMode(PWMpin, OUTPUT); //Define o PWMpin como saída, emissor da PWM.
  //Serial.println("Início da leitura"); //Marca o início dos dados.
  
}

void loop() {
  //A condicional abaixo corrige o valor do sinal se maior que 255.
  if (sinal > 255){
    sinal = 50;
  }
  analogWrite(PWMpin, sinal); //Evia o valor do PWM.
 
  if (pulseInLong(Recpin, HIGH)){//Verifica se há sinal HIGH a ser recebido na porta.
    taxa = pulseInLong(Recpin, HIGH); //Grava o valor lido pela função pulseInLong, que 
    //mede o tempo em que a entrada da porta ficou HIGH.
    Serial.println(taxa);//Exibe no SerialMonitor o tempo lido.     
  }

  sinal = sinal + 10; // altera duty cycle a ser usado no proximo loop
}
