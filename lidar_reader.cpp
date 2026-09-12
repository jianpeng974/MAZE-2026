#include "lidar_reader.h"

////// LidarPoint Methods

String LidarPoint::toString() const {
  String result = "(dist=";
  result += String(_distance);
  result += ", int=";
  result += String(_intensity);
  result += ", ang=";
  result += String(_angle / 100.0); // Mostra em graus decimais
  result += ")";
  return result;
}

////// Functions

uint8_t getPoints(LidarPoint* pointsArray) {
  // Procura o cabeçalho 0x54 0x2C ("T,")
  if (!SerialLidar.find("T,")) {
    return 0; 
  }

  byte buffer[45];
  size_t nbrBytesReceived = SerialLidar.readBytes(buffer, 45);

  if (nbrBytesReceived != 45) {
    return 0;
  }

  // Extrai dados do pacote
  uint16_t startAngle = _get2BytesLsbMsb(buffer, 2);
  uint16_t endAngle = _get2BytesLsbMsb(buffer, 40);
  uint8_t crcCheck = buffer[44];

  // Valida integridade dos dados
  if (_calCRC8FromBuffer(buffer, 44) == crcCheck) {
    uint16_t step = angleStep(startAngle, endAngle);

    for (unsigned int i = 0; i < 12; i++) {
      // Offset de 4 bytes iniciais + 3 bytes por ponto (distancia 2 bytes + intensidade 1 byte)
      int offset = 4 + (i * 3);
      uint16_t dist = _get2BytesLsbMsb(buffer, offset);
      uint8_t intens = buffer[offset + 2];
      float ang = angleFromStep(startAngle, step, i);

      // Preenche o array fornecido pelo usuário
      pointsArray[i] = LidarPoint(dist, intens, ang);
    }
    return 12; // Retorna que 12 pontos foram processados
  }
  
  return 0; 
}

uint8_t _calCRC8FromBuffer(uint8_t* p, uint8_t lenWithoutCRCCheckValue) {
  uint8_t crc = 0xD8; 
  for (uint16_t i = 0; i < lenWithoutCRCCheckValue; i++) {
    crc = crcTable[(crc ^ *p++) & 0xff];
  }
  return crc;
}

uint16_t _get2BytesLsbMsb(byte buffer[], int index) {
  return (buffer[index + 1] << 8) | buffer[index];
}

uint16_t angleStep(uint16_t startAngle, uint16_t endAngle, unsigned int lenMinusOne) {
  if (startAngle <= endAngle) {
    return (endAngle - startAngle) / lenMinusOne;
  } else {
    return (36000 + endAngle - startAngle) / lenMinusOne;
  }
}

uint16_t angleFromStep(uint16_t startAngle, uint16_t step, unsigned int indice) {
  return (startAngle + (step * indice)) % 36000;
}