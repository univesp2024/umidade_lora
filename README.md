# Monitoramento de Umidade do Solo via LoRa

**Projeto Integrador VI (2º Semestre de 2026)**

**Curso:** Engenharia da Computação – Universidade Virtual do Estado de São Paulo (UNIVESP)

Este repositório contém o código-fonte e a documentação do sistema de monitoramento remoto de umidade do solo. O projeto utiliza uma arquitetura IoT distribuída, onde um sensor de campo transmite dados via rádio LoRa para um gateway receptor, que por sua vez encaminha as informações para um servidor web via rede Wi-Fi, disponibilizando os dados em um *dashboard* gráfico em tempo real.

---

## Arquitetura do Sistema

O sistema é dividido em três camadas principais:

1. **Nó Transmissor (Edge/Campo):** Lê os dados analógicos do sensor de umidade do solo e transmite a porcentagem via protocolo LoRa a cada 10 segundos.
2. **Gateway Receptor (Master):** Recebe os pacotes LoRa, processa as informações e realiza requisições HTTP (POST) seguras para a API na nuvem.
3. **Backend e Frontend (Nuvem):** Uma API em PHP que persiste os dados em um banco de dados MySQL, e um *dashboard* em HTML/JS (Chart.js) que plota o histórico de umidade.

## Tecnologias Utilizadas

**Hardware**

* **Transmissor:** Heltec WiFi LoRa 32 V2 (ESP32 + SX1276)
* **Receptor:** Heltec WiFi LoRa 32 V3 (ESP32-S3 + SX1262)
* **Sensor:** Higrômetro de solo com módulo comparador LM393 (leitura via pino analógico)

**Software & Bibliotecas (C++)**

* Ambiente: Arduino IDE
* `LoRa.h` (Sandeep Mistry) para modulação do SX1276 (V2)
* `RadioLib` (Jan Gromeš) para modulação do SX1262 (V3)
* `Adafruit_SSD1306` e `Adafruit_GFX` para os displays OLED
* `WiFiClientSecure` e `HTTPClient` para comunicação com a API

**Backend & Web**

* **Linguagem:** PHP 8+
* **Banco de Dados:** MySQL / MariaDB (`geopo312_univesp`)
* **Frontend:** HTML5, CSS3 e JavaScript (Chart.js para gráficos)

---

## Esquema de Ligação (Pinout)

### Transmissor (Heltec V2)

| Componente LM393 | Pino Heltec V2 |
| --- | --- |
| VCC | 3.3V |
| GND | GND |
| A0 (Saída Analógica) | Pino 36 (VP) |

### Receptor (Heltec V3)

*Não requer sensores externos, opera apenas conectado à energia e rede Wi-Fi local.*

---

## Instalação e Configuração

### 1. Banco de Dados

Execute o script SQL abaixo no seu servidor MySQL para criar a estrutura necessária:

```sql
CREATE DATABASE IF NOT EXISTS geopo312_univesp DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE geopo312_univesp;

CREATE TABLE IF NOT EXISTS leituras (
    id INT AUTO_INCREMENT PRIMARY KEY,
    node_id VARCHAR(50) NOT NULL,
    umidade INT NOT NULL,
    data_hora DATETIME DEFAULT CURRENT_TIMESTAMP
);

```

### 2. Configuração Web (API e Dashboard)

1. Hospede os arquivos `api.php` e `index.php` em seu servidor web (ex: Apache/Nginx).
2. Edite as variáveis de conexão com o banco de dados (`$host`, `$dbname`, `$user`, `$pass`) no início de ambos os arquivos.
3. Certifique-se de que a API esteja acessível via HTTPS para a comunicação segura do ESP32.

### 3. Configuração do Firmware (ESP32)

1. Abra a Arduino IDE e instale as bibliotecas requeridas via *Gerenciador de Bibliotecas*.
2. **Transmissor:** Abra o sketch do Transmissor, selecione a placa *Heltec WiFi LoRa 32 V2* e faça o upload.
3. **Receptor:** Abra o sketch do Receptor, configure suas credenciais de rede nas variáveis `ssid` e `password`, e a URL da API na variável `serverName`. Selecione a placa *Heltec WiFi LoRa 32 V3* e faça o upload.

---

## Sincronização LoRa

Para garantir a comunicação entre chips diferentes (SX1276 na V2 e SX1262 na V3), ambos os códigos utilizam os seguintes parâmetros de rádio sincronizados estritamente nas bibliotecas correspondentes:

* **Frequência:** 915.0 MHz
* **Bandwidth:** 125 kHz
* **Spreading Factor (SF):** 7
* **Coding Rate:** 4/5
* **Sync Word:** 0x12

Deseja incluir no repositório uma seção dedicada às imagens (como fotos do hardware montado em campo, diagrama esquemático ou capturas de tela do dashboard funcionando)?
