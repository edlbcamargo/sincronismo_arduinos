// Este é o código do arduino que não envia o sinal, lembre-se sempre de alimentar os 
//arduinos com a mesma fonte ou entre si.
int espectador = 4;
int cont = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); // Prepara a porta 13 para acender ou apagar o led da placa.
  pinMode(espectador, INPUT_PULLUP); // Prepara a porta 4 para aguardar o sinal.
}

void loop() {
  int codestate = digitalRead(espectador); //Variável lê o estado da porta 4.

  if (codestate == LOW){
    digitalWrite(LED_BUILTIN, LOW); //Led apagado.
    Serial.println("APAGADO"); //Informa no Monitor Serial.  
  }
  else if (codestate == HIGH){
    digitalWrite(LED_BUILTIN, HIGH); //Led aceso.
    Serial.println("ACESO"); //Informa.     
  }
  
}
