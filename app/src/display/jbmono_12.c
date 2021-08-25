/*******************************************************************************
 * Size: 12 px
 * Bpp: 1
 * Opts: --no-compress --no-prefilter --bpp 1 --size 12 --font /home/steven/.local/share/fonts/JetBrainsMono-Regular.ttf -r 0x20-0x7F,0xB0,0x2022 --font FontAwesome5-Solid+Brands+Regular.woff -r 61441,61448,61451,61452,61452,61453,61457,61459,61461,61465,61468,61473,61478,61479,61480,61502,61512,61515,61516,61517,61521,61522,61523,61524,61543,61544,61550,61552,61553,61556,61559,61560,61561,61563,61587,61589,61636,61637,61639,61671,61674,61683,61724,61732,61787,61931,62016,62017,62018,62019,62020,62087,62099,62212,62189,62810,63426,63650 --format lvgl -o /home/steven/gitproj/zmk/app/src/display/jbmono_12.c --force-fast-kern-format
 ******************************************************************************/

#include "lvgl.h"

#ifndef JBMONO_12
#define JBMONO_12 1
#endif

#if JBMONO_12

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xaa, 0xa0, 0xc0,

    /* U+0022 "\"" */
    0x99, 0x99,

    /* U+0023 "#" */
    0x24, 0x49, 0xf9, 0x22, 0x85, 0x3f, 0x24, 0x48,

    /* U+0024 "$" */
    0x21, 0x1d, 0x5a, 0xd1, 0xc7, 0x2d, 0x7e, 0x42,
    0x0,

    /* U+0025 "%" */
    0xe1, 0x42, 0x8f, 0x61, 0xf, 0xe4, 0x89, 0x1e,

    /* U+0026 "&" */
    0x31, 0x24, 0x10, 0x62, 0x59, 0xa6, 0x74,

    /* U+0027 "'" */
    0xf0,

    /* U+0028 "(" */
    0x13, 0x48, 0x88, 0x88, 0x88, 0x43, 0x10,

    /* U+0029 ")" */
    0x8c, 0x21, 0x11, 0x11, 0x11, 0x2c, 0x80,

    /* U+002A "*" */
    0x10, 0x21, 0xf1, 0x82, 0x88, 0x80,

    /* U+002B "+" */
    0x20, 0x82, 0x3f, 0x20, 0x80,

    /* U+002C "," */
    0x4b, 0x40,

    /* U+002D "-" */
    0xf8,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x8, 0x44, 0x21, 0x10, 0x8c, 0x42, 0x31, 0x0,

    /* U+0030 "0" */
    0x74, 0x63, 0x5a, 0xd6, 0x31, 0x70,

    /* U+0031 "1" */
    0x65, 0x8, 0x42, 0x10, 0x84, 0xf8,

    /* U+0032 "2" */
    0x74, 0x42, 0x11, 0x19, 0x98, 0xf8,

    /* U+0033 "3" */
    0xf8, 0xc8, 0xe1, 0x86, 0x33, 0x70,

    /* U+0034 "4" */
    0x10, 0x88, 0x84, 0xc7, 0xe1, 0x8,

    /* U+0035 "5" */
    0xfc, 0x21, 0xe8, 0x84, 0x31, 0x70,

    /* U+0036 "6" */
    0x21, 0x10, 0xed, 0xc6, 0x31, 0x70,

    /* U+0037 "7" */
    0xfe, 0x20, 0x86, 0x10, 0xc2, 0x8, 0x60,

    /* U+0038 "8" */
    0x74, 0x63, 0x17, 0x6e, 0x31, 0x70,

    /* U+0039 "9" */
    0x74, 0x63, 0x17, 0x88, 0xc4, 0x60,

    /* U+003A ":" */
    0xf0, 0x3c,

    /* U+003B ";" */
    0x6c, 0x0, 0xd2, 0xc0,

    /* U+003C "<" */
    0x1b, 0x31, 0x83, 0x4,

    /* U+003D "=" */
    0xf8, 0x1, 0xf0,

    /* U+003E ">" */
    0xc1, 0x86, 0x36, 0x40,

    /* U+003F "?" */
    0xf0, 0x42, 0x37, 0x21, 0x0, 0x60,

    /* U+0040 "@" */
    0x7b, 0x18, 0x6f, 0xa6, 0x9a, 0x6f, 0x83, 0x7,
    0x0,

    /* U+0041 "A" */
    0x31, 0x45, 0x14, 0x4b, 0xe8, 0xa2, 0x8c,

    /* U+0042 "B" */
    0xf4, 0x63, 0x1f, 0x46, 0x31, 0xf0,

    /* U+0043 "C" */
    0x74, 0x61, 0x8, 0x42, 0x11, 0x70,

    /* U+0044 "D" */
    0xf4, 0x63, 0x18, 0xc6, 0x31, 0xf0,

    /* U+0045 "E" */
    0xfc, 0x21, 0xf, 0xc2, 0x10, 0xf8,

    /* U+0046 "F" */
    0xfc, 0x21, 0xf, 0xc2, 0x10, 0x80,

    /* U+0047 "G" */
    0x74, 0x61, 0xb, 0xc6, 0x31, 0x70,

    /* U+0048 "H" */
    0x8c, 0x63, 0x1f, 0xc6, 0x31, 0x88,

    /* U+0049 "I" */
    0xf9, 0x8, 0x42, 0x10, 0x84, 0xf8,

    /* U+004A "J" */
    0x78, 0x42, 0x10, 0x84, 0x31, 0x70,

    /* U+004B "K" */
    0x8a, 0x29, 0x24, 0xe2, 0x49, 0x22, 0x88,

    /* U+004C "L" */
    0x84, 0x21, 0x8, 0x42, 0x10, 0xf8,

    /* U+004D "M" */
    0x8e, 0xf7, 0xda, 0xc6, 0x31, 0x88,

    /* U+004E "N" */
    0xce, 0x73, 0x5a, 0xd6, 0x73, 0x98,

    /* U+004F "O" */
    0x74, 0x63, 0x18, 0xc6, 0x31, 0x70,

    /* U+0050 "P" */
    0xf4, 0x63, 0x1f, 0x42, 0x10, 0x80,

    /* U+0051 "Q" */
    0x74, 0x63, 0x18, 0xc6, 0x31, 0x70, 0x82,

    /* U+0052 "R" */
    0xf4, 0x63, 0x1f, 0x4a, 0x53, 0x88,

    /* U+0053 "S" */
    0x74, 0x61, 0x87, 0xc, 0x31, 0x70,

    /* U+0054 "T" */
    0xfe, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x10,

    /* U+0055 "U" */
    0x8c, 0x63, 0x18, 0xc6, 0x31, 0x70,

    /* U+0056 "V" */
    0x8e, 0x28, 0xb2, 0x59, 0x45, 0x14, 0x20,

    /* U+0057 "W" */
    0x9b, 0xb5, 0xab, 0x56, 0xad, 0x5a, 0xb6, 0x64,

    /* U+0058 "X" */
    0x44, 0xc8, 0xa0, 0xc1, 0x7, 0xa, 0x22, 0x44,

    /* U+0059 "Y" */
    0x44, 0x89, 0xb1, 0x42, 0x82, 0x4, 0x8, 0x10,

    /* U+005A "Z" */
    0xf8, 0x44, 0x22, 0x21, 0x10, 0xf8,

    /* U+005B "[" */
    0xf2, 0x49, 0x24, 0x92, 0x70,

    /* U+005C "\\" */
    0x86, 0x10, 0x86, 0x10, 0x82, 0x10, 0x82, 0x10,

    /* U+005D "]" */
    0xe4, 0x92, 0x49, 0x24, 0xf0,

    /* U+005E "^" */
    0x22, 0x94, 0x98, 0x80,

    /* U+005F "_" */
    0xf8,

    /* U+0060 "`" */
    0xc8,

    /* U+0061 "a" */
    0x74, 0x42, 0xf8, 0xc5, 0xe0,

    /* U+0062 "b" */
    0x84, 0x2d, 0x98, 0xc6, 0x31, 0xf0,

    /* U+0063 "c" */
    0x74, 0x61, 0x8, 0x45, 0xc0,

    /* U+0064 "d" */
    0x8, 0x5b, 0x38, 0xc6, 0x31, 0x78,

    /* U+0065 "e" */
    0x74, 0x63, 0xf8, 0x45, 0xc0,

    /* U+0066 "f" */
    0x1c, 0x82, 0x3f, 0x20, 0x82, 0x8, 0x20,

    /* U+0067 "g" */
    0x6c, 0xe3, 0x18, 0xbc, 0x21, 0x70,

    /* U+0068 "h" */
    0x84, 0x2d, 0x98, 0xc6, 0x31, 0x88,

    /* U+0069 "i" */
    0x21, 0x1, 0xc2, 0x10, 0x84, 0x27, 0xc0,

    /* U+006A "j" */
    0x0, 0xc0, 0xf0, 0x84, 0x21, 0x8, 0x47, 0xe0,

    /* U+006B "k" */
    0x82, 0x8, 0xa6, 0x93, 0x89, 0x26, 0x88,

    /* U+006C "l" */
    0xe0, 0x82, 0x8, 0x20, 0x82, 0x8, 0x1c,

    /* U+006D "m" */
    0xfe, 0x9a, 0x69, 0xa6, 0x9a, 0x40,

    /* U+006E "n" */
    0xb6, 0x63, 0x18, 0xc6, 0x20,

    /* U+006F "o" */
    0x74, 0x63, 0x18, 0xc5, 0xc0,

    /* U+0070 "p" */
    0xb6, 0x63, 0x18, 0xc7, 0xd0, 0x80,

    /* U+0071 "q" */
    0x6c, 0xe3, 0x18, 0xc5, 0xe1, 0x8,

    /* U+0072 "r" */
    0xb6, 0x63, 0x8, 0x42, 0x0,

    /* U+0073 "s" */
    0x74, 0x60, 0xe0, 0xc5, 0xc0,

    /* U+0074 "t" */
    0x20, 0x8f, 0xc8, 0x20, 0x82, 0x8, 0x1c,

    /* U+0075 "u" */
    0x8c, 0x63, 0x18, 0xc5, 0xc0,

    /* U+0076 "v" */
    0x8a, 0x2c, 0x94, 0x51, 0xc2, 0x0,

    /* U+0077 "w" */
    0x96, 0xdb, 0x6d, 0xb5, 0x54, 0xc0,

    /* U+0078 "x" */
    0x89, 0x67, 0x8, 0x51, 0x68, 0x80,

    /* U+0079 "y" */
    0x8a, 0x24, 0x94, 0x50, 0xc2, 0x8, 0x60,

    /* U+007A "z" */
    0xf8, 0x44, 0x44, 0x43, 0xe0,

    /* U+007B "{" */
    0xc, 0x41, 0x4, 0x10, 0x4e, 0x4, 0x10, 0x41,
    0x3,

    /* U+007C "|" */
    0xff, 0xf0,

    /* U+007D "}" */
    0xc0, 0x82, 0x8, 0x20, 0x81, 0xc8, 0x20, 0x82,
    0x30,

    /* U+007E "~" */
    0xe6, 0xd9, 0x80,

    /* U+00B0 "°" */
    0x69, 0x96,

    /* U+2022 "•" */
    0xff, 0x80,

    /* U+F001 "" */
    0x0, 0x70, 0x3f, 0x1f, 0xf1, 0xfb, 0x1c, 0x31,
    0x83, 0x18, 0x31, 0x83, 0x19, 0xf7, 0x9f, 0xf8,
    0x47, 0x0,

    /* U+F008 "" */
    0xbf, 0xde, 0x7, 0xa0, 0x5e, 0x7, 0xbf, 0xde,
    0x7, 0xa0, 0x5e, 0x7, 0xbf, 0xd0,

    /* U+F00B "" */
    0xf7, 0xf7, 0xbf, 0xfd, 0xfe, 0x0, 0xf, 0x7f,
    0x7b, 0xff, 0xdf, 0xc0, 0x0, 0xf7, 0xf7, 0xbf,
    0xfd, 0xfc,

    /* U+F00C "" */
    0x0, 0x20, 0x7, 0x0, 0xe4, 0x1c, 0xe3, 0x87,
    0x70, 0x3e, 0x1, 0xc0, 0x8, 0x0,

    /* U+F00D "" */
    0xc3, 0xe7, 0x7e, 0x3c, 0x3c, 0x7e, 0xe7, 0xc3,

    /* U+F011 "" */
    0x6, 0x2, 0x64, 0x76, 0xe6, 0x66, 0xc6, 0x3c,
    0x63, 0xc6, 0x3c, 0x3, 0x60, 0x67, 0xe, 0x3f,
    0xc0, 0xf0,

    /* U+F013 "" */
    0xe, 0x4, 0xf0, 0x7f, 0xef, 0xfe, 0x71, 0xe7,
    0xc, 0x71, 0xef, 0xfe, 0x7f, 0xe4, 0xf0, 0xe,
    0x0,

    /* U+F015 "" */
    0x3, 0x30, 0x1e, 0xc1, 0xcf, 0xc, 0xcc, 0x6f,
    0xdb, 0x7f, 0xb3, 0xff, 0xf, 0x3c, 0x3c, 0xf0,
    0xf3, 0xc0,

    /* U+F019 "" */
    0xe, 0x0, 0xe0, 0xe, 0x0, 0xe0, 0x3f, 0xc3,
    0xf8, 0x1f, 0x0, 0xe0, 0xf5, 0xff, 0xff, 0xff,
    0x5f, 0xff,

    /* U+F01C "" */
    0x1f, 0xe0, 0xc0, 0xc6, 0x1, 0x90, 0x2, 0xf8,
    0x7f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,

    /* U+F021 "" */
    0x0, 0x31, 0xf3, 0x71, 0xfc, 0x7, 0xc3, 0xf0,
    0x0, 0x0, 0x0, 0x0, 0xfc, 0x3e, 0x3, 0xf8,
    0xec, 0xf8, 0xc0, 0x0,

    /* U+F026 "" */
    0xc, 0x7f, 0xff, 0xff, 0xf1, 0xc3,

    /* U+F027 "" */
    0xc, 0xe, 0x3f, 0x7f, 0x9f, 0xdf, 0xe0, 0x70,
    0x18,

    /* U+F028 "" */
    0x0, 0x60, 0x1, 0x83, 0x34, 0x38, 0xdf, 0xda,
    0xfe, 0x57, 0xf6, 0xbf, 0x8d, 0x1c, 0xd0, 0x61,
    0x80, 0x18,

    /* U+F03E "" */
    0xff, 0xf9, 0xff, 0x9f, 0xf9, 0xef, 0xfc, 0x7d,
    0x83, 0xc0, 0x38, 0x3, 0xff, 0xf0,

    /* U+F048 "" */
    0xc3, 0xc7, 0xcf, 0xdf, 0xff, 0xff, 0xdf, 0xcf,
    0xc7, 0xc3,

    /* U+F04B "" */
    0x0, 0x1c, 0x3, 0xe0, 0x7f, 0xf, 0xf9, 0xff,
    0xbf, 0xff, 0xfe, 0xff, 0x9f, 0xc3, 0xe0, 0x70,
    0x0, 0x0,

    /* U+F04C "" */
    0xfb, 0xff, 0x7f, 0xef, 0xfd, 0xff, 0xbf, 0xf7,
    0xfe, 0xff, 0xdf, 0xfb, 0xff, 0x7c,

    /* U+F04D "" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,

    /* U+F051 "" */
    0xc3, 0xe3, 0xf3, 0xfb, 0xff, 0xff, 0xfb, 0xf3,
    0xe3, 0xc3,

    /* U+F052 "" */
    0xc, 0x3, 0xc0, 0x7c, 0x1f, 0xc7, 0xfd, 0xff,
    0xbf, 0xf0, 0x0, 0xff, 0xff, 0xff, 0xff, 0x80,

    /* U+F053 "" */
    0xc, 0x73, 0x9c, 0xe3, 0x87, 0xe, 0x1c, 0x30,

    /* U+F054 "" */
    0x83, 0x87, 0xe, 0x1c, 0x73, 0x9c, 0xe2, 0x0,

    /* U+F067 "" */
    0xe, 0x1, 0xc0, 0x38, 0x7, 0xf, 0xff, 0xff,
    0xc3, 0x80, 0x70, 0xe, 0x1, 0xc0,

    /* U+F068 "" */
    0xff, 0xff, 0xfc,

    /* U+F06E "" */
    0xf, 0x81, 0xc7, 0x1c, 0x1d, 0xc6, 0x7e, 0xfb,
    0xf7, 0xdd, 0xdd, 0xc7, 0x1c, 0xf, 0x80,

    /* U+F070 "" */
    0x0, 0x1, 0xc0, 0x1, 0xdf, 0x0, 0xe3, 0x80,
    0xdb, 0x84, 0xfb, 0x9c, 0x77, 0x3c, 0x6e, 0x38,
    0x78, 0x38, 0x70, 0x1e, 0x30, 0x0, 0x30, 0x0,
    0x0,

    /* U+F071 "" */
    0x3, 0x0, 0x1c, 0x0, 0xf8, 0x3, 0xf0, 0x1c,
    0xc0, 0x73, 0x83, 0xcf, 0x1f, 0xfc, 0x7c, 0xfb,
    0xf3, 0xef, 0xff, 0x80,

    /* U+F074 "" */
    0x0, 0x0, 0x6, 0xe1, 0xff, 0x3f, 0x17, 0x60,
    0xe4, 0x1f, 0x6f, 0xbf, 0xf1, 0xf0, 0x6, 0x0,
    0x40,

    /* U+F077 "" */
    0x0, 0x3, 0x1, 0xe0, 0xcc, 0x61, 0xb0, 0x30,
    0x0,

    /* U+F078 "" */
    0x0, 0x30, 0x36, 0x18, 0xcc, 0x1e, 0x3, 0x0,
    0x0,

    /* U+F079 "" */
    0x30, 0x0, 0xf7, 0xf3, 0xf0, 0x65, 0xa0, 0xc3,
    0x1, 0x86, 0xb, 0x4c, 0x1f, 0x9f, 0xde, 0x0,
    0x18,

    /* U+F07B "" */
    0x78, 0xf, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,

    /* U+F093 "" */
    0x6, 0x0, 0xf0, 0x1f, 0x83, 0xfc, 0x7, 0x0,
    0x70, 0x7, 0x0, 0x70, 0xf7, 0xff, 0xff, 0xff,
    0x5f, 0xff,

    /* U+F095 "" */
    0x0, 0x0, 0xf, 0x0, 0xf0, 0x1f, 0x0, 0xf0,
    0x6, 0x0, 0xe0, 0x1c, 0x73, 0xcf, 0xf8, 0xfe,
    0xf, 0xc0, 0x40, 0x0,

    /* U+F0C4 "" */
    0x70, 0x5b, 0x3f, 0x6f, 0x3f, 0xc1, 0xf0, 0x3e,
    0x1f, 0xe6, 0xde, 0xd9, 0xee, 0x8,

    /* U+F0C5 "" */
    0x1f, 0x43, 0xef, 0x7f, 0xef, 0xfd, 0xff, 0xbf,
    0xf7, 0xfe, 0xff, 0xdf, 0xf8, 0x3, 0xfc, 0x0,

    /* U+F0C7 "" */
    0xff, 0x98, 0x1b, 0x3, 0xe0, 0x7c, 0xf, 0xff,
    0xfe, 0x7f, 0x8f, 0xf9, 0xff, 0xfc,

    /* U+F0E7 "" */
    0x78, 0x78, 0xf8, 0xf0, 0xff, 0xfe, 0xfc, 0x1c,
    0x18, 0x18, 0x10, 0x30,

    /* U+F0EA "" */
    0x18, 0x3b, 0x8e, 0xe3, 0xf8, 0xe0, 0x3b, 0xae,
    0xe7, 0xbf, 0xef, 0xfb, 0xf0, 0xfc, 0x3f,

    /* U+F0F3 "" */
    0x4, 0x0, 0x80, 0x7c, 0x1f, 0xc3, 0xf8, 0x7f,
    0x1f, 0xf3, 0xfe, 0x7f, 0xdf, 0xfc, 0x0, 0x7,
    0x0,

    /* U+F11C "" */
    0xff, 0xff, 0x52, 0xbd, 0x4a, 0xff, 0xff, 0xeb,
    0x5f, 0xff, 0xfd, 0x2, 0xf4, 0xb, 0xff, 0xfc,

    /* U+F124 "" */
    0x0, 0x0, 0xf, 0x3, 0xf0, 0xfe, 0x3f, 0xef,
    0xfc, 0xff, 0xc0, 0x78, 0x7, 0x80, 0x78, 0x7,
    0x0, 0x70, 0x2, 0x0,

    /* U+F15B "" */
    0xfa, 0x7d, 0xbe, 0xff, 0xf, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf0,

    /* U+F1EB "" */
    0x7, 0xc0, 0x7f, 0xf1, 0xe0, 0xf7, 0x0, 0x70,
    0x7c, 0x3, 0xfe, 0x6, 0xc, 0x0, 0x0, 0x3,
    0x80, 0x7, 0x0, 0xe, 0x0,

    /* U+F240 "" */
    0xff, 0xff, 0x80, 0x1f, 0x7f, 0xfe, 0xff, 0xbd,
    0xff, 0xf8, 0x1, 0xff, 0xff, 0x80,

    /* U+F241 "" */
    0xff, 0xff, 0x80, 0x1f, 0x7f, 0x3e, 0xfe, 0x3d,
    0xfc, 0xf8, 0x1, 0xff, 0xff, 0x80,

    /* U+F242 "" */
    0xff, 0xff, 0x80, 0x1f, 0x78, 0x3e, 0xf0, 0x3d,
    0xe0, 0xf8, 0x1, 0xff, 0xff, 0x80,

    /* U+F243 "" */
    0xff, 0xff, 0x80, 0x1f, 0x60, 0x3e, 0xc0, 0x3d,
    0x80, 0xf8, 0x1, 0xff, 0xff, 0x80,

    /* U+F244 "" */
    0xff, 0xff, 0x80, 0x1f, 0x0, 0x3e, 0x0, 0x3c,
    0x0, 0xf8, 0x1, 0xff, 0xff, 0x80,

    /* U+F287 "" */
    0x0, 0xc0, 0x7, 0x80, 0x10, 0x7, 0x20, 0x6f,
    0xff, 0xfc, 0x41, 0x80, 0x40, 0x0, 0xb8, 0x0,
    0xf0,

    /* U+F293 "" */
    0x3e, 0x3b, 0x9c, 0xdb, 0x7c, 0xbf, 0x1f, 0x9f,
    0x87, 0xd5, 0xf9, 0x9d, 0xc7, 0xc0,

    /* U+F2ED "" */
    0xe, 0x1f, 0xfc, 0x0, 0x0, 0x7, 0xfc, 0xd5,
    0x9a, 0xb3, 0x56, 0x6a, 0xcd, 0x59, 0xab, 0x3f,
    0xe0,

    /* U+F304 "" */
    0x0, 0x40, 0xe, 0x0, 0xf0, 0x37, 0x7, 0xa0,
    0xfc, 0x1f, 0x83, 0xf0, 0x7e, 0xf, 0xc0, 0xf8,
    0xf, 0x0, 0x80, 0x0,

    /* U+F55A "" */
    0xf, 0xfe, 0x3f, 0xfc, 0xfb, 0x3b, 0xf0, 0xff,
    0xf3, 0xef, 0xc3, 0xcf, 0xb7, 0x8f, 0xff, 0xf,
    0xfe,

    /* U+F7C2 "" */
    0x1f, 0x9a, 0xbe, 0xaf, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf8,

    /* U+F8A2 "" */
    0x0, 0x0, 0x3, 0x30, 0x37, 0x3, 0xff, 0xff,
    0xff, 0x70, 0x3, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 115, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 115, .box_w = 2, .box_h = 9, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 115, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 6, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 14, .adv_w = 115, .box_w = 5, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 23, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 31, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 38, .adv_w = 115, .box_w = 1, .box_h = 4, .ofs_x = 3, .ofs_y = 5},
    {.bitmap_index = 39, .adv_w = 115, .box_w = 4, .box_h = 13, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 46, .adv_w = 115, .box_w = 4, .box_h = 13, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 53, .adv_w = 115, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 59, .adv_w = 115, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 64, .adv_w = 115, .box_w = 3, .box_h = 4, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 66, .adv_w = 115, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 67, .adv_w = 115, .box_w = 2, .box_h = 2, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 68, .adv_w = 115, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 76, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 82, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 88, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 94, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 100, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 106, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 112, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 118, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 131, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 137, .adv_w = 115, .box_w = 2, .box_h = 7, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 139, .adv_w = 115, .box_w = 3, .box_h = 9, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 143, .adv_w = 115, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 147, .adv_w = 115, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 150, .adv_w = 115, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 154, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 160, .adv_w = 115, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 169, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 176, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 188, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 194, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 200, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 206, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 212, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 218, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 224, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 230, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 237, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 243, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 249, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 255, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 261, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 267, .adv_w = 115, .box_w = 5, .box_h = 11, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 274, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 280, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 286, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 294, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 300, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 307, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 315, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 323, .adv_w = 115, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 331, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 337, .adv_w = 115, .box_w = 3, .box_h = 12, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 342, .adv_w = 115, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 350, .adv_w = 115, .box_w = 3, .box_h = 12, .ofs_x = 2, .ofs_y = -1},
    {.bitmap_index = 355, .adv_w = 115, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 359, .adv_w = 115, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 360, .adv_w = 115, .box_w = 3, .box_h = 2, .ofs_x = 2, .ofs_y = 8},
    {.bitmap_index = 361, .adv_w = 115, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 366, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 372, .adv_w = 115, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 377, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 383, .adv_w = 115, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 388, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 395, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 401, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 407, .adv_w = 115, .box_w = 5, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 414, .adv_w = 115, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 422, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 429, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 436, .adv_w = 115, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 442, .adv_w = 115, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 447, .adv_w = 115, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 452, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 458, .adv_w = 115, .box_w = 5, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 464, .adv_w = 115, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 469, .adv_w = 115, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 474, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 481, .adv_w = 115, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 486, .adv_w = 115, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 492, .adv_w = 115, .box_w = 6, .box_h = 7, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 498, .adv_w = 115, .box_w = 6, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 504, .adv_w = 115, .box_w = 6, .box_h = 9, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 511, .adv_w = 115, .box_w = 5, .box_h = 7, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 516, .adv_w = 115, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 525, .adv_w = 115, .box_w = 1, .box_h = 12, .ofs_x = 3, .ofs_y = -1},
    {.bitmap_index = 527, .adv_w = 115, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 536, .adv_w = 115, .box_w = 6, .box_h = 3, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 539, .adv_w = 115, .box_w = 4, .box_h = 4, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 541, .adv_w = 115, .box_w = 3, .box_h = 3, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 543, .adv_w = 192, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 561, .adv_w = 192, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 575, .adv_w = 192, .box_w = 13, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 593, .adv_w = 192, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 607, .adv_w = 132, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 615, .adv_w = 192, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 633, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 650, .adv_w = 216, .box_w = 14, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 668, .adv_w = 192, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 686, .adv_w = 216, .box_w = 14, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 702, .adv_w = 192, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 722, .adv_w = 96, .box_w = 6, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 728, .adv_w = 144, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 737, .adv_w = 216, .box_w = 13, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 755, .adv_w = 192, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 769, .adv_w = 168, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 779, .adv_w = 168, .box_w = 11, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 797, .adv_w = 168, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 811, .adv_w = 168, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 825, .adv_w = 168, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 835, .adv_w = 168, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 851, .adv_w = 120, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 859, .adv_w = 120, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 867, .adv_w = 168, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 881, .adv_w = 168, .box_w = 11, .box_h = 2, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 884, .adv_w = 216, .box_w = 13, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 899, .adv_w = 240, .box_w = 15, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 924, .adv_w = 216, .box_w = 14, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 944, .adv_w = 192, .box_w = 12, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 961, .adv_w = 168, .box_w = 10, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 970, .adv_w = 168, .box_w = 10, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 979, .adv_w = 240, .box_w = 15, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 996, .adv_w = 192, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1010, .adv_w = 192, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1028, .adv_w = 192, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1048, .adv_w = 168, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1062, .adv_w = 168, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1078, .adv_w = 168, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1092, .adv_w = 120, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1104, .adv_w = 168, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1119, .adv_w = 168, .box_w = 11, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1136, .adv_w = 216, .box_w = 14, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1152, .adv_w = 192, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1172, .adv_w = 144, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1186, .adv_w = 240, .box_w = 15, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1207, .adv_w = 240, .box_w = 15, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1221, .adv_w = 240, .box_w = 15, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1235, .adv_w = 240, .box_w = 15, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1249, .adv_w = 240, .box_w = 15, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1263, .adv_w = 240, .box_w = 15, .box_h = 7, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1277, .adv_w = 240, .box_w = 15, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1294, .adv_w = 168, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1308, .adv_w = 168, .box_w = 11, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1325, .adv_w = 192, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1345, .adv_w = 240, .box_w = 15, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1362, .adv_w = 144, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1376, .adv_w = 193, .box_w = 12, .box_h = 8, .ofs_x = 0, .ofs_y = 1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x1f72, 0xef51, 0xef58, 0xef5b, 0xef5c, 0xef5d, 0xef61,
    0xef63, 0xef65, 0xef69, 0xef6c, 0xef71, 0xef76, 0xef77, 0xef78,
    0xef8e, 0xef98, 0xef9b, 0xef9c, 0xef9d, 0xefa1, 0xefa2, 0xefa3,
    0xefa4, 0xefb7, 0xefb8, 0xefbe, 0xefc0, 0xefc1, 0xefc4, 0xefc7,
    0xefc8, 0xefc9, 0xefcb, 0xefe3, 0xefe5, 0xf014, 0xf015, 0xf017,
    0xf037, 0xf03a, 0xf043, 0xf06c, 0xf074, 0xf0ab, 0xf13b, 0xf190,
    0xf191, 0xf192, 0xf193, 0xf194, 0xf1d7, 0xf1e3, 0xf23d, 0xf254,
    0xf4aa, 0xf712, 0xf7f2
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 176, .range_length = 63475, .glyph_id_start = 96,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 59, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t jbmono_12 = {
#else
lv_font_t jbmono_12 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 12,          /*The maximum line height required by the font*/
    .base_line = 1,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if JBMONO_12*/

