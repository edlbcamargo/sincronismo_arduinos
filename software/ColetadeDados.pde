import processing.serial.*;
Serial UnoPort; //Cria o objeto da clsse serial para ler os sinais capturados pelo Uno
Serial NanoPort; //Mesma função para o Nano
Table data; //Cria tabela para guardar os sinais
Table dataSinais;

int numSinais = 50; //Número limite de sinais
int contSinais = 0; //Conta o número de sinais para comparar
int valUno[] ; //Variável para ler os sinais
int valNano[] ;

void setup(){
  String portUno = Serial.list()[0]; //A variável recebe os sinais da porta 0 (USB)
  String portNano = Serial.list()[1];
  
  data = new Table();
  UnoPort = new Serial(this, portUno, 9600); //Prepara a porta para receber o sinal
  NanoPort = new Serial(this, portNano, 9600);
  
  data.addColumn("Sinal");
  data.addColumn("UNO");
  data.addColumn("NANO");
}

void draw(){
  int i = 0;
  TableRow UnoRow = dataSinais.addRow(); //Adiciona uma linha na nova leitura
  TableRow NanoRow = dataSinais.addRow(); //Adiciona uma linha na nova leitura
  while(contSinais < numSinais){
    valUno[i] = UnoPort.read();
    valNano[i] = NanoPort.read();
    UnoRow.setInt("Sinais", contSinais);
    
  if (valUno[i] != 0){
    //println(valUno); //Use para verificar a entrada de sinal
    UnoRow.setInt("UNO", valUno[i]);
  }
  if (valNano[i] != 0){
    //println(valNano); //Idem ao valUno
    NanoRow.setInt("NANO", valNano[i]);
  }
  contSinais++;
  i++;
  }
  if(contSinais == numSinais){
    saveTable(dataSinais, "LeituradeSinaisArduino.csv");  
  }
}
