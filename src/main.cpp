#include <Arduino.h>
#include <ArduinoJson.h>

void setup() 
{
  Serial.begin(115200);
  uint32_t quantidade = 30UL;
  float temperatura1 = 2.2f;
  float temperatura2 = 25.4f;
  float temperatura3 = 29.8f;
  bool estaLigado = false;
  const char* nomeDispositivo = "ESP32S3";
  int pinosUsados[] = {1, 2, 5, 42, 35};

  //* O heap é a área da RAM usada para alocacão dinamica
  //* é reservada e liberada durante a execucão, conforme a necessidade
  //* Outras áreas da RAM:
  //* Memória estática guarda variáveis globais e static
  //* Pilha (stack) guarda variáveis locais das funcões

  Serial.printf("1- Heap livre: %u bytes\n\r", ESP.getFreeHeap());
  Serial.printf("2- Maior bloco de memória livre %u bytes\n\r", ESP.getMaxAllocHeap());

  JsonDocument doc;

  doc["quantidade"] = quantidade;
  doc["estaLigado"] = estaLigado;
  doc["nomeDispositivo"] = nomeDispositivo;


  #pragma region Array Vetores
  JsonArray pinos = doc["pinos"].to<JsonArray>();

  //* Jeito 1 para todos os casos
  size_t tamanhoVetor = sizeof(pinosUsados) / sizeof(pinosUsados[0]);
  for(int i = 0; i < tamanhoVetor; i++)
  {
    pinos.add(pinosUsados[i]);
  }
  //? is -> se for
  //? as -> sendo
  //? to -> para

  //* Jeito 2 apenas para números e booleanos
  pinos.clear();
  copyArray(pinosUsados, tamanhoVetor, pinos);
  #pragma endregion

  #pragma region Objetos
  JsonObject temps = doc["temperatura"].to<JsonObject>();
  temps["sensor1"] = temperatura1;
  temps["sensor2"] = temperatura2;
  temps["sensor3"] = temperatura3;
  #pragma endregion
  
  Serial.printf("1- Heap livre: %u bytes\n\r", ESP.getFreeHeap());
  Serial.printf("2- Maior bloco de memória livre %u bytes\n\r", ESP.getMaxAllocHeap());

  bool faltouMemoria = doc.overflowed();
  char* buffer [512];
  if(!faltouMemoria)
    serializeJsonPretty(doc, buffer, sizeof(buffer));

  JsonDocument doc2;
  JsonDocument filtroJson;

  filtroJson["estaLigado"] = true;
  filtroJson["temperatura"] = true;

  DeserializationError erroJson = deserializeJson(doc2, buffer, DeserializationOption::Filter(filtroJson));

  Serial.printf(doc2["estaLigado"].as<bool>() ? "EstaLigado" : "Esta Desligado");
  Serial.printf("Temp1: %5.2f;\n\rTemp2: %5.2f\n\rTemp3: %5.2f\n\r", 
    doc["temperatura"]["sensor1"].as<float>(),
    doc["temperatura"]["sensor2"].as<float>(),
    doc["temperatura"]["sensor3"].as<float>());

    if(doc2["nomeDispositivo"].is<const char*>())
    {
      Serial.printf("Nome do Dispositivo: %s", doc2["nomeDispositivo"].as<const char*>());
    }
    else
    {
      Serial.println("Nome do dispositivo não está neste doc");
    }
}

void loop() {}