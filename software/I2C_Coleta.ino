#define slave 0x02
unsigned long sinal; //Variável responsável por memorizar o valor da PWM.
int Signpin = 4; //Porta responsável por ler os sinais da PWM.
int cont = 0;
boolean partida = false;

void setup() {
  Serial.begin(9600); //Inicializa o SerialMonitor.
  pinMode(Signpin, INPUT); //Configura o pino digital 4 como entrada. 
  Wire.begin(slave);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop() {
  if (partida == true){ //Verifica se há sinal a receber.
    sinal = pulseInLong(Signpin, HIGH); //Armazena o tempo (ms).
    Serial.println(sinal); //Exibe no SerialMonitor o tempo lido.   
  }
}

void receiveEvent(){
  partida = Wire.read();
}
