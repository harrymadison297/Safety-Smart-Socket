#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_err.h"

#include "ade9153a.h"
#include "my_ade9153.h"

/* Varible */
spi_device_handle_t spi;

/*********************************************************
 * Function
 ********************************************************/

/* Function SPI */

/* Init spi */
bool init_spiADE9153(uint8_t xSPI_HOST, int8_t SPI_PIN_CS, uint32_t SPI_CLK) {
	esp_err_t ret;
	int8_t pinMISO, pinMOSI, pinCLK, pinCS;

	/* Select SPI Pin */
	if (xSPI_HOST == HSPI_HOST) {
		pinMISO = PIN_HSPI_MISO;
		pinMOSI = PIN_HSPI_MOSI;
		pinCLK = PIN_HSPI_CLK;
		pinCS = PIN_HSPI_CS;
	} else if (xSPI_HOST == VSPI_HOST) {
		pinMISO = PIN_VSPI_MISO;
		pinMOSI = PIN_VSPI_MOSI;
		pinCLK = PIN_VSPI_CLK;
		pinCS = PIN_VSPI_CS;
	} else {
		return false;
	}
	if (SPI_PIN_CS >= 0) {
		pinCS = SPI_PIN_CS;
	}

	/* Config bus */
	spi_bus_config_t buscfg = { .mosi_io_num = pinMOSI, .miso_io_num = pinMISO,
			.sclk_io_num = pinCLK, .quadhd_io_num = -1, .quadwp_io_num = -1,
			.max_transfer_sz = 10 };

	/* Config slave interface */
	spi_device_interface_config_t devcfg =
			{ .command_bits = 0, .address_bits = 0, .dummy_bits = 0,
					.queue_size = 2, .clock_speed_hz = SPI_CLK,
					.duty_cycle_pos = 128, .mode = 0, .spics_io_num = pinCS,
					.flags = 0 };

	/* Init spi bus */
	ret = spi_bus_initialize(xSPI_HOST, &buscfg, DMA_CHAN);
	assert(ret==ESP_OK);
	if (ret != ESP_OK) {
		return false;
	}
	/* Add slave to spi bus */
	ret = spi_bus_add_device(xSPI_HOST, &devcfg, &spi);
	assert(ret==ESP_OK);
	if (ret != ESP_OK) {
		return false;
	}
	/* Pullup pin SPI */
	gpio_set_pull_mode(PIN_VSPI_MISO, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(PIN_VSPI_CLK, GPIO_PULLDOWN_ONLY);
	gpio_set_pull_mode(PIN_VSPI_CS, GPIO_PULLUP_ONLY);

	/* Run DSP */
	spi_write16(REG_RUN, ADE9153_RUN);

	return true;
}

/* Format: ||ADDRESS|/W|xxx||DATA-16BIT|| */
void spi_write16(uint16_t address, uint16_t data) {
	esp_err_t ret;
	uint16_t cmd_hdr;
	uint8_t sendPkt[4];
	uint8_t recvPkt[4];

	cmd_hdr = (address << 4) & ~(1 << CMD_BIT);
	sendPkt[0] = (cmd_hdr >> 8) & 0xff;
	sendPkt[1] = (cmd_hdr >> 0) & 0xff;
	sendPkt[2] = (data >> 8) & 0xff;
	sendPkt[3] = (data >> 0) & 0xff;
	memset(recvPkt, 0, sizeof(recvPkt));

	spi_transaction_t trs;
	memset(&trs, 0, sizeof(trs));
	trs.length = CMD_16BIT;
	trs.tx_buffer = sendPkt;
	trs.rx_buffer = recvPkt;
	ret = spi_device_polling_transmit(spi, &trs);
	assert(ret == ESP_OK);
}

/* Format: ||ADDRESS|/W|xxx||DATA-32BIT|| */
void spi_write32(uint16_t address, uint32_t data) {
	esp_err_t ret;
	uint16_t cmd_hdr;
	uint8_t sendPkt[6];
	uint8_t recvPkt[6];

	cmd_hdr = (address << 4) & ~(1 << CMD_BIT);
	sendPkt[0] = (cmd_hdr >> 8) & 0xff;
	sendPkt[1] = (cmd_hdr >> 0) & 0xff;
	sendPkt[2] = (data >> 24) & 0xff;
	sendPkt[3] = (data >> 16) & 0xff;
	sendPkt[4] = (data >> 8) & 0xff;
	sendPkt[5] = (data >> 0) & 0xff;
	memset(recvPkt, 0, sizeof(recvPkt));

	spi_transaction_t trs;
	memset(&trs, 0, sizeof(trs));
	trs.length = CMD_32BIT; //CMD_16BIT + CMD_32BIT;
	trs.tx_buffer = sendPkt;
	trs.rx_buffer = recvPkt;
	ret = spi_device_polling_transmit(spi, &trs);
	assert(ret == ESP_OK);
}

/* Format: ||ADDRESS|/R|xxx||DATA-16BIT|| */
uint16_t spi_read16(uint16_t address) {
	esp_err_t ret;
	uint16_t cmd_hdr;
	uint8_t sendPkt[4];
	uint8_t recvPkt[4];
	uint16_t res;

	cmd_hdr = (address << 4) | (1 << CMD_BIT);
	sendPkt[0] = (cmd_hdr >> 8) & 0xFF;
	sendPkt[1] = (cmd_hdr >> 0) & 0xFF;
	sendPkt[2] = 0x00;
	sendPkt[3] = 0x00;

	spi_transaction_t trs;
	memset(&trs, 0, sizeof(trs));
	trs.length = CMD_16BIT; /* CMD_16BIT + CMD_16BIT; */
	trs.tx_buffer = sendPkt;
	trs.rx_buffer = recvPkt;
	ret = spi_device_polling_transmit(spi, &trs);
	assert(ret == ESP_OK);
	res = (recvPkt[2] << 8) | (recvPkt[3] << 0);
	return res;
}

/* Format: ||ADDRESS|/R|xxx||DATA-32BIT|| */
uint32_t spi_read32(uint16_t address) {
	esp_err_t ret;
	uint16_t cmd_hdr;
	uint8_t sendPkt[6];
	uint8_t recvPkt[6];
	uint32_t res;

	cmd_hdr = (address << 4) | (1 << CMD_BIT);
	sendPkt[0] = (cmd_hdr >> 8) & 0xFF;
	sendPkt[1] = (cmd_hdr >> 0) & 0xFF;
	for (uint8_t i = 2; i < 6; i++) {
		sendPkt[i] = 0x00;
	}

	spi_transaction_t trs;
	memset(&trs, 0, sizeof(trs));
	trs.length = CMD_32BIT; //CMD_16BIT + CMD_32BIT;
	trs.tx_buffer = sendPkt;
	trs.rx_buffer = recvPkt;
	ret = spi_device_polling_transmit(spi, &trs);
	assert(ret == ESP_OK);
	res = (recvPkt[2] << 24) | (recvPkt[3] << 16) | (recvPkt[4] << 8)
			| (recvPkt[5] << 0);
	return res;
}

/* function ade9153 */
float calculate_target_aicc(float rShunt, float PGAGain) {
	float targetAICC;
	targetAICC = 13.41105 / (rShunt * PGAGain); // 10^9/(52725703*sqrt(2)) = 13.41105
	return targetAICC;
}

float calculate_target_avcc(float rBig, float rSmall, float *vHeadRoom) {
	float targetAVCC;
	// *vHeadRoom = (rBig/rSmall + 1)*0.5/(VNOM*sqrt(2));
	*vHeadRoom = (rBig / rSmall + 1) * 0.35355 / VNOM; // 0.5/sqrt(2) = 0.35355
	// targetAVCC = (pow(10,9))*(VNOM*(*vHeadRoom))/26362852
	targetAVCC = 37.93216 * (VNOM * (*vHeadRoom)); // 10^9/26362852 = 37.93216
	return targetAVCC;
}

float calculate_target_powCC(float targetAICC, float targetAVCC) {
	float targetPowCC;
	// targetPowCC = targetAICC*targetAVCC*(pow(2,27))*(pow(10,-18))*(pow(10,6));
	targetPowCC = targetAICC * targetAVCC * 0.0001342;
	return targetPowCC;
}

void ADE9153_reset(uint8_t pinReset) {
	static uint8_t initReset = 0;
	if (!initReset) {
		gpio_config_t cfgReset = { .mode = GPIO_MODE_OUTPUT, .intr_type =
				GPIO_INTR_DISABLE, .pin_bit_mask = (1 << pinReset) };
		gpio_config(&cfgReset);
		initReset = 1;
	}
	gpio_set_level(pinReset, 0);
	vTaskDelay(100 / portTICK_PERIOD_MS);
	gpio_set_level(pinReset, 1);
	vTaskDelay(100 / portTICK_PERIOD_MS);
}

void ADE9153_initCFG() {
	spi_write16(REG_AI_PGAGAIN, ADE9153_AI_PGAGAIN16N);
	spi_write32(REG_VDIV_RSMALL, ADE9153_VDIV_RSMALL);
	spi_write16(REG_COMPMODE, ADE9153_COMPMODE);
	spi_write16(REG_CONFIG2, ADE9153_CONFIG2);
	spi_write16(REG_ACCMODE, ADE9153_ACCMODE);
	spi_write32(REG_VLEVEL, ADE9153_VLEVEL);  
	spi_write32(REG_CONFIG0, ADE9153_CONFIG0);
	spi_write32(REG_AIRMS_OS, 0xFFFFFFF5);
	spi_write32(REG_AVRMS_OS, 0x00000000);
	spi_write16(REG_EP_CFG, ADE9153_EP_CFG);
}

void ADE9153_lock() {
	spi_write16(REG_WR_LOCK, ADE9153_LOCK);
}

void ADE9153_unLock() {
	spi_write16(REG_WR_LOCK, ADE9153_UNLOCK);
}

bool ADE9153_acal_AINormal() {
	uint32_t mSReady = 0;
	mSReady = spi_read32(REG_MS_STATUS_CURRENT);
	if (mSReady == 0xFFFFFFFF) {
		return false;
	}
	if ((ADE9153_MS_STATUS_CURRENT & mSReady) == 0) {
		return false;
	}
	spi_write32(REG_MS_ACAL_CFG, ADE9153_ACAL_AI_NORMAL);
	return true;
}

bool ADE9153_acal_AITurbo() {
	uint32_t mSReady = 0;
	mSReady = spi_read32(REG_MS_STATUS_CURRENT);
	if (mSReady == 0xFFFFFFFF) {
		return false;
	}
	if ((ADE9153_MS_STATUS_CURRENT & mSReady) == 0) {
		return false;
	}
	spi_write32(REG_MS_ACAL_CFG, ADE9153_ACAL_AI_TURBO);
	return true;
}

bool ADE9153_acal_AV() {
	uint32_t mSReady = 0;
	mSReady = spi_read32(REG_MS_STATUS_CURRENT);
	if (mSReady == 0xFFFFFFFF) {
		return false;
	}
	if ((ADE9153_MS_STATUS_CURRENT & mSReady) == 0) {
		return false;
	}
	spi_write32(REG_MS_ACAL_CFG, ADE9153_ACAL_AV);
	return true;
}

void ADE9153_acal_stop() {
	spi_write32(REG_MS_ACAL_CFG, ADE9153_ACAL_STOP);
}

void ADE9153_AIGainCFG(float targetAICC, float mSureAICC) {
	int32_t AIGain = 0;
	// AIGain = (mSureAICC / targetAICC - 1)*2^27
	AIGain = ((-mSureAICC) / targetAICC - 1) * 134217728; // debug av-ade9153a
	printf("AIGain: %lx - %ld\n", AIGain, AIGain);
	spi_write32(REG_AIGAIN, AIGain);
}

void ADE9153_AVGainCFG(float targetAVCC, float mSureAVCC) {
	int32_t AVGain = 0;
	// AVGain = (mSureAVCC / targetAVCC - 1)*2^27
	AVGain = (mSureAVCC / targetAVCC - 1) * 134217728;  //2^27 = 134217728
	printf("AVGain: %ld\n", AVGain);
	spi_write32(REG_AVGAIN, AVGain);
}

// Format Reg: 21.11
void ADE9153_acal_result(ACALRegs_t *Data) {
	int32_t tempReg;
	float tempValue;

	tempReg = (int32_t) spi_read32(REG_MS_ACAL_AICC);
	Data->mSureAICCReg = tempReg;
	printf("mSureAICCReg: %lx\n", Data->mSureAICCReg);
	// tempValue = (float)tempReg/(float)2^11
	tempValue = (float) tempReg / (float) 2048;  //2^11 = 2048
	Data->mSureAICCValue = tempValue;
	printf("mSureAICCValue: %f\n", Data->mSureAICCValue);
	tempReg = (int32_t) spi_read32(REG_MS_ACAL_AICERT);
	Data->mSureAICERTReg = tempReg;
	printf("mSureAICERTReg: %ld\n", Data->mSureAICERTReg);

	tempReg = (int32_t) spi_read32(REG_MS_ACAL_AVCC);
	Data->mSureAVCCReg = tempReg;
	printf("mSureAVCCReg: %lx\n", Data->mSureAVCCReg);
	// tempValue = (float)tempReg/(float)2^11
	tempValue = (float) tempReg / (float) 2048;  //2^11 = 2048
	Data->mSureAVCCValue = tempValue;
	printf("mSureAVCCValue: %f\n", Data->mSureAVCCValue);
	tempReg = (int32_t) spi_read32(REG_MS_ACAL_AVCERT);
	Data->mSureAVCERTReg = tempReg;
	printf("mSureAVCERTReg: %ld\n", Data->mSureAVCERTReg);
}

void ADE9153_read_RMSRegs(RMSRegs_t *Data) {
	int32_t tempReg;
	float tempValue;
	float tempXCC;

	tempXCC = Data->targetAICC;
	tempReg = (int32_t) spi_read32(REG_AIRMS);
	Data->AIReg = tempReg;
	tempValue = (float) tempReg * tempXCC / 1000000;  //mA
	Data->AIValue = tempValue;

	tempXCC = Data->targetAVCC;
	tempReg = (int32_t) spi_read32(REG_AVRMS);
	Data->AVReg = tempReg;
	tempValue = (float) tempReg * tempXCC / 1000000 / 1000;  //mV => V
	Data->AVValue = tempValue;
}

void ADE9153_read_PowRegs(PowRegs_t *Data) {
	int32_t tempReg;
	float tempValue;
	float tempXCC;

	tempXCC = Data->targetPowCC;
	tempReg = (int32_t) spi_read32(REG_AWATT);
	Data->activeReg = tempReg;
	tempValue = (float) tempReg * tempXCC / 1000 / 1000;  //mW => W
	Data->activeValue = tempValue;

	tempReg = (int32_t) spi_read32(REG_AFVAR);
	Data->reactiveReg = tempReg;
	tempValue = (float) tempReg * tempXCC / 1000 / 1000;  //mAVR => AVR
	Data->reactiveValue = tempValue;

	tempReg = (int32_t) spi_read32(REG_AVA);
	Data->apparentReg = tempReg;
	tempValue = (float) tempReg * tempXCC / 1000 / 1000;  //mAV => AV
	Data->apparentValue = tempValue;
}

void ADE9153_read_PQRegs(PQRegs_t *Data) {
	int32_t tempReg;
	float tempValue;
	uint16_t tempFreq;
	float constValue;

	tempReg = (int32_t) spi_read32(REG_APF);
	Data->pwFactorReg = tempReg;
	// tempValue = (float)tempReg/(float)2^27
	tempValue = (float) tempReg / (float) 134217728;  //2^27 = 134217728
	Data->pwFactorValue = tempValue;

	tempReg = (int32_t) spi_read32(REG_APERIOD);
	Data->periodReg = tempReg;
	tempValue = (float) (4000 * 65536) / (float) (tempReg + 1);
	/* Fix value power factor > 1 */
	if ((tempValue >= 1)||(tempValue <= -1)) {
		tempValue = 0;
	}
	Data->freqValue = tempValue;

	tempFreq = spi_read16(REG_ACCMODE);
	if ((tempFreq & 0x10) == 0) {     // 0b00010000 or (1<<4)
		constValue = 0.017578125;  // 50Hz
	} else {
		constValue = 0.02109375;   // 60Hz
	}
//	printf("constValue: %f\n", constValue);
	tempReg = (int16_t) spi_read16(REG_ANGL_AV_AI);
	Data->angleAV_AIReg = tempReg;
	tempValue = (float) tempReg * constValue;
	Data->angleAV_AIValue = tempValue;
}

