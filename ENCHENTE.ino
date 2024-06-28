#include <Q2HX711.h>

const byte OUT_pin = 3;
const byte SCK_pin = 2;
const int buzzer_pin = 10; // Pino do buzzer

// Configurações do sensor
int media = 10; // Média das leituras do sensor
Q2HX711 sensor_pressao(OUT_pin, SCK_pin);

// Variáveis de calibração
float pressaoInicial;
float leituraPressao;

// Leitura do sensor
float lerSensor() {
  float leitura = 0.0;
  for (int i = 0; i < media; i++) {
    leitura += sensor_pressao.read();
  }
  return leitura / media;
}

void setup() {
  // Inicialização do sensor e configurações adicionais
  pressaoInicial = lerSensor();
  
  pinMode(buzzer_pin, OUTPUT); // Configura o pino do buzzer como saída
  Serial.begin(9600);
}

void loop() {
  // Leitura e cálculo da pressão
  float leitura = lerSensor();
  leituraPressao = (leitura - pressaoInicial) / 1000;
  leituraPressao = max(leituraPressao, 0); 
  float profundidade = leituraPressao * (2.5 / 100);

  // Controle do buzzer
  int buzzerSignal = controlarBuzzer(profundidade);

  // Saída para o Serial Monitor
  Serial.print("Profundidade: ");
  Serial.print(profundidade, 2); // Saída da profundidade com duas casas decimais
  Serial.println(" metros");

  //Serial.print("Escalar do Buzzer: ");
  //Serial.println(buzzerSignal); // Saída do sinal do buzzer escalado

  //delay(30000); // Intervalo de leitura e envio para o Serial Monitor
}

// Função para controle do Buzzer
int controlarBuzzer(float profundidade) {
  if (profundidade >= 0.5 && profundidade <= 2.0) {
    // Alerta: bip rápido
    digitalWrite(buzzer_pin, HIGH);
    delay(350); // Tempo de cada bip
    digitalWrite(buzzer_pin, LOW);
    delay(350); // Intervalo entre bips
    return 5; // Sinal do buzzer (escalado)
  } else if (profundidade > 2.1 && profundidade <= 4) {
    // Evacuação: som contínuo e alto
    digitalWrite(buzzer_pin, HIGH);
    delay(200); // Tempo de cada bip
    digitalWrite(buzzer_pin, LOW);
    delay(200); // Intervalo entre bips
    return 10; // Sinal do buzzer (escalado)
  } else if (profundidade > 4.0) {
    // Perigo: sequência de bips mais longos
    for (int i = 0; i < 3; i++) {
      digitalWrite(buzzer_pin, HIGH);
      delay(300); // Tempo de cada bip
      digitalWrite(buzzer_pin, LOW);
      delay(300); // Intervalo entre bips
    }
    return 15; // Sinal do buzzer (escalado)
  } else {
    // Desliga o buzzer se não estiver em nenhum dos níveis
    digitalWrite(buzzer_pin, LOW);
    return 0; // Sinal do buzzer (escalado)
  }
}
