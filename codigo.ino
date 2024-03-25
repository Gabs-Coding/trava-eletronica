/** 
 * Código arduino para desbloquear uma trava elétrica de mola.
 * O projeto usará um cartão RFID MIFARE para que o desbloqueio da tranca seja feito.
 */

#include <SPI.h>
#include <MFRC522.h>

const int CS_PIN = 10; // Atribuição do pino 10 ao SPI slave select input (Pin 24, NSS, active low);
const int RPD_PIN = 9; // Atribuição do pino 0 ao pino de reset e power down input (Pin 6, NRSTPD, active low);
const int ATIVACAO_TRAVA_PIN = 2; // Atribuição do pino 2 para abrir e fechar a trava;

void setup() {
	MFRC522 leitor_cartao(CS_PIN, RPD_PIN);
	Serial.begin(9600); // Iniciando a porta serial;
	SPI.begin(); // Iniciando o SPI BUS;
	leitor_cartao.PCD_Init(); // Iniciando a interface MFRC522
	pinMode(ATIVACAO_TRAVA_PIN, OUTPUT); // Definindo o modo do pino ATIVACAO_TRAVA_PIN para saída;
}

void loop() {
	/**
	 * Esse if verifica se: 1 não há um cartão sem cadastro sendo lido; 2 não há um dados de um cartão sendo
   * lidos.
   */
	if (!leitor_cartao.PICC_IsNewCardPresent() || !leitor_cartao.PICC_ReadCardSerial()) {
		return;
	}
	
}

