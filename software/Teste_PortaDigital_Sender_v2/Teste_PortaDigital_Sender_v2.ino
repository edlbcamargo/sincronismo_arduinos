// Este é o código do arduino que envia o sinal, lembre-se sempre de alimentar os arduinos
//com a mesma fonte ou entre si.
int espectador = 4; // Define a porta que aguarda o sinal.
int comando = 9; // Define a porta que envia o sinal.
int cont = 0; // Controla o número de ciclos para cada estado.

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); // Prepara a porta 13 para acender ou apagar o led da placa.
  pinMode(espectador, INPUT_PULLUP); // Prepara a porta 4 para aguardar o sinal.
  pinMode(comando, OUTPUT); // Prepara a porta 9 para enviar o sinal que acende o Led.  

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
  //Essa parte abaixo é só para alternar entre aceso e apagado
  while (cont < 50){  
    digitalWrite(comando, HIGH);
    int codestate = digitalRead(espectador);
    if (codestate == HIGH){
      digitalWrite(LED_BUILTIN, HIGH); 
      Serial.println("ACESO"); 
    }
    if (cont == 49){
      cont = cont + 49;   
    }
    cont++;
  }
  while (cont < 100){
    digitalWrite(comando, LOW);
    int codestate = digitalRead(espectador);
    if (codestate == LOW){
      digitalWrite(LED_BUILTIN, LOW); 
      Serial.println("APAGADO"); 
    }
    if (cont == 50){
      cont = cont -49;
    }
    cont--;      
  }
}
