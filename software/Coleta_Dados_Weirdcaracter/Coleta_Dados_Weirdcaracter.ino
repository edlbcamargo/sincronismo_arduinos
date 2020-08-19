//Este código é capaz de coletar os sinais provenientes de outro arduino por meio da função 
// pulseInLong. 
int Recpin = 4; //Porta responsável por ler os sinais da PWM.
unsigned long sinal; //Variável responsável por memorizar o valor da PWM.
int cont = 0;

void setup() {
  Serial.begin(9600); //Inicializa o SerialMonitor.
  pinMode(Recpin, INPUT); //Configura o pino digital 4 como entrada. 
   

}

void loop() {
  if (pulseInLong(Recpin, HIGH)){ //Verifica se há sinal a receber.
    sinal = pulseInLong(Recpin, HIGH); //Armazena o tempo (ms).
    if (cont == 0){ //Apareceu um caractere estranho na primeira leitura dos sinais enviados.
      String change = String(sinal); //Precisa trocar o tipo da variável para remover o caracter. 
      change.remove(1, 1); //Este método remove de uma String um caracter (direita) a partir da primeira posição (esquerda).
      sinal = change.toInt(); //Para retornar o valor para int.
      cont = 1;
    }
    Serial.println(sinal); //Exibe no SerialMonitor o tempo lido.
  }
  
}
