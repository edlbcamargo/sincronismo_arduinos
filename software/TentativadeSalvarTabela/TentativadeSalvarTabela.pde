//Processing tem uma estrutura muito parecida com a IDE arduino.
//A ideia deste código é captar os sinais gerados pelo arduíno e gerar uma tabela 
// .csv.

import processing.serial.*; //importa a biblioteca serial.
Serial Unoport; //Cria um objeto da classe serial, serve para ler se há sinal.
Table table; //Cria uma variável tabela que serve pra armazenar os dados.

int numLeituras = 51; //essa variável serve para limitar o número de leituras.
int numCount = 1; //essa serve para contar o número de leituras.

void setup(){//Prepara o programa.
 String portNum = Serial.list()[0]; //
 
 table = new Table();
 Unoport = new Serial(this, portNum, 9600);
 
 table.addColumn("Contagem");
 table.addColumn("Sinal");
}

void draw(){
  int sinalIn = Unoport.read();
  if (Unoport.available() > 0){
    TableRow novaLinha = table.addRow();
    
    novaLinha.setInt("Contagem", numCount);
    novaLinha.setInt("Sinal", sinalIn);
    numCount++;
    //println(sinalIn);
    println(numCount);
  } 
  if (numCount == numLeituras){
    saveTable( table, "arquivo6.csv");
    println("Finalizado");
  }
}
