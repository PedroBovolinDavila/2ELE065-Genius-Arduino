/*
[] - IMPLEMENTAR DEBOUNCE NOS BOTOES !!!
[] - ALTERAR PARA USO DO MILIS !!!
[x] - TIRAR O ULTIMO BOTAO !!!
[] - MUDAR A FUNCAO ANIMACAO DE INICIO PARA USAR MILLIS E NAO TRAVAR O INICIO DO CODIGO !!!
[] - MUDAR VARIAVEIS CONSTANTES PARA #DEFINE (MENOS CONSUMO DE MEMORIA)
*/

const int led_verde = 4;
const int led_azul = 5;
const int led_vermelho = 6;
const int led_amarelo = 7;

const int leds[] = {led_verde, led_azul, led_vermelho, led_amarelo};
const int total_leds = sizeof(leds)/sizeof(leds[0]);

const int btn_verde = 8;
const int btn_azul = 9;
const int btn_vermelho = 10;
const int btn_amarelo = 11;
const int btn_iniciar = 2;

const int pino_buzzer = A4;
const int som_verde = 440;
const int som_azul = 494;
const int som_vermelho = 523;
const int som_amarelo = 587;
const int som_erro = 250;

const int notaDo = 262; // Dó
const int notaRe = 294; // Ré
const int notaMi = 330; // Mi
const int notaFa = 349; // Fá
const int notaSol = 392; // Sol

char modo = 'i';

int rodada = 0;

int ordem_leds[25];
int tamanho_ordem_leds = sizeof(ordem_leds)/sizeof(ordem_leds[0]);

void iniciarJogo();
void pistar(int delay_);
void animacao_inicio();

void setup() {
  pinMode(led_verde, OUTPUT);
  pinMode(led_azul, OUTPUT);
  pinMode(led_vermelho, OUTPUT);
  pinMode(led_amarelo, OUTPUT);

  pinMode(btn_verde, INPUT_PULLUP);
  pinMode(btn_azul, INPUT_PULLUP);
  pinMode(btn_vermelho, INPUT_PULLUP);
  pinMode(btn_amarelo, INPUT_PULLUP);

  pinMode(btn_iniciar, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(btn_iniciar), iniciarJogo, FALLING);

  for(int i = 0; i < tamanho_ordem_leds; i++) {
    ordem_leds[i] = -1;
  }
  
  Serial.begin(9600);
}

void loop() {
  if (modo == 'i') {
    if (digitalRead(btn_verde) == LOW || digitalRead(btn_azul) == LOW || digitalRead(btn_vermelho) == LOW || digitalRead(btn_amarelo) == LOW) {
      modo = 'p';
      delay(500);
    }
  } else if(modo == 'p') {
    int valor = random(4, 8);
    ordem_leds[rodada] = valor;
    
    for(int i = 0; i <= rodada; i++) {
      if (ordem_leds[i] != -1) {
      	digitalWrite(ordem_leds[i], HIGH);
        
        if(ordem_leds[i] == led_verde){
        	tone(pino_buzzer, som_verde);
        } else if (ordem_leds[i] == led_azul){
        	tone(pino_buzzer, som_azul);
        } else if (ordem_leds[i] == led_vermelho){
        	tone(pino_buzzer, som_vermelho);
        } else if (ordem_leds[i] == led_amarelo){
        	tone(pino_buzzer, som_amarelo);
        }
        
        delay(1000);
        digitalWrite(ordem_leds[i], LOW);
        noTone(pino_buzzer);
        delay(1000);
      }
    }
    
    modo = 'r';
    
  } else if (modo == 'r') {
    int botoes_selecionados[rodada + 1];
	int tm_botoes_selecionados = sizeof(botoes_selecionados)/sizeof(botoes_selecionados[0]);
    int index = 0;
    
    
    while (index <= rodada) {
      if (digitalRead(btn_verde) == LOW) {
        botoes_selecionados[index] = led_verde;
        tone(pino_buzzer, som_verde);
        delay(500);
        noTone(pino_buzzer);
        index++;
      	delay(300);
      }
      if (digitalRead(btn_azul) == LOW) {
        botoes_selecionados[index] = led_azul;
        tone(pino_buzzer, som_azul);
        delay(500);
        noTone(pino_buzzer);
        index++;
        delay(300);
      }
      if (digitalRead(btn_vermelho) == LOW) {
        botoes_selecionados[index] = led_vermelho;
        tone(pino_buzzer, som_vermelho);
        delay(500);
        noTone(pino_buzzer);
        index++;
        delay(300);
      }
      if (digitalRead(btn_amarelo) == LOW) {
        botoes_selecionados[index] = led_amarelo;
        tone(pino_buzzer, som_amarelo);
        delay(500);
        noTone(pino_buzzer);
        index++;
        delay(300);
      }
      
    } 
    
    for(int i = 0; i <= rodada; i++) {
      if (botoes_selecionados[i] != ordem_leds[i]) {
      	piscar(150);
        tone(pino_buzzer, som_erro);
        delay(1000);
        noTone(pino_buzzer);
        modo = 'i';
      }
    }
    
    if (modo != 'i') {
    	rodada++;
    	modo = 'p';
    } else {
    	rodada = 0;
      	
      	for(int i = 0; i < tamanho_ordem_leds; i++) {
  			ordem_leds[i] = -1;
    	}
      
     	delay(4000);
    }
    
  } else {
    animacao_inicio();
  }
}

void iniciarJogo() {
  if(modo == 'i'){
  	modo = 'p';
  }
}

void piscar(int delay_) {  
	for(int i = 0; i < total_leds; i++) {
    	digitalWrite(leds[i], HIGH);
    }
    delay(delay_);
    
    for(int i = total_leds - 1; i >= 0; i--) {
    	digitalWrite(leds[i], LOW);
    }
    delay(delay_);
  	for(int i = 0; i < total_leds; i++) {
    	digitalWrite(leds[i], HIGH);
    }
    delay(delay_);
    
    for(int i = total_leds - 1; i >= 0; i--) {
    	digitalWrite(leds[i], LOW);
    }
    delay(delay_);
  	for(int i = 0; i < total_leds; i++) {
    	digitalWrite(leds[i], HIGH);
    }
    delay(delay_);
    
    for(int i = total_leds - 1; i >= 0; i--) {
    	digitalWrite(leds[i], LOW);
    }
    delay(delay_);
}

void animacao_inicio(){
  tone(pino_buzzer, notaDo, 300);//Frequência e duração da nota Dó
  digitalWrite(led_verde, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);
  
  //Ré
  tone(pino_buzzer, notaRe, 300);//Frequência e duração da nota Ré
  digitalWrite(led_azul, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);

  //Mi
  tone(pino_buzzer, notaMi, 300);//Frequência e duração da nota Mi
  digitalWrite(led_vermelho, HIGH);
  
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);

  //Fá
  tone(pino_buzzer, notaFa, 300);//Frequência e duração da nota Fá
  digitalWrite(led_amarelo, HIGH);

  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);

  //Fá Fá
  for (int a = 0; a < 2; a++) {
    tone(pino_buzzer, notaFa, 200);//Frequência e duração da nota Fá
    for(int i = 0; i < total_leds; i++) {
    	digitalWrite(leds[i], HIGH);
    }
    delay(300);//Intervalo de 300 milissegundos
    noTone(pino_buzzer);
    for(int i = 0; i < total_leds; i++) {
    	digitalWrite(leds[i], LOW);
    }
  }

  //Dó
  tone(pino_buzzer, notaDo, 300);//Frequência e duração da nota Dó
  digitalWrite(led_verde, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);
  digitalWrite(led_verde, LOW);

  //Ré
  tone(pino_buzzer, notaRe, 300);//Frequência e duração da nota Ré
  digitalWrite(led_azul, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);
  digitalWrite(led_azul, LOW);

  //Dó
  tone(pino_buzzer, notaDo, 300);//Frequência e duração da nota Dó
  digitalWrite(led_verde, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);
  digitalWrite(led_verde, LOW);

  //Ré
  tone(pino_buzzer, notaRe, 300);//Frequência e duração da nota Ré
  digitalWrite(led_azul, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);
  digitalWrite(led_azul, LOW);

  //Ré Ré
  for (int b = 0; b < 2; b++) {
    tone(pino_buzzer, notaRe, 200);//Frequência e duração da nota Ré
    for(int i = 0; i < total_leds; i++) {
    	digitalWrite(leds[i], HIGH);
    }
    delay(300);//Intervalo de 300 milissegundos
    noTone(pino_buzzer);
    for(int i = 0; i < total_leds; i++) {
    	digitalWrite(leds[i], LOW);
    }
  }

  //Dó
  tone(pino_buzzer, notaDo, 300);//Frequência e duração da nota Dó
  digitalWrite(led_amarelo, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);

  //Sol
  tone(pino_buzzer, notaSol, 300);//Frequência e duração da nota Sol
  digitalWrite(led_vermelho, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);

  //Fa
  tone(pino_buzzer, notaFa, 300);//Frequência e duração da nota Fá
  digitalWrite(led_azul, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);

  //Mi
  tone(pino_buzzer, notaMi, 300);//Frequência e duração da nota Fá
  digitalWrite(led_verde, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);

  //Mi Mi
  for (int c = 0; c < 2; c++) {
    tone(pino_buzzer, notaMi, 200);//Frequência e duração da nota Mi
    for(int i = 0; i < total_leds; i++) {
    	digitalWrite(leds[i], HIGH);
    }
    delay(300);//Intervalo de 300 milissegundos
    noTone(pino_buzzer);
    for(int i = 0; i < total_leds; i++) {
    	digitalWrite(leds[i], LOW);
    }
  }

  //Dó
  tone(pino_buzzer, notaDo, 300);//Frequência e duração da nota Dó
  digitalWrite(led_verde, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);

  //Ré
  tone(pino_buzzer, notaRe, 300);//Frequência e duração da nota Ré
  digitalWrite(led_azul, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);

  //Mi
  tone(pino_buzzer, notaMi, 300);//Frequência e duração da nota Mi
  digitalWrite(led_vermelho, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);

  //Fa
  tone(pino_buzzer, notaFa, 300);//Frequência e duração da nota Fá
  digitalWrite(led_amarelo, HIGH);
  delay(200);//Intervalo de 200 milissegundos
  noTone(pino_buzzer);

  for (int d = 0; d < 2; d++) {
    tone(pino_buzzer, notaFa, 200);//Frequência e duração da nota Fá
    for(int i = 0; i < total_leds; i++) {
    	digitalWrite(leds[i], HIGH);
    }
    delay(300);//Intervalo de 300 milissegundos
    noTone(pino_buzzer);
    for(int i = 0; i < total_leds; i++) {
    	digitalWrite(leds[i], LOW);
    }
  }
}
