// Este é o código do arduino que não envia o sinal, lembre-se sempre de alimentar os 
//arduinos com a mesma fonte ou entre si.
int espectador = 4;
int cont = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); // Prepara a porta 13 para acender ou apagar o led da placa.
  pinMode(espectador, INPUT_PULLUP); // Prepara a porta 4 para aguardar o sinal.
  
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

void loop() {
  while (cont < 50){// Roda este while a partir do 0 até 49 (50 vezes).  
    int codestate = digitalRead(espectador); 
    if (codestate == HIGH){
      digitalWrite(LED_BUILTIN, HIGH); 
      Serial.println("ACESO"); 
    }
    if (cont == 49){ //Soma 49 à última leitura, totalizando 98.
      cont = cont + 49;   
    }
    cont++;
  }
  while (cont < 100){// Roda a partir do 99 até o 50 (50 vezes).
    int codestate = digitalRead(espectador);
    if (codestate == LOW){
      digitalWrite(LED_BUILTIN, LOW); 
      Serial.println("APAGADO"); 
    }
    if (cont == 50){
      cont = cont - 49;
    }
    cont--;      
  }
}
