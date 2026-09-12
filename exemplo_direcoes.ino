#include "lidar_reader.h"

// Buffer fixo para receber os 12 pontos de cada pacote
LidarPoint pontosDoPacote[12];

// Margem de erro para capturar o ângulo (na ESP32 podemos usar 0.6 devido à maior velocidade)
const float margem = 0.6; 

void setup() {
  // A ESP32 suporta velocidades muito altas no monitor serial
  Serial.begin(9600); 
  
  // Inicializa a Serial2 (Porta RX2)
  // Baudrate: 230400
  // Protocolo: SERIAL_8N1 (padrão)
  // RX: Pino 16 (Padrão da ESP32 para Serial2)
  // TX: Pino 17 (Não usado pelo Lidar, mas necessário para a função)
  Serial2.begin(230400);
  
  Serial.println("ESP32 Lidar - Buscando pontos cardeais (0, 90, 180, 270)...");
  Serial.println("Conecte o TX do Lidar no GPIO 16 (RX2)");
}

void loop() {
  // Captura os pontos do sensor
  uint8_t totalLido = getPoints(pontosDoPacote);

  if (totalLido > 0) {
    for (int i = 0; i < totalLido; i++) {
      // Converte o ângulo inteiro da biblioteca para float (graus)
      float anguloAtual = pontosDoPacote[i].angle() / 100.0;
      uint16_t distancia = pontosDoPacote[i].distance();
      delay(10);
      // Só processa se houver retorno de sinal (distância > 0)
      if (distancia > 0) {
        
        // Verifica proximidade do NORTE (0° ou 360°)
        if (abs(anguloAtual - 0.0) < margem || abs(anguloAtual - 360.0) < margem) {
          imprimirPonto("NORTE (0°)", anguloAtual, distancia);
        }
        // Verifica proximidade do LESTE (90°)
        else if (abs(anguloAtual - 90.0) < margem) {
          imprimirPonto("LESTE (90°)", anguloAtual, distancia);
        }
        // Verifica proximidade do SUL (180°)
        else if (abs(anguloAtual - 180.0) < margem) {
          imprimirPonto("SUL (180°)", anguloAtual, distancia);
        }
        // Verifica proximidade do OESTE (270°)
        else if (abs(anguloAtual - 270.0) < margem) {
          imprimirPonto("OESTE (270°)", anguloAtual, distancia);
        }
      }
    }
  }
}

// Função auxiliar para exibição formatada
void imprimirPonto(String direcao, float angulo, uint16_t dist) {
  Serial.print("[");
  Serial.print(direcao);
  Serial.print("] Ang: ");
  Serial.print(angulo, 2); // 2 casas decimais
  Serial.print("° | Dist: ");
  Serial.print(dist);
  Serial.println(" mm");
}