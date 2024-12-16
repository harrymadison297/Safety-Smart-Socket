#ifndef _SP_ADE9153_H_
#define _SP_ADE9153_H_

#include <stdint.h>
#include <stdbool.h>
#include "ade9153a.h"
#include "driver/spi_master.h"

/**************************************************************
 * Definitions
 *************************************************************/

/* Definitions SPI */
// Pin VSPI
#define PIN_VSPI_MOSI 12
#define PIN_VSPI_MISO 34
#define PIN_VSPI_CLK  25
#define PIN_VSPI_CS   23

// Pin HSPI
#define PIN_HSPI_MOSI 13
#define PIN_HSPI_MISO 12
#define PIN_HSPI_CLK  14
#define PIN_HSPI_CS   15

// DMA CHANNEL
#define DMA_CHAN 0

// Transaction bits
#define CMD_16BIT 32
#define CMD_32BIT 48

// CMD_R/W
#define CMD_BIT 3
//#define CMD_WRITE 0xFFF0
//#define CMD_READ 0xFFF8

/* Definitions ADE9153 */
#define VNOM 220 // Vnom = 220
// Definition general
#define ADE9153_AI_PGAGAIN16N 0x0002 // AI_PGAGAIN = 16, Signal on IAN
#define ADE9153_AI_PGAGAIN16 0x0012 // AI_PGAGAIN = 16, Signal on IAP
#define ADE9153_AI_PGAGAIN24 0x0013 // AI_PGAGAIN = 24, Signal on IAP
#define ADE9153_AI_PGAGAIN32 0x0014 // AI_PGAGAIN = 32, Signal on IAP

#define ADE9153_VDIV_RSMALL 0x000003E8 // RSMALL = 1000 Ohm

#define ADE9153_COMPMODE 0x0005 // Computation mode register set 0x0005
#define ADE9153_CONFIG2 0x0C00//0x0A00 // High-pass filter frequence 1.2475 Hz
#define ADE9153_ACCMODE 0x0000 // System frequency 50Hz, Fundamental reactive power &Total active power Signed accumulation mode
#define ADE9153_VLEVEL 0x0022EA28 // VLEVEL = 2 * 1144084
#define ADE9153_CONFIG0 0x00000000 // ZX_SRC_SEL = 0, Zero-crossing After the high-pass filter and phase compensation. ‬
#define ADE9153_RUN 0x0001 // DSP on
#define ADE9153_EP_CFG 0x0001 // Allow power factor opetator

// Definition mSure Autocalibration
#define ADE9153_MS_STATUS_CURRENT 0x00000001 // mSure ready for run autocalibration
#define ADE9153_ACAL_AV 0x00000043 // AUTOCAL_AV=1, ACAL_RUN =1, ACAL_MODE=1
#define ADE9153_ACAL_AI_NORMAL 0x00000013 // AUTOCAL_AI=1, ACALMODE_AI=0, ACAL_RUN=1, ACAL_MODE=1
#define ADE9153_ACAL_AI_TURBO 0x00000017 // AUTOCAL_AI=1, ACALMODE_AI=1, ACAL_RUN=1, ACAL_MODE=1
#define ADE9153_ACAL_STOP 0x00000000//0x00000001 // Stop mSure autocalibration

// Lock/unLock ADE9153 from Address 0x000 to Address 0x073 and Address 0x400 to Address 0x4FE.
#define ADE9153_LOCK 0x3C64
#define ADE9153_UNLOCK 0x4AD1

/**************************************************************
 * Structure and global variable
 *************************************************************/
typedef struct {
	int32_t AIReg;
	float AIValue;
	float targetAICC;  // nA/code
	int32_t AVReg;
	float AVValue;
	float targetAVCC;  // nV/code
} RMSRegs_t;

typedef struct {
	int32_t activeReg;
	float activeValue;
	int32_t reactiveReg;
	float reactiveValue;
	int32_t apparentReg;
	float apparentValue;
	float targetPowCC;  // uW/code
} PowRegs_t;

typedef struct {
	int32_t pwFactorReg;
	float pwFactorValue;
	int32_t periodReg;
	float freqValue;
	int32_t angleAV_AIReg;
	float angleAV_AIValue;
} PQRegs_t;

typedef struct{
	int32_t mSureAICCReg;    // format 21.11
	float mSureAICCValue;    // nA/code
	int32_t mSureAICERTReg;
	int32_t mSureAVCCReg;    // format 21.11
	float mSureAVCCValue;    // nV/code
	int32_t mSureAVCERTReg;
} ACALRegs_t;

/**************************************************************
 * Functions
 *************************************************************/
/* function spi */

/*********************************************
 * Init SPI
 * Default: MSB first, mode0, 4wire-halfduplex
 * Config: SPI_HOST(HSPI-VSPI), PIN_CS, SPI_CLK
 */
bool init_spiADE9153(uint8_t xSPI_HOST, int8_t SPI_CS, uint32_t SPI_CLK);

/*********************************************
 * Write SPI
 * Format:
 *    CMD_HDR-16bit: |ADDRESS|/W|xxx|
 *    Data-16bit: |DATA-16BIT|
 *    Data-32bit: |DATA-32BIT|
 */
void spi_write16(uint16_t address, uint16_t data);
void spi_write32(uint16_t address, uint32_t data);

/*********************************************
 * Read SPI
 * Format:
 *    CMD_HDR-16bit: |ADDRESS|/W|xxx|
 *    Data-32bit: |DATA-32BIT|
 */
uint16_t spi_read16(uint16_t address);
uint32_t spi_read32(uint16_t address);

/* function ade9153 */
float calculate_target_aicc(float rShunt, float PGAGain);
float calculate_target_avcc(float rBig, float rSmall, float *vHeadRoom);
float calculate_target_powCC(float targetAICC, float targetAVCC);

void ADE9153_reset(uint8_t pinReset);

void ADE9153_initCFG();
void ADE9153_lock();
void ADE9153_unLock();


bool ADE9153_acal_AINormal();
bool ADE9153_acal_AITurbo();
bool ADE9153_acal_AV();
void ADE9153_acal_stop();
void ADE9153_AIGainCFG(float targetAICC, float mSureAICC);
void ADE9153_AVGainCFG(float targetAVCC, float mSureAVCC);

void ADE9153_acal_result(ACALRegs_t *Data);
void ADE9153_read_RMSRegs(RMSRegs_t *Data);
void ADE9153_read_PowRegs(PowRegs_t *Data);
void ADE9153_read_PQRegs(PQRegs_t *Data);

#endif
