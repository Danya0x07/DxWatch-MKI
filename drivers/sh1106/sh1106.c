#include "sh1106.h"
#include "sh1106_port.h"

#define CMD_SET_COL_ADDR_L  0x00  // 0x0..0xF
#define CMD_SET_COL_ADDR_H  0x10  // 0x0..0xF
#define CMD_SET_PAGE_ADDR   0xB0  // 0..7

#define CMD_SET_START_LINE  0x40  // 0..63
#define CMD_DISPLAY_OFFSET_CTRL 0xD3  // ,0..63
#   define DISPLAY_OFFSET_MASK  0x3F

#define CMD_SET_REMAP   0xA0  // 0|1
#define CMD_MUX_RATIO_CTRL  0xA8  // ,0..63
#   define MUX_RATIO_MASK   0x3F
#define CMD_SET_SCAN_DIR    0xC0  // 0x0|0x8
#   define SCAN_DIR_F   0x0
#   define SCAN_DIR_B   0x8
#define CMD_COM_PADS_CONF_CTRL  0xDA  // ,0x02|0x12
#   define COM_PADS_SEQUENTIAL  0x02
#   define COM_PADS_ALTERNATIVE 0x12
#define CMD_VCOM_LEVEL_CTRL 0xDB  // ,0x00..0xFF
// VCOM = β * VREF
// β: 0x00..0x3F --> 0.43..0.834; 0x40..0xFF --> 1

#define CMD_CLKDIV_OSCFREQ_CTRL 0xD5  // ,0x00..0xFF
// [7:4] - Oscillator frequency: 0..15 --> -25%..+50% (5% step)
// [3:0] - Display clock divide ratio: 0..15 --> 1..16
#define CMD_CHARGE_TIMINGS_CTRL 0xD9  // ,0x11..0xFF
#   define CHARGE_TIMING_MIN    1
// [7:4] - Discharge period: 1..15 (in DCLK pulses)
// [3:0] - Precharge period: 1..15 (in DCLK pulses)

#define CMD_DISPLAY_ON  0xAE  // 0|1
#define CMD_FULL_ON 0xA4  // 0|1
#define CMD_INVERT  0xA6  // 0|1
#define CMD_CONTRAST_CTRL   0x81  // ,0x00..0xFF

#define CMD_DCDC_CTRL   0xAD  // ,0x8A..0x8B
#   define DCDC_EXT_VPP 0x8A
#   define DCDC_BUILTIN 0x8B

#define CMD_SET_PUMP_VOLTAGE    0x30 // 0..3
#   define PUMP_VOLTAGE_6V4 0
#   define PUMP_VOLTAGE_7V4 1
#   define PUMP_VOLTAGE_8V  2
#   define PUMP_VOLTAGE_9V  3

#define CMD_NOP 0xE3

/* Not available in SPI mode: 
#define CMD_RMW  0xE0
#define CMD_CANCEL_RMW   0xEE
*/

static const uint8_t INIT_SEQUENCE[] = {
    CMD_DISPLAY_ON | 0,
    CMD_INVERT | 0,
    CMD_FULL_ON | 0,
    CMD_CLKDIV_OSCFREQ_CTRL, 0x80,
    CMD_MUX_RATIO_CTRL, 63,
    CMD_DISPLAY_OFFSET_CTRL, 0,
    CMD_SET_START_LINE | 0,
    CMD_SET_PUMP_VOLTAGE | PUMP_VOLTAGE_8V,
    CMD_SET_REMAP | 0,
    CMD_SET_SCAN_DIR | SCAN_DIR_F,
    CMD_COM_PADS_CONF_CTRL, COM_PADS_ALTERNATIVE,
    CMD_VCOM_LEVEL_CTRL, 0x40,
    CMD_CHARGE_TIMINGS_CTRL, 0xF1,
    CMD_DCDC_CTRL, DCDC_BUILTIN,
    CMD_CONTRAST_CTRL, 0x7F,
    CMD_DISPLAY_ON | 1
};

static void SendCommand(uint8_t cmd)
{
    DC_LOW();
    CS_LOW();
    SPI_TransferByte(cmd);
    CS_HIGH();
}

void SH1106_Init(void)
{
    for (uint_fast8_t i = 0; i < sizeof(INIT_SEQUENCE); i++) {
        SendCommand(INIT_SEQUENCE[i]);
    }
}

void SH1106_SetCursor(uint8_t row, uint8_t col)
{
    /* I have 128x64 screen with SEG2 mapped to col0,
     * SEG3 to col1, ... SEG129 to col127.
     * Writes to other segments have no effect.
     */
    col += 2;
    SendCommand(CMD_SET_PAGE_ADDR | (row & 7));
    SendCommand(CMD_SET_COL_ADDR_H | (col >> 4));
    SendCommand(CMD_SET_COL_ADDR_L | (col & 0x0F));
}

void SH1106_WriteByte(uint8_t data)
{
    DC_HIGH();
    CS_LOW();
    SPI_TransferByte(data);
    CS_HIGH();
}

void SH1106_WriteBytes(const uint8_t *data, uint16_t len)
{
    DC_HIGH();
    CS_LOW();
    SPI_TransferBytes(NULL, data, len);
    CS_HIGH();
}

void SH1106_SetFullOn(bool on)
{
    SendCommand(CMD_FULL_ON | on);
}

void SH1106_SetInverse(bool inverse)
{
    SendCommand(CMD_INVERT | inverse);
}

void SH1106_SetEnable(bool enable)
{
    SendCommand(CMD_DISPLAY_ON | enable);
}

void SH1106_SetContrast(uint8_t contrast)
{
    SendCommand(CMD_CONTRAST_CTRL);
    SendCommand(contrast);
}

void SH1106_SetOffset(uint8_t offset)
{
    SendCommand(CMD_DISPLAY_OFFSET_CTRL);
    SendCommand(offset & DISPLAY_OFFSET_MASK);
}