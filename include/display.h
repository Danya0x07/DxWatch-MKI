/* Display interface adapter.
 */

#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <sh1106.h>

#define DISPLAY_NUM_ROWS    SH1106_NUM_ROWS
#define DISPLAY_NUM_COLS    SH1106_NUM_COLS
#define DISPLAY_MAX_X   SH1106_MAX_COL
#define DISPLAY_MAX_PG  SH1106_MAX_ROW

#define DISPLAY_Init()  SH1106_Init()

#define DISPLAY_SetCursor(x, page)  SH1106_SetCursor((page), (x))
#define DISPLAY_WriteByte(b)        SH1106_WriteByte((b))
#define DISPLAY_WriteBytes(b, len)  SH1106_WriteBytes((b), (len))

/* The great paradox of such displays.
 * Contrast actualy acts like brightness.
 * I don't know why.
 */
#define DISPLAY_SetBrightness(b)    SH1106_SetContrast((b))
#define DISPLAY_SetInverse(inv)     SH1106_SetInverse((inv))

#define DISPLAY_ON()    SH1106_SetEnable(true)
#define DISPLAY_OFF()   SH1106_SetEnable(false)

#endif // _DISPLAY_H