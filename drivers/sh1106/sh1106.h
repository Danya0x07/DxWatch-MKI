/** SH1106 128x64 OLED Display driver
 * Communication via 4-wire SPI.
 */

#ifndef INC_DRIVERS_SH1106
#define INC_DRIVERS_SH1106

#include <main.h>

#define SH1106_NUM_ROWS 8
#define SH1106_NUM_COLS 128
#define SH1106_MAX_ROW  (SH1106_NUM_ROWS - 1)
#define SH1106_MAX_COL  (SH1106_NUM_COLS - 1)

// Call after SPI initialization.
void SH1106_Init(void);

/* Set cursor position for the next write.
 * row: 0..7; 
 * col: 0..127 */
void SH1106_SetCursor(uint8_t row, uint8_t col);

void SH1106_WriteByte(uint8_t data);
void SH1106_WriteBytes(const uint8_t *data, uint16_t len);

void SH1106_SetFullOn(bool on);
void SH1106_SetInverse(bool inverse);
void SH1106_SetEnable(bool enable);

/* Set contrast of the pixels. (Actually that means brightness) 
 * contrast: 0..255 */
void SH1106_SetContrast(uint8_t contrast);

/* Set the vertical offset of screen content.
 * offset: 0..63 */
void SH1106_SetOffset(uint8_t offset);

#endif // INC_DRIVERS_SH1106