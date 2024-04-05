/** 
 * @brief Código arduino para desbloquear uma trava elétrica de mola.
 * @authors Gabs-Coding, Palloma Cafe, Laura Santos, Pedro Werlang.
 * @version 1.0-alpha
 * @date 2024
 */

#include <SPI.h>
#include <MFRC522.h>

const int CS_PIN = 10; /**< Atribuição do pino vinculado ao SPI slave select input (Pin 24, NSS, active low); */
const int RPD_PIN = 9; /**< Atribuição do pino vinculado ao reset e power down input (Pin 6, NRSTPD, active low); */
const int ISR_PIN = 4; /**< Atribuição do pino vinculado a interrupção do botão que abrirá a trava solenoide. */
const int ATIVACAO_TRAVA_PIN = 2; /**< Atribuição do pino 2 para abrir e fechar a trava; */
const int LED_PORTA_ABERTA = 3;
const String CARTAO_CADASTRADO("20 AE B5 56"); /**< Código UID do cartão cadastrado. */

MFRC522 leitor_cartao(CS_PIN, RPD_PIN);
String dados_lidos(""); /**< Variável que armazenará os dados lidos do cartão.*/

void setup() {
	Serial.begin(9600); /**< Iniciando a porta serial; */
	SPI.begin(); /**< Iniciando o SPI BUS; */
	leitor_cartao.PCD_Init(); /**< Iniciando a interface MFRC522 */
	pinMode(ATIVACAO_TRAVA_PIN, OUTPUT); /**< Definindo o modo do pino ATIVACAO_TRAVA_PIN para saída; */
	pinMode(LED_PORTA_ABERTA, OUTPUT);
	attachInterrupt(digitalPinToInterrupt(ISR_PIN), destravar_sem_cartao, HIGH);
}

void loop() {
	/**
	 * Verificando se: 1 não há um cartão sem cadastro sendo lido; 2 não há dados de um cartão sendo
   * lidos.
   */
	if (!leitor_cartao.PICC_IsNewCardPresent()) {
		return;
	}
	if (!leitor_cartao.PICC_ReadCardSerial()) {
		return;
	}
	/**
   * @brief Esse laço popula a variável "dados_lidos" com os dados lidos pelo leitor RFID de um cartão próximo.
   * @details Para melhor manipulação e visualização, o laço a seguir concatena um espaço em branco e um zero,
   * caso o byte lido seja menor que 0x10 (HEX) (16 (BIN)); senão, apenas um espaço em branco é adicionado. Após
   * isso, o byte é concatenado em formato HEX na mesma string.
	 */
	for (byte i = 0; i < leitor_cartao.uid.size; i++) {
		Serial.print(leitor_cartao.uid.uidByte[i] < 0x10 ? " 0" : " ");
		Serial.print(leitor_cartao.uid.uidByte[i], HEX);
		dados_lidos.concat(String(leitor_cartao.uid.uidByte[i] < 0x10 ? " 0" : " "));
		dados_lidos.concat(String(leitor_cartao.uid.uidByte[i], HEX));
	}
	dados_lidos.toUpperCase(); /**< Garante que os dados lidos estejam em caixa alta.*/
	/**
	 * @brief Lógica de checagem se os cartão lido está cadastrado.
	 * @details Caso a validação seja bem sucedida, a trava será destrancada.
	 */
	if (dados_lidos.substring(1) == CARTAO_CADASTRADO.substring(1)) {
		abrirTrava();
  } else {
		return;
	}
}


/**
 * @brief abre a trava solenoide por 3 segundos.
 * @details muda o estado do pino "ATIVACAO_TRAVA_PIN" e "LED_PORTA_ABERTA" para "HIGH", aguarda 3 segundos
 * e muda o estado do pino para "LOW" encerrando o procedimento.
 */
void abrirTrava() {
	digitalWrite(ATIVACAO_TRAVA_PIN, HIGH);
	digitalWrite(LED_PORTA_ABERTA, HIGH);
	delay(3000);           
	digitalWrite(ATIVACAO_TRAVA_PIN, LOW);
	digitalWrite(LED_PORTA_ABERTA, LOW);
}

/**
 * @brief faz a chamada do procedimento "abrirTrava()".
 * @warning pode haver um possível bug na chamada do procedimento "abrirTrava()", pois interrupções não 
 * permitem o bom funcionamento do procedimento nativo "delay()".
 */
void destravar_sem_cartao() {
	abrirTrava();
}
