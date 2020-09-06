/* Esse código aqui é basicamente um teste pra ver qual a diferença do tempo em ciclos
que cada arduino tem para receber um sinal. Ainda não sei como armazenar, talvez use 
uma tabela também pra ver claramente.*/ 

import processing.serial.*; //importa a biblioteca serial.
Serial Unoport; // Cria um objeto da classe serial, serve para ler se há sinal na porta do Arduino.
Serial Nanoport; // Mesma função do anterior, mas para o Nano.
Table table; //Cria uma variável tabela que serve pra armazenar os dados.

int lf = 10; // Nova linha (Linefeed) em ASCII.
String UnoString = null; // String onde sera guardada a mensagem lida pelo arduino Uno.
String NanoString = null; //Serve para guardar a mernsagem do Nano.

int numLeituras = 501; //Essa variável serve para limitar o número de leituras.
int numCount = 1; //Essa serve para contar o número de leituras.
int times = 0; //Variável que conta o número de vezes que ficou preso no primeiro try.
int vezes = 0; //Conta o número de vezes que ficou preso no segundo try.

void setup(){//Prepara o programa.
 String portUno = Serial.list()[0]; // Porta no Windows é a primeira, use para o Uno.
 String portNano = Serial.list()[1]; // Porta do Nano.
 /*Tem 3 entradas USB no meu notebook, eu uso a primeira (lado direito mais próximo
 do usuário) para o Uno (COM3) e a segunda para o Nano (COM4).*/
 println("Portas seriais disponiveis:");
 println(Serial.list());
 println("Portas seriais em uso:");
 println(portUno, portNano);
 
 table = new Table();
 
 // Inicializa a porta
 Unoport = new Serial(this, portUno, 9600);
 Nanoport = new Serial(this, portNano, 9600); 
 
 // joga fora as primeiras leitura do Uno, diminui a chance de pegar a mensagem no meio.
 try {
   Unoport.clear();
   while (Unoport.readStringUntil(lf) == null){
     println("Preso no while Unoport");
     times = times + 1;
   }
   UnoString = Unoport.readStringUntil(lf);
   UnoString = null;
 } 
 catch (Exception e){ 
   println("Deu algum xabu no Unoport");
 } 
 finally{
   println("Os ciclos do while Unoport foram: ", times);   
 }
 //Joga fora as leituras null do nano até aparecer um sinal. 
 //Evita que o programa pare de rodar.
 try {
   Nanoport.clear();
   while (Nanoport.readStringUntil(lf) == null){
     println("Preso no while Nanoport");
     vezes = vezes + 1;
   }
   NanoString = Nanoport.readStringUntil(lf);
   NanoString = null;
 } 
 catch (Exception e){
   println("Deu algum xabu no Nanoport");  
 }
 finally {
   println("Os ciclos do while do Unoport foram: ", times);
   println("Os ciclos do while do Nanoport foram: ", vezes);
     
 }
 // cria colunas da tabela
 table.addColumn("Contagem");
 table.addColumn("Sinal 1");
 table.addColumn("Sinal 2");
}

void draw(){
  if (Unoport.available() > 0 && Nanoport.available() > 0){ //A leitura sempre aqui dentro
    UnoString = Unoport.readStringUntil(lf);
    NanoString = Nanoport.readStringUntil(lf);
    
    if (UnoString != null && NanoString != null){
      println(UnoString, NanoString);
      TableRow novaLinha = table.addRow();
      
      // remove quebra de linhas da string
      UnoString = UnoString.replace('\n', ' '); // troca quebra de linha por espaço.
      String valor_Uno = String.valueOf(UnoString.trim()); // O trim() remove o espaço do final.
      
      NanoString = NanoString.replace('\n', ' '); 
      String valor_Nano = String.valueOf(NanoString.trim());
      
      // coloca dados novos na tabela
      novaLinha.setInt("Contagem", numCount);
      novaLinha.setString("Sinal 1", valor_Uno);
      novaLinha.setString("Sinal 2", valor_Nano);
      numCount++;
      println(numCount);      
    }
  }
  if (numCount == numLeituras){
    saveTable( table, "data/arquivo9.csv");
    println("Finalizado");
    exit();
  }
}
