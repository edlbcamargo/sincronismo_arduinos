#define analogPin A0 //Porta responsável por ler os sinais da PWM.
int PWMPin = 9;
unsigned long taxa; //Variável responsável por memorizar o valor da PWM.
int sinal = 50;

void setup() {
  Serial.begin(9600);
  pinMode(analogPin, INPUT);
  pinMode(PWMPin, OUTPUT);
  Serial.println("Início da leitura");

}

void loop() {
  
  sinal = sinal*2.55;
  if (sinal > 255){ // analogWrite() aceita valores de no maximo 255
    sinal = 0;
  }
  analogWrite(PWMPin, sinal);
  delay(100);
  if (pulseInLong(analogPin, HIGH) > 50){
    taxa = pulseInLong(analogPin, HIGH);
    Serial.println(taxa);
  }
}
