/** 
 * @brief Código arduino para desbloquear uma trava elétrica de mola.
 * @authors Gabs-Coding, Palloma Cafe, Laura Santos, Pedro Werlang.
 * @version 1.0-alpha
 * @date 2024
 */

#include <SPI.h>
#include <MFRC522.h>

const int CS_PIN = 10; /**< Atribuição do pino 10 ao SPI slave select input (Pin 24, NSS, active low); */
const int RPD_PIN = 9; /**< Atribuição do pino 0 ao pino de reset e power down input (Pin 6, NRSTPD, active low); */
const int ATIVACAO_TRAVA_PIN = 2; /**< Atribuição do pino 2 para abrir e fechar a trava; */
const String CARTAO_CADASTRADO = "AL GU MC OD IG OO"; /**< Código UID do cartão cadastrado. */

void setup() {
	MFRC522 leitor_cartao(CS_PIN, RPD_PIN);
	String dados_lidos(""); /**< Variável que armazenará os dados lidos do cartão.*/
	Serial.begin(9600); /**< Iniciando a porta serial; */
	SPI.begin(); /**< Iniciando o SPI BUS; */
	leitor_cartao.PCD_Init(); /**< Iniciando a interface MFRC522 */
	pinMode(ATIVACAO_TRAVA_PIN, OUTPUT); /**< Definindo o modo do pino ATIVACAO_TRAVA_PIN para saída; */
}

void loop() {
	/**
	 * Esse if verifica se: 1 não há um cartão sem cadastro sendo lido; 2 não há um dados de um cartão sendo
   * lidos.
   */
	if (!leitor_cartao.PICC_IsNewCardPresent() || !leitor_cartao.PICC_ReadCardSerial()) {
		return;
	}
	/**
   * @brief Esse laço popula a variável "dados_lidos" com os dados lidos pelo leitor RFID de um cartão próximo.
   * @details Para melhor manipulação e visualização, o laõ a seguir concatena um espaço em branco e um zero,
   * caso o byte lido seja menor que 0x10 (HEX) (16 (BIN)); senão, apenas um espaço em branco é adicionado. Após
   * isso, o byte é concatenado em formato HEX na mesma string.
	 */
	for (byte i = 0; i < leitor_cartao.uid.size; i++) {
		dados_lidos.concat(String(leitor_cartao.uid.uidByte[i] < 0x10 ? " 0" : " "));
		dados_lidos.concat(String(leitor_cartao.uid.uidByte[i], HEX));
	}
	dados_lidos.toUpperCase();
	if (conteudo.substring(1) == CARTAO_CADASTRADO) {
    digitalWrite(ATIVACAO_TRAVA_PIN, HIGH); 
    delay(3000);           
    digitalWrite(ATIVACAO_TRAVA_PIN, LOW);  
  }
}

