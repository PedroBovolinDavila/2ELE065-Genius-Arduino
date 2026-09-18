// --- Mapeamento de Pinos ---
#define LED_VERDE    4
#define LED_AZUL     5
#define LED_VERMELHO 6
#define LED_AMARELO  7

#define BTN_VERDE    8
#define BTN_AZUL     9
#define BTN_VERMELHO 10
#define BTN_AMARELO  11
#define BTN_INICIAR  2

#define PINO_BUZZER  A4

// --- Frequências de Áudio ---
#define SOM_VERDE    440
#define SOM_AZUL     494
#define SOM_VERMELHO 523
#define SOM_AMARELO  587
#define SOM_ERRO     250

#define NOTA_DO      262
#define NOTA_RE      294
#define NOTA_MI      330
#define NOTA_FA      349
#define NOTA_SOL     392

// --- Configurações Gerais ---
#define TEMPO_DEBOUNCE 50 // ms

const int leds[] = {LED_VERDE, LED_AZUL, LED_VERMELHO, LED_AMARELO};
const int sons[] = {SOM_VERDE, SOM_AZUL, SOM_VERMELHO, SOM_AMARELO};
const int botoesPinos[] = {BTN_VERDE, BTN_AZUL, BTN_VERMELHO, BTN_AMARELO};
const int totalLeds = 4;

// Estrutura para controle de debounce de cada botão
struct Botao {
  bool estadoAnterior;
  bool estadoEstavel;
  unsigned long ultimoTempoMudanca;
};

Botao botoes[4] = {
  {HIGH, HIGH, 0},
  {HIGH, HIGH, 0},
  {HIGH, HIGH, 0},
  {HIGH, HIGH, 0}
};

char modo = 'i'; // 'i' = Inicio, 'p' = Tocar Sequencia, 'r' = Resposta Jogador, 'e' = Erro
int rodada = 0;
int ordemLeds[25];
int passoExibicao = 0;
int passoJogador = 0;

unsigned long tempoAnterior = 0;
bool estadoLedsTocando = false;

// Controle da Animação de Início sem delay
int passoAnimacao = 0;
unsigned long tempoAnimacao = 0;

void setup() {
  for (int i = 0; i < totalLeds; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(botoesPinos[i], INPUT_PULLUP);
  }

  pinMode(PINO_BUZZER, OUTPUT);
  pinMode(BTN_INICIAR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_INICIAR), iniciarJogo, FALLING);

  for (int i = 0; i < 25; i++) {
    ordemLeds[i] = -1;
  }

  randomSeed(analogRead(A0));
  Serial.begin(9600);
}

void loop() {
  unsigned long tempoAtual = millis();

  switch (modo) {
    case 'i': // Animação do menu inicial (não bloqueante)
      executarAnimacaoInicio(tempoAtual);
      if (lerBotoesComDebounce() != -1) {
        iniciarJogo();
      }
      break;

    case 'p': // Toca a sequência gerada
      executarSequencia(tempoAtual);
      break;

    case 'r': // Resposta do jogador com leitura com debounce
      executarRespostaJogador();
      break;

    case 'e': // Tratamento de erro/fim de jogo
      executarErro(tempoAtual);
      break;
  }
}

// ISR para a interrupção no pino 2
void iniciarJogo() {
  if (modo == 'i' || modo == 'e') {
    desligarLeds();
    noTone(PINO_BUZZER);
    rodada = 0;
    passoExibicao = 0;
    passoJogador = 0;
    for (int i = 0; i < 25; i++) ordemLeds[i] = -1;
    modo = 'p';
  }
}

// Leitura de botões com algoritmo de Debounce não-bloqueante
int lerBotoesComDebounce() {
  unsigned long agora = millis();
  
  for (int i = 0; i < totalLeds; i++) {
    bool leituraAtual = digitalRead(botoesPinos[i]);

    if (leituraAtual != botoes[i].estadoAnterior) {
      botoes[i].ultimoTempoMudanca = agora;
      botoes[i].estadoAnterior = leituraAtual;
    }

    if ((agora - botoes[i].ultimoTempoMudanca) > TEMPO_DEBOUNCE) {
      if (leituraAtual != botoes[i].estadoEstavel) {
        botoes[i].estadoEstavel = leituraAtual;

        // Borda de descida (botão pressionado)
        if (botoes[i].estadoEstavel == LOW) {
          return i; // Retorna o índice do botão (0 a 3)
        }
      }
    }
  }
  return -1; // Nenhum botão foi pressionado
}

void executarSequencia(unsigned long tempoAtual) {
  if (passoExibicao == 0 && !estadoLedsTocando) {
    ordemLeds[rodada] = random(0, 4); // Armazena índice (0-3)
  }

  if (!estadoLedsTocando) {
    if (tempoAtual - tempoAnterior >= 300) { // Intervalo entre notas
      tempoAnterior = tempoAtual;
      int ledIndex = ordemLeds[passoExibicao];
      
      digitalWrite(leds[ledIndex], HIGH);
      tone(PINO_BUZZER, sons[ledIndex]);
      estadoLedsTocando = true;
    }
  } else {
    if (tempoAtual - tempoAnterior >= 600) { // Duração do som/led
      tempoAnterior = tempoAtual;
      int ledIndex = ordemLeds[passoExibicao];
      
      digitalWrite(leds[ledIndex], LOW);
      noTone(PINO_BUZZER);
      estadoLedsTocando = false;
      
      passoExibicao++;
      if (passoExibicao > rodada) {
        passoExibicao = 0;
        passoJogador = 0;
        modo = 'r'; // Alterna para aguardar o jogador
      }
    }
  }
}

void executarRespostaJogador() {
  int botaoPressionado = lerBotoesComDebounce();

  if (botaoPressionado != -1) {
    digitalWrite(leds[botaoPressionado], HIGH);
    tone(PINO_BUZZER, sons[botaoPressionado]);
    delay(200); // Feedback visual e sonoro curto ao pressionar
    digitalWrite(leds[botaoPressionado], LOW);
    noTone(PINO_BUZZER);

    if (botaoPressionado == ordemLeds[passoJogador]) {
      passoJogador++;
      if (passoJogador > rodada) {
        rodada++;
        if (rodada >= 25) {
          modo = 'i'; // Limite máximo atingido
        } else {
          modo = 'p';
          tempoAnterior = millis();
        }
      }
    } else {
      modo = 'e'; // Jogada incorreta
      tempoAnterior = millis();
    }
  }
}

void executarErro(unsigned long tempoAtual) {
  static bool erroLigado = false;
  
  if (!erroLigado) {
    tone(PINO_BUZZER, SOM_ERRO);
    for (int i = 0; i < totalLeds; i++) digitalWrite(leds[i], HIGH);
    erroLigado = true;
    tempoAnterior = tempoAtual;
  } else if (tempoAtual - tempoAnterior >= 1000) {
    noTone(PINO_BUZZER);
    for (int i = 0; i < totalLeds; i++) digitalWrite(leds[i], LOW);
    erroLigado = false;
    modo = 'i';
  }
}

void desligarLeds() {
  for (int i = 0; i < totalLeds; i++) {
    digitalWrite(leds[i], LOW);
  }
}

// Animação de entrada fluida e não-bloqueante usando millis()
void executarAnimacaoInicio(unsigned long tempoAtual) {
  const int duracaoNota = 250;
  
  if (tempoAtual - tempoAnimacao >= duracaoNota) {
    tempoAnimacao = tempoAtual;
    desligarLeds();
    noTone(PINO_BUZZER);

    switch (passoAnimacao) {
      case 0: tone(PINO_BUZZER, NOTA_DO); digitalWrite(LED_VERDE, HIGH); break;
      case 1: tone(PINO_BUZZER, NOTA_RE); digitalWrite(LED_AZUL, HIGH); break;
      case 2: tone(PINO_BUZZER, NOTA_MI); digitalWrite(LED_VERMELHO, HIGH); break;
      case 3: tone(PINO_BUZZER, NOTA_FA); digitalWrite(LED_AMARELO, HIGH); break;
      case 4: 
      case 5: tone(PINO_BUZZER, NOTA_FA); for(int i=0; i<4; i++) digitalWrite(leds[i], HIGH); break;
      case 6: tone(PINO_BUZZER, NOTA_SOL); digitalWrite(LED_VERMELHO, HIGH); break;
      default: passoAnimacao = -1; break;
    }
    passoAnimacao++;
  }
}