//Quinta tentativa de fazer um programa que consiga ler os bytes enviados pela Arduino
//:)
//A ideia deste código é captar os sinais gerados pelo arduíno e gerar uma tabela 
// .csv.

import processing.serial.*; //importa a biblioteca serial.
Serial port; //Cria um objeto da classe serial, serve para ler o sinal.
Table table; //Cria uma variável tabela que serve pra armazenar os dados.
int num = 0; //Variável para contagem de leituras.
int max = 20; //Número máximo de leituras. Só para parar de rodar.

void setup(){//Prepara o programa.
 String portNum = Serial.list()[0];//Prepara a porta para receber o sinal. 
 
 table = new Table();
 port = new Serial(this, portNum, 9600);
 
 table.addColumn("Contagem");
 table.addColumn("Sinal");
}

void draw(){
  byte[] inSinal = new byte[1];
  TableRow newLine = table.addRow();
  while (port.available() > 0) {
   inSinal = port.readBytes();
   port.readBytes(inSinal);
   if (inSinal != null) {
     String read = new String(inSinal);
     println(read);
     newLine.setInt("Contagem", num);
     newLine.setString("Sinal", read);
     num++;
   }
   
   
  }
  if (port.available() ==0) {
    saveTable( table, "data/arquivo5.csv");
    println("Finalizado");
  }
}
