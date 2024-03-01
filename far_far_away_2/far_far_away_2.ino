#include <Adafruit_LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd_1(0x27, 16, 2);

//Adafruit_LiquidCrystal lcd_1(0);

#define buttonPin1 2 // Pino do primeiro botão (contagem)
#define buttonPin2 3 // Pino do segundo botão (reset)
#define buttonPin3 4 // Pino do sensor de porta

int contadorAtual = 0; // Inicializando a variavel que ira armazenar quantas pessoas passaram na catraca em um dia
int contadorAnterior = 0; // Inicializando a variavel que ira armazenar quantas pessoas passaram na catraca no dia anterior
int diferenca = 0; // Inicializando a variavel que tira a diferenca entre o dia atual e o dia Anterior
int zero = 0; // variavel usada para printar na tela o numero 0 quando o contadorAtual for igual a zero e menor ou igual que o contadorAnterior
bool diaIniciado = false; // variavel utilizada para registrar quando dia comecou e quando o dia terminou 

void setup() {
  // Registrar o botao 1 como entrada
  pinMode(buttonPin1, INPUT);
  // Registrar o botao 2 como entrada
  pinMode(buttonPin2, INPUT);
  // Registrar o botao 3 como entrada
  pinMode(buttonPin3, INPUT);
  // Registrar o LCD 16x2
  lcd_1.begin();

  // Zerar a EEPROM
  for (int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }

  // Lendo o valor anterior da EEPROM e atribuindo a contadorAnterior
  contadorAnterior = EEPROM.read(0);

  // Se o dia já começou, exiba a contagem atual
  if (diaIniciado) {
    // Set do Cursor na coluna 0 e linha 0 do LCD
    lcd_1.setCursor(0, 0);
    // Printa no LCD a palavra NOW
    lcd_1.print("NOW: ");
    // Printa na tela a quantidade de pessoas no dia atual que passaram na catraca
    lcd_1.print(contadorAtual);
    // Set do Cursor na coluna 0 e linha 1 do LCD
    lcd_1.setCursor(0, 1);
    // Printa no LCD a palavra Diference
    lcd_1.print("Diference: ");
    // Printa na tela a Diferenca de pessoas no dia atual que passaram na catraca e no dia Anterior
    lcd_1.print(diferenca);
  } else { // Caso contrário, exiba a mensagem de início do dia
    // Set do Cursor na coluna 16 e linha 0 do LCD
    lcd_1.setCursor(16, 0);
    // Printa no LCD a palavra INICIO DO DIA!!!
    lcd_1.print("INICIO DO DIA!!!");
    //Animacao de deslizar
    for(int i=0; i<16; i++){
      lcd_1.scrollDisplayLeft();
      delay(300);
    }
    diaIniciado = true; // Marca o dia como iniciado
    // Mantem a palvra INICIO DO DIA!!! por 1s na tela
    delay(1000);
    //Limpa a tela
    lcd_1.clear();
    // Set do Cursor na coluna 0 e linha 0 do LCD
    lcd_1.setCursor(0, 0);
    // Printa no LCD a palavra NOW
    lcd_1.print("NOW: ");
    // Printa na tela a quantidade de pessoas no dia atual que passaram na catraca
    lcd_1.print(contadorAtual);
    // Set do Cursor na coluna 0 e linha 1 do LCD
    lcd_1.setCursor(0, 1);
    // Printa no LCD a palavra Diference
    lcd_1.print("Diference: ");
    // Printa na tela a Diferenca de pessoas no dia atual que passaram na catraca e no dia Anterior
    lcd_1.print(diferenca);
  }
}

void loop() {
  // Verifica o estado do botão 1
  if (digitalRead(buttonPin1) == HIGH) {
    //Chamar a funcao do Botao1
    botao1();
    // Esperar 200 ms para seguir
    delay(200); // Pequeno atraso para evitar a detecção múltipla de pressionamento do botão
  }

  // Verifica o estado do botão 2
  if (digitalRead(buttonPin2) == HIGH) {
    //Chamar a funcao do Botao2
    botao2();
    // Esperar 200 ms para seguir
    delay(200); // Pequeno atraso para evitar a detecção múltipla de pressionamento do botão
  }

  // Verifica o estado do botão 3
  if (digitalRead(buttonPin3) == HIGH) {
    //Chamar a funcao do Botao3
    botao3();
    // Esperar 200 ms para seguir
    delay(200); // Pequeno atraso para evitar a detecção múltipla de pressionamento do botão
  }
}

void botao1() {
  if (!diaIniciado) {
    //Zera a diferenca toda vez q inicializar o dia
    diferenca = 0;
    //limpa a tela
    lcd_1.clear();
    //brilho maximo na tela
    lcd_1.setBacklight(255);
    // Set do Cursor na coluna 16, linha 0
    lcd_1.setCursor(16, 0);
    // Exibe a mensagem de início do dia se o dia não tiver começado
    lcd_1.print("INICIO DO DIA!!!");
    //Animacao de deslizar a frase da direita pra esquerda
    for (int i = 0; i < 16; i++) {
      lcd_1.scrollDisplayLeft();
      delay(300);
    }
    diaIniciado = true; // Marca o dia como iniciado
    //Espera 1s do texto INICIO DO DIA na tela
    delay(1000);
    // Limpa a Tela
    lcd_1.clear();
    // Set do Cursor na coluna 0, linha 0
    lcd_1.setCursor(0, 0);
    // Printa no LCD a palavra NOW
    lcd_1.print("NOW: ");
    // Printa na tela a quantidade de pessoas no dia atual que passaram na catraca
    lcd_1.print(contadorAtual);
    // Set do Cursor na coluna 0 e linha 1 do LCD
    lcd_1.setCursor(0, 1);
    // Printa no LCD a palavra Diference
    lcd_1.print("Diference: ");
    // Printa na tela a Diferenca de pessoas no dia atual que passaram na catraca e no dia Anterior
    lcd_1.print(diferenca);
  } else {
    //registra cada aperto no botao 1 como alguem que passou na catraca
    contadorAtual++;
    // Funcao que mostra o valor atualizado na tela
    atualizarDisplay();
  }
}

void botao2() {
  // Escrevendo o valor atual de contadorAnterior na EEPROM
  EEPROM.write(0, contadorAtual);
  // Ao apertar o botao 2 (finalizar o dia) ele ira armazenar o valor do dia atual como sendo do dia anterior
  contadorAnterior = contadorAtual;
  // Comeca um novo dia com o contadorAtual Zerado
  contadorAtual = 0;
  // Limpa a Tela
  lcd_1.clear();
  // Chama a Funcao que mostra os valores de Now e Diference zerados
  lcd_1.print("NOW: ");
  // Printa na tela a quantidade de pessoas no dia atual que passaram na catraca
  lcd_1.print(contadorAtual);
  // Set do Cursor na coluna 0 e linha 1 do LCD
  lcd_1.setCursor(0, 1);
  // Printa no LCD a palavra Diference
  lcd_1.print("Diference: ");
  // Printa na tela a Diferenca de pessoas no dia atual que passaram na catraca e no dia Anterior
  lcd_1.print(0);
  // MAntem o Now e Diference por 1s na tela
  delay(1000);
  // Set do Cursor na coluna 16 e linha 0
  lcd_1.setCursor(16, 0);
  // Printa na tela FIM DO DIA!!!
  lcd_1.print("FIM DO DIA!!!");
  // Animacao de deslizar o FIM DO DIA!! na tela
  for(int i=0; i<16; i++){
    lcd_1.scrollDisplayLeft();
    delay(300);
  }
  diaIniciado = false; // Marca o dia como não iniciado(Foi finalizado o dia)
  delay(2000);
  // Limpa a Tela
  lcd_1.clear();
  // Desliga a Tela (Brilho Minimo)
  lcd_1.setBacklight(0);
}

void botao3() {
  // Verifica se o dia esta Iniciado
  if (diaIniciado==true){
    // Limpa a tela
    lcd_1.clear();
    // Declara o auxiliar (aux) que ira repetir a animacao de Embarque imediato (Now Boarding)
    int aux = 0;
    // Repete 3x esse bloco de codigo
    while(aux < 3){
      // Limpa a tela do LCD
      lcd_1.clear();
      // Define a posição inicial da palavra na coluna 16, linha 0
      lcd_1.setCursor(16, 0);
      // Escreve a palavra
      lcd_1.print("Now Boarding");
      // Animação deslizante
      for(int i = 0; i < 14; i++) {
        lcd_1.scrollDisplayLeft(); // Desloca o conteúdo do display para a esquerda
        delay(300); // Atraso para criar a animação
      }
      // Define a posição inicial da palavra na coluna 16, linha 1
      lcd_1.setCursor(16,1);
      //Escreve a Palavra
      lcd_1.print("FAR FAR AWAY");
      //Espera por 1s
      delay(1000);
      // Animação deslizante
      for(int i = 0; i < 14; i++) {
        lcd_1.scrollDisplayLeft(); // Desloca o conteúdo do display para a esquerda
        delay(300); // Atraso para criar a animação
      }
      // acrescenta uma unidade para repetir o processo 3x 
      aux++;
      // Mostra na tela a parcial de pessoas embarcados do dia
      atualizarDisplay();
      // Mantem a parcial de pessoas embarcados do dia durante 1s
      delay(1000);
    }
  }else{
    // Limpa a Tela
    lcd_1.clear();
    // Desliga a Tela (Brilho Minimo)
    lcd_1.setBacklight(0);
  }
  
}

void atualizarDisplay() {
  //Limpa a tela do LCD
  lcd_1.clear();
  // Define a posição inicial da palavra na coluna 0, linha 0
  lcd_1.setCursor(0, 0);
  // Printa no LCD a palavra NOW
  lcd_1.print("NOW: ");
  // Printa na tela a quantidade de pessoas no dia atual que passaram na catraca
  lcd_1.print(contadorAtual);
  // Espera 200 ms
  delay(200);
  // Define a posição inicial da palavra na coluna 0, linha 1
  lcd_1.setCursor(0, 1);
  // Printa no LCD a palavra Diference
  lcd_1.print("Diference: ");
  // condicao para evitar numeros negativos na Diference entre o contadorAtual - contadorAnterior
  if (contadorAnterior == 0) {
    // Printa o numero Zero no primeiro dia de inicializacao do embarcado
    lcd_1.print(zero);
    // Condicao de apos 1 dia do sistema iniciado verificamos se o Contador Atual eh maior que o contadorAnterior
  }else{
    // contadorAtual eh menor ou igual ao contadorAnterior.
    // Tira a diferenca entre contadorAtual e contadorAnterior
    diferenca = contadorAtual - contadorAnterior;
    // Printa na tela a diferenca
    lcd_1.print(diferenca);
  }
  // Espera 200 ms
  delay(200);
}
