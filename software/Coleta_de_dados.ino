int analogPin = A0; //Porta responsável por ler os sinais da PWM.
unsigned long sinal; //Variável responsável por memorizar o valor da PWM.

void setup() {
  Serial.begin(9600);
  pinMode(analogPin, INPUT); //Configura o pino A0 como entrada. Opcional. 
  Serial.println("Início da leitura");  

}

void loop() {
  if (pulseInLong(analogPin, HIGH)){
  sinal = pulseInLong(analogPin, HIGH);
  Serial.println(sinal);
  
  }
  
}
