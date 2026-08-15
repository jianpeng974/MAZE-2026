#include "lidar_reader.h"

// Buffer fixo para receber os 12 pontos de cada pacote
LidarPoint pontosDoPacote[12];

int dir_atual;
int lin = 20;
int col = 20;
int matriz[lin][col] = 0;
int pos_x, pos_y;

int contador_mov = 0;

char vetor_mov[50] = {}

//estabelecer a direçao que esta    //direita e esquerda (sentido)
int atualizar_dir(int dir_atual, char sentido) {
  if (sentido == "e") {
    dir_atual -= 1;
    if (dir_atual == 0) {
      dir_atual = 4;
    }
  } else if (sentido == "d") {
    dir_atual += 1;
    if (dir_atual == 5) {
      dir_atual = 1;
    }
  }
  return dir_atual;
}



/*
1-frente
2-direita
3-tras
4-esquerda
*/

//componentes em direçoes atuais

//usar p
void atualiza_posicao(int matriz[lin][col], int pos_x, int pos_y, int dir_atual, char sentido) {
  if (dir_atual == 1) {  //frente   
    if (sentido == "f") {
      pos_y -= 1;
    } else if (sentido == "d") {
      pos_x += 1;
    } else if (sentido == "t") {
      pos_y += 1;
    } else if (sentido == "e") {
      pos_x -= 1;
    }
  }

  else if (dir_atual == 2) {  //direita
    if (sentido == "f") {
      pos_x += 1;
    } else if (sentido == "d") {
      pos_y += 1;
    } else if (sentido == "t") {
      pos_x -= 1;
    } else if (sentido == "e") {
      pos_y -= 1;
    }
  }

  else if (dir_atual == 3) {  //tras
    if (sentido == "f") {
      pos_y += 1;
    } else if (sentido == "d") {
      pos_x -= 1;
    } else if (sentido == "t") {
      pos_y -= 1;
    } else if (sentido == "e") {
      pos_x += 1;
    }
  }

  else if (dir_atual == 4) {  //esquerda
    if (sentido == "f") {
      pos_x -= 1;
    } else if (sentido == "d") {
      pos_y -= 1;
    } else if (sentido == "t") {
      pos_x += 1;
    } else if (sentido == "e") {
      pos_y += 1;
    }
  }
  matriz[pos_x][pos_y] = 1;
}

//SEGUIDOR BURRO

//esse seguidor precisa de uma matriz:
//metas:

//-colocar o robo no meio e setar como 0.0
//-representa se a matriz ja foi visitada ou nao
// Margem de erro para capturar o ângulo (na ESP32 podemos usar 0.6 devido à maior velocidade)


const float margem = 0.6;

uint16_t dist_n = 0;

uint16_t dist_s = 0;

uint16_t dist_l = 0;

uint16_t dist_o = 0;

int tam_vet = 50;

char vet_mov [tam_vet] = 0;

//revisar sabado

bool nao_visitado(int matriz[lin][col], int pos_x, int pos_y, int dir_atual, char sentido) {
  // confere se ja foi visitado ou nao
  //checa suas direçoes se ja foi visitados ou nao
  //fazer parecido com o de cima 
  // diferente de !=0
  // ve a posiçao na matriz
  if (dir_atual == 1) {  //frente
    if (sentido == "f" && (matriz[pos_X][pos_y + 1] == 0) ||

        (sentido == "t" && matriz[pos_X][pos_y - 1] == 0) ||

        (sentido == "d" && matriz[pos_X + 1][pos_y] == 0) ||

        (sentido == "e" && matriz[pos_X - 1][pos_y] == 0)) {  //frente
      return 1;
    }
  }

  //ver se posiçao ja esta sendo mudadaa
  else if (dir_atual == 2) {  //frente
    if (sentido == "f" && (matriz[pos_X + 1][pos_y] == 0) ||

        (sentido == "t" && matriz[pos_X - 1][pos_y] == 0) ||

        (sentido == "d" && matriz[pos_X][pos_y - 1] == 0) ||

        (sentido == "e" && matriz[pos_X][pos_y + 1] == 0)) {  //frente
      return 1;
    }
  }

  //ver se posiçao ja esta sendo mudadaa
  else if (dir_atual == 3) {  //frente
    if (sentido == "f" && (matriz[pos_X][pos_y - 1] == 0) ||

        (sentido == "t" && matriz[pos_X][pos_y + 1] == 0) ||

        (sentido == "d" && matriz[pos_X - 1][pos_y] == 0) ||

        (sentido == "e" && matriz[pos_X + 1][pos_y] == 0)) {  //frente
      return 1;
    }
  }

  //ver se posiçao ja esta sendo mudadaa
  else if (dir_atual == 4) {  //frente
    if (sentido == "f" && (matriz[pos_X - 1][pos_y] == 0) ||

        (sentido == "t" && matriz[pos_X + 1][pos_y] == 0) ||

        (sentido == "d" && matriz[pos_X][pos_y + 1] == 0) ||

        (sentido == "e" && matriz[pos_X][pos_y - 1] == 0)) {  //frente
      return 1;
    }
  }
  return 0;
}














// fazer lista de movimentos dps
void scan() {
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
          dist_n = distancia;
        }
        // Verifica proximidade do LESTE (90°)
        else if (abs(anguloAtual - 90.0) < margem) {
          imprimirPonto("LESTE (90°)", anguloAtual, distancia);
          dist_l = distancia;
        }
        // Verifica proximidade do SUL (180°)
        else if (abs(anguloAtual - 180.0) < margem) {
          imprimirPonto("SUL (180°)", anguloAtual, distancia);
          dist_s = distancia;

        }
        // Verifica proximidade do OESTE (270°)
        else if (abs(anguloAtual - 270.0) < margem) {
          imprimirPonto("OESTE (270°)", anguloAtual, distancia);
          dist_o = distancia
        }  // caso der errado escrever uma lista com os dist_-- e retorna ela
      }
    }
  }
}















void setup() {

  //centro visitado
  pos_x = lin / 2;
  pos_y = col / 2;

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

  for (int l = 0; l <= lin - 1; l++) {
    for (int c = 0; c <= col - 1; c++) {
      matriz[l][c] = 0;
    }
  }
  matriz[pos_x][pos_y] = 1;
}

//Mapeamento Burro

//-melhorar o if junto com os blocos visitados ou nao
//- se  parede && visitado ...

void loop() {

  //atualiza vetor 
  
  scan();

  //posicao x = coluna
  //posicao y = linha

  if (dist_l >= 20 && nao_visitado(matriz[pos_y][pos_x], pos_x, pos_y, dir_atual, "d")) {
    dir(1800);  //subistituir por constante quando tiver
    vetor_mov[contador_mov] = "d";
    contador_mov += 1;
    dir_atual = atualizar_dir(dir_atual, "d");  //*** sobre atualizar os valores
    frente(1800);
    vetor_mov[contador_mov] = "f";
    contador_mov += 1;
  }

  else if (dist_n >= 20 && nao_visitado(matriz[pos_y][pos_x], pos_x, pos_y, dir_atual, "f")) {
    frente(1800);  //frente
    vetor_mov[contador_mov] = "f";
    contador_mov += 1;
  }

  else if (dist_o >= 20 && nao_visitado(matriz[pos_y][pos_x], pos_x, pos_y,   dir_atual, "e")) {
    esq(1800);  //esquerda
    vetor_mov[contador_mov] = "e";
    contador_mov += 1;
    dir_atual = atualizar_dir(dir_atual, "e");  //*** sobre atualizar os valores
    frente(1800);
    vetor_mov[contador_mov] = "f";
    contador_mov += 1;
  }

  else {
    tras(1800)//tras
    contador_mov -= 1;
    vetor_mov[contador_mov] = "t";
    contador_mov += 1;
    
  }
}




// Função auxiliar para exibição formatada
void imprimirPonto(String direcao, float angulo, uint16_t dist) {
  Serial.print("[");
  Serial.print(direcao);
  Serial.print("] Ang: ");
  Serial.print(angulo, 2);  // 2 casas decimais
  Serial.print("° | Dist: ");
  Serial.print(dist);
  Serial.println(" mm");
}
