/*
 * display.h
 *
 *  Created on: 10 d’oct. 2019
 *      Author: amarti
 */


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

//*****************************************************************************
//
// A set of color definitions.  This set is the subset of the X11 colors (from
// rgb.txt) that are supported by typical web browsers.
//
//*****************************************************************************
#define GRAPHICS_COLOR_ALICE_BLUE                0x00F0F8FF
#define GRAPHICS_COLOR_ANTIQUE_WHITE             0x00FAEBD7
#define GRAPHICS_COLOR_AQUA                      0x0000FFFF
#define GRAPHICS_COLOR_AQUAMARINE                0x007FFFD4
#define GRAPHICS_COLOR_AZURE                     0x00F0FFFF
#define GRAPHICS_COLOR_BEIGE                     0x00F5F5DC
#define GRAPHICS_COLOR_BISQUE                    0x00FFE4C4
#define GRAPHICS_COLOR_BLACK                     0x00000000
#define GRAPHICS_COLOR_BLANCHED_ALMOND           0x00FFEBCD
#define GRAPHICS_COLOR_BLUE                      0x000000FF
#define GRAPHICS_COLOR_BLUE_VIOLET               0x008A2BE2
#define GRAPHICS_COLOR_BROWN                     0x00A52A2A
#define GRAPHICS_COLOR_BURLY_WOOD                0x00DEB887
#define GRAPHICS_COLOR_CADET_BLUE                0x005F9EA0
#define GRAPHICS_COLOR_CHARTREUSE                0x007FFF00
#define GRAPHICS_COLOR_CHOCOLATE                 0x00D2691E
#define GRAPHICS_COLOR_CORAL                     0x00FF7F50
#define GRAPHICS_COLOR_CORNFLOWER_BLUE           0x006495ED
#define GRAPHICS_COLOR_CORNSILK                  0x00FFF8DC
#define GRAPHICS_COLOR_CRIMSON                   0x00DC143C
#define GRAPHICS_COLOR_CYAN                      0x0000FFFF
#define GRAPHICS_COLOR_DARK_BLUE                 0x0000008B
#define GRAPHICS_COLOR_DARK_CYAN                 0x00008B8B
#define GRAPHICS_COLOR_DARK_GOLDENROD            0x00B8860B
#define GRAPHICS_COLOR_DARK_GRAY                 0x00A9A9A9
#define GRAPHICS_COLOR_DARK_GREEN                0x00006400
#define GRAPHICS_COLOR_DARK_KHAKI                0x00BDB76B
#define GRAPHICS_COLOR_DARK_MAGENTA              0x008B008B
#define GRAPHICS_COLOR_DARK_OLIVE_GREEN          0x00556B2F
#define GRAPHICS_COLOR_DARK_ORANGE               0x00FF8C00
#define GRAPHICS_COLOR_DARK_ORCHID               0x009932CC
#define GRAPHICS_COLOR_DARK_RED                  0x008B0000
#define GRAPHICS_COLOR_DARK_SALMON               0x00E9967A
#define GRAPHICS_COLOR_DARK_SEA_GREEN            0x008FBC8F
#define GRAPHICS_COLOR_DARK_SLATE_BLUE           0x00483D8B
#define GRAPHICS_COLOR_DARK_SLATE_GRAY           0x002F4F4F
#define GRAPHICS_COLOR_DARK_TURQUOISE            0x0000CED1
#define GRAPHICS_COLOR_DARK_VIOLET               0x009400D3
#define GRAPHICS_COLOR_DEEP_PINK                 0x00FF1493
#define GRAPHICS_COLOR_DEEP_SKY_BLUE             0x0000BFFF
#define GRAPHICS_COLOR_DIM_GRAY                  0x00696969
#define GRAPHICS_COLOR_DODGER_BLUE               0x001E90FF
#define GRAPHICS_COLOR_FIRE_BRICK                0x00B22222
#define GRAPHICS_COLOR_FLORAL_WHITE              0x00FFFAF0
#define GRAPHICS_COLOR_FOREST_GREEN              0x00228B22
#define GRAPHICS_COLOR_FUCHSIA                   0x00FF00FF
#define GRAPHICS_COLOR_GAINSBORO                 0x00DCDCDC
#define GRAPHICS_COLOR_GHOST_WHITE               0x00F8F8FF
#define GRAPHICS_COLOR_GOLD                      0x00FFD700
#define GRAPHICS_COLOR_GOLDENRON                 0x00DAA520
#define GRAPHICS_COLOR_GRAY                      0x00808080
#define GRAPHICS_COLOR_GREEN                     0x00008000
#define GRAPHICS_COLOR_GREEN_YELLOW              0x00ADFF2F
#define GRAPHICS_COLOR_HONEYDEW                  0x00F0FFF0
#define GRAPHICS_COLOR_HOT_PINK                  0x00FF69B4
#define GRAPHICS_COLOR_INDIAN_RED                0x00CD5C5C
#define GRAPHICS_COLOR_INDIGO                    0x004B0082
#define GRAPHICS_COLOR_IVORY                     0x00FFFFF0
#define GRAPHICS_COLOR_KHAKI                     0x00F0E68C
#define GRAPHICS_COLOR_LAVENDER                  0x00E6E6FA
#define GRAPHICS_COLOR_LAVENDER_BLUSH            0x00FFF0F5
#define GRAPHICS_COLOR_LAWN_GREEN                0x007CFC00
#define GRAPHICS_COLOR_LEMON_CHIFFON             0x00FFFACD
#define GRAPHICS_COLOR_LIGHT_BLUE                0x00ADD8E6
#define GRAPHICS_COLOR_LIGHT_CORAL               0x00F08080
#define GRAPHICS_COLOR_LIGHT_CYAN                0x00E0FFFF
#define GRAPHICS_COLOR_LIGHT_GOLDENRON_YELLOW    0x00FAFAD2
#define GRAPHICS_COLOR_LIGHT_GREEN               0x0090EE90
#define GRAPHICS_COLOR_LIGHT_GRAY                0x00D3D3D3
#define GRAPHICS_COLOR_LIGHT_PINK                0x00FFB6C1
#define GRAPHICS_COLOR_LIGHT_SALMON              0x00FFA07A
#define GRAPHICS_COLOR_LIGHT_SEA_GREEN           0x0020B2AA
#define GRAPHICS_COLOR_LIGHT_SKY_BLUE            0x0087CEFA
#define GRAPHICS_COLOR_LIGHT_SLATE_GRAY          0x00778899
#define GRAPHICS_COLOR_LIGHT_STEEL_BLUE          0x00B0C4DE
#define GRAPHICS_COLOR_LIGHT_YELLOW              0x00FFFFE0
#define GRAPHICS_COLOR_LIME                      0x0000FF00
#define GRAPHICS_COLOR_LIME_GREEN                0x0032CD32
#define GRAPHICS_COLOR_LINEN                     0x00FAF0E6
#define GRAPHICS_COLOR_MAGENTA                   0x00FF00FF
#define GRAPHICS_COLOR_MAROON                    0x00800000
#define GRAPHICS_COLOR_MEDIUM_AQUAMARINE         0x0066CDAA
#define GRAPHICS_COLOR_MEDIUM_BLUE               0x000000CD
#define GRAPHICS_COLOR_MEDIUM_ORCHID             0x00BA55D3
#define GRAPHICS_COLOR_MEDIUM_PURPLE             0x009370DB
#define GRAPHICS_COLOR_MEDIUM_SEA_GREEN          0x003CB371
#define GRAPHICS_COLOR_MEDIUM_SLATE_BLUE         0x007B68EE
#define GRAPHICS_COLOR_MEDIUM_SPRING_GREEN       0x0000FA9A
#define GRAPHICS_COLOR_MEDIUM_TURQUOISE          0x0048D1CC
#define GRAPHICS_COLOR_MEDIUM_VIOLET_RED         0x00C71585
#define GRAPHICS_COLOR_MIGNIGHT_BLUE             0x00191970
#define GRAPHICS_COLOR_MINT_CREAM                0x00F5FFFA
#define GRAPHICS_COLOR_MISTY_ROSE                0x00FFE4E1
#define GRAPHICS_COLOR_MOCCASIN                  0x00FFE4B5
#define GRAPHICS_COLOR_NAVAJO_WHITE              0x00FFDEAD
#define GRAPHICS_COLOR_NAVY                      0x00000080
#define GRAPHICS_COLOR_OLD_LACE                  0x00FDF5E6
#define GRAPHICS_COLOR_OLIVE                     0x00808000
#define GRAPHICS_COLOR_OLIVE_DRAB                0x006B8E23
#define GRAPHICS_COLOR_ORANGE                    0x00FFA500
#define GRAPHICS_COLOR_ORANGE_RED                0x00FF4500
#define GRAPHICS_COLOR_ORCHID                    0x00DA70D6
#define GRAPHICS_COLOR_PALE_GOLDENRON            0x00EEE8AA
#define GRAPHICS_COLOR_PALE_GREEN                0x0098FB98
#define GRAPHICS_COLOR_PALE_TURQUOISE            0x00AFEEEE
#define GRAPHICS_COLOR_PALE_VIOLET_RED           0x00DB7093
#define GRAPHICS_COLOR_PAPAYA_WHIP               0x00FFEFD5
#define GRAPHICS_COLOR_PEACH_PUFF                0x00FFDAB9
#define GRAPHICS_COLOR_PERU                      0x00CD853F
#define GRAPHICS_COLOR_PINK                      0x00FFC0CB
#define GRAPHICS_COLOR_PLUM                      0x00DDA0DD
#define GRAPHICS_COLOR_POWDER_BLUE               0x00B0E0E6
#define GRAPHICS_COLOR_PURPLE                    0x00800080
#define GRAPHICS_COLOR_RED                       0x00FF0000
#define GRAPHICS_COLOR_ROSY_BROWN                0x00BC8F8F
#define GRAPHICS_COLOR_ROYAL_BLUE                0x004169E1
#define GRAPHICS_COLOR_SADDLE_BROWN              0x008B4513
#define GRAPHICS_COLOR_SALMON                    0x00FA8072
#define GRAPHICS_COLOR_SANDY_BROWN               0x00F4A460
#define GRAPHICS_COLOR_SEA_GREEN                 0x002E8B57
#define GRAPHICS_COLOR_SEASHELL                  0x00FFF5EE
#define GRAPHICS_COLOR_SIENNA                    0x00A0522D
#define GRAPHICS_COLOR_SILVER                    0x00C0C0C0
#define GRAPHICS_COLOR_SKY_BLUE                  0x0087CEEB
#define GRAPHICS_COLOR_SLATE_BLUE                0x006A5ACD
#define GRAPHICS_COLOR_SLATE_GRAY                0x00708090
#define GRAPHICS_COLOR_SNOW                      0x00FFFAFA
#define GRAPHICS_COLOR_SPRING_GREEN              0x0000FF7F
#define GRAPHICS_COLOR_STEEL_BLUE                0x004682B4
#define GRAPHICS_COLOR_TAN                       0x00D2B48C
#define GRAPHICS_COLOR_TEAL                      0x00008080
#define GRAPHICS_COLOR_THISTLE                   0x00D8BFD8
#define GRAPHICS_COLOR_TOMATO                    0x00FF6347
#define GRAPHICS_COLOR_TURQUOISE                 0x0040E0D0
#define GRAPHICS_COLOR_VIOLET                    0x00EE82EE
#define GRAPHICS_COLOR_WHEAT                     0x00F5DEB3
#define GRAPHICS_COLOR_WHITE                     0x00FFFFFF
#define GRAPHICS_COLOR_WHITE_SMOKE               0x00F5F5F5
#define GRAPHICS_COLOR_YELLOW                    0x00FFFF00
#define GRAPHICS_COLOR_YELLOW_GREEN              0x009ACD32
#define GRAPHICS_COLOR_BLACK                     0x00000000

/********************************************/
/****  Init all the peripherals needed   ****/
/****  to communicate with the LCD       ****/
/****  and the LCD itself                ****/
/********************************************/
void Init_display ();


/********************************************/
/****  Filled_circle with color          ****/
/****  x horizontal. Range: [0, 127]     ****/
/****  y vertical. Range: [0, 127]       ****/
/****  radius.  Range: [0, 127]          ****/
/****  color: see previous defines       ****/
/********************************************/
void Filled_circle (uint8_t x, uint8_t y, uint8_t radius, int32_t color);


/********************************************/
/****  Filled_rectangle with color       ****/
/****  x horizontal. Range: [0, 127]     ****/
/****  y vertical. Range: [0, 127]       ****/
/****  color: see previous defines       ****/
/********************************************/
void Filled_rectangle (uint8_t xmin, uint8_t xmax, uint8_t ymin, uint8_t ymax, int32_t color);

/********************************************/
/****  Shows a null-terminated string    ****/
/****  x horizontal range: [0, 127]      ****/
/****  y vertical range: [0, 127]        ****/
/****  color: see previous defines       ****/
/********************************************/
void write_string(uint8_t *msg, int32_t  x, int32_t  y, uint32_t color);



#endif /* DISPLAY_H_ */
