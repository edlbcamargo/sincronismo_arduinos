//Processing tem uma estrutura muito parecida com a IDE arduino.
//A ideia deste código é captar os sinais gerados pelo arduíno e gerar uma tabela 
// .csv.

import processing.serial.*; //importa a biblioteca serial.
Serial Unoport; // Cria um objeto da classe serial, serve para ler se há sinal na porta do Arduino.
Serial Nanoport; // Mesma função do anterior, mas para o Nano.
Table table; //Cria uma variável tabela que serve pra armazenar os dados.

int numLeituras = 51; //Essa variável serve para limitar o número de leituras.
int numCount = 1; //Essa serve para contar o número de leituras.

int lf = 10; // Nova linha (Linefeed) em ASCII.
String UnoString = null; // String onde sera guardada a mensagem lida pelo arduino Uno.
String NanoString = null; //Serve para guardar a mernsagem do Nano.

void setup(){//Prepara o programa.
 //String portUno = Serial.list()[0]; // Porta no Windows é a primeira, use para o Uno.
 //String portNano = Serial.list()[1]; // Porta do Nano.
 String portUno = Serial.list()[Serial.list().length -2];
 String portNano = Serial.list()[Serial.list().length -1];// porta no Linux é a última
 println("Portas seriais disponiveis:");
 println(Serial.list());
 println("Portas seriais em uso:");
 println(portUno, portNano);
 
 table = new Table();
 
 // Inicializa a porta
 //Unoport = new Serial(this, portNum, 9600);
 Unoport = new Serial(this, portUno, 9600);
 Nanoport = new Serial(this, portNano, 9600); 

 // joga fora a primeira leitura, diminui a chance de pegar a mensagem no meio
 Unoport.clear();
 UnoString = Unoport.readStringUntil(lf);
 UnoString = null;
 
 Nanoport.clear();
 NanoString = Nanoport.readStringUntil(lf);
 NanoString = null;
 
 
 // cria colunas da tabela
 table.addColumn("Contagem");
 table.addColumn("Sinal 1");
 table.addColumn("Sinal 2");
}

void draw(){
  if (Unoport.available() > 0 && Nanoport.available() > 0){ // a leitura da serial deve ficar aqui dentro!
    UnoString = Unoport.readStringUntil(lf);
    NanoString = Nanoport.readStringUntil(lf);
    TableRow novaLinha = table.addRow();
    
    // remove quebra de linhas da string
    UnoString = UnoString.replace('\n', ' '); // troca quebra de linha por espaço
    int valor_Uno = Integer.valueOf(UnoString.trim()); // converte para inteiro. O trim() remove o espaço do final
    
    NanoString = NanoString.replace('\n', ' '); 
    int valor_Nano = Integer.valueOf(NanoString.trim()); 
    
    // coloca dados novos na tabela
    novaLinha.setInt("Contagem", numCount);
    novaLinha.setInt("Sinal 1", valor_Uno);
    novaLinha.setInt("Sinal 2", valor_Nano);
    numCount++;
    //println(sinalIn);
    println(numCount);
  } 
  if (numCount == numLeituras){
    saveTable( table, "data/arquivo1.csv");
    println("Finalizado");
    exit();
  }
}
