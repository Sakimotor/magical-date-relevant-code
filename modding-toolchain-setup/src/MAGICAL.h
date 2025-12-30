#define TEXT_H 8

#define TEXT_W 8

typedef unsigned char   undefined;

typedef unsigned int* pointer;

typedef unsigned char    bool;
typedef unsigned char    byte;
typedef unsigned int    dword;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long long    uint5;
typedef unsigned long    ulong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef short    wchar_t;
typedef unsigned short    word;
typedef struct CLUT_and_PXL CLUT_and_PXL, *PCLUT_and_PXL;

typedef struct CLUT_info CLUT_info, *PCLUT_info;

typedef struct PXL_stuff PXL_stuff, *PPXL_stuff;

typedef struct CLUT_bnum CLUT_bnum, *PCLUT_bnum;

typedef struct CLUT_xy CLUT_xy, *PCLUT_xy;

typedef struct CLUT_WH CLUT_WH, *PCLUT_WH;

typedef struct CLUT_entry CLUT_entry, *PCLUT_entry;

typedef uint __u_int;

typedef __u_int u_int;

typedef ushort u_short;

typedef struct kanji_pattern kanji_pattern, *Pkanji_pattern;

struct CLUT_WH {
    ushort W;
    ushort H;
};

struct CLUT_bnum {
    uint field0_0x0;
};

struct CLUT_xy {
    ushort X;
    ushort Y;
};

struct CLUT_entry {
    int R:5;
    int G:5;
    int B:5;
    int STP:1;
};

struct CLUT_info {
    struct CLUT_bnum CLUT_bnum;
    struct CLUT_xy CLUT_XY;
    struct CLUT_WH CLUT_WH;
    struct CLUT_entry CLUT_entry1;
    struct CLUT_entry CLUT_entry2;
    struct CLUT_entry CLUT_entry3;
    struct CLUT_entry CLUT_entry4;
    struct CLUT_entry CLUT_entry5;
    struct CLUT_entry CLUT_entry6;
    struct CLUT_entry CLUT_entry7;
    struct CLUT_entry CLUT_entry8;
    struct CLUT_entry CLUT_entry9;
    struct CLUT_entry CLUT_entry10;
    struct CLUT_entry CLUT_entry11;
    struct CLUT_entry CLUT_entry12;
    struct CLUT_entry CLUT_entry13;
    struct CLUT_entry CLUT_entry14;
    struct CLUT_entry CLUT_entry15;
    struct CLUT_entry CLUT_entry16;
};

struct kanji_pattern {
    undefined2 row0;
    undefined2 row1;
    undefined2 row2;
    undefined2 row3;
    undefined2 row4;
    undefined2 row5;
    undefined2 row6;
    undefined2 row7;
    undefined2 row8;
    undefined2 row9;
    undefined2 row10;
    undefined2 row11;
    undefined2 row12;
    undefined2 row13;
    undefined2 row14;
};

struct PXL_stuff {
    u_int PXL_bnum;
    u_short PXL_DX;
    u_short PXL_DY;
    u_short PXL_W;
    u_short PXL_H;
    struct kanji_pattern PXL_entries;
};

struct CLUT_and_PXL {
    struct CLUT_info field0_0x0;
    struct PXL_stuff field1_0x2c;
};

typedef enum color_particular_cases {
    CLUT_TRANSPARENT=0,
    CLUT_WHITE=4294967295
} color_particular_cases;

typedef struct FLAG FLAG, *PFLAG;

struct FLAG {
    uint PMODE:3;
    uint CF:1;
    uint zeroed_bits:28; /* 0 */
};

typedef struct offset_then_TIM offset_then_TIM, *Poffset_then_TIM;

typedef struct TIM_data TIM_data, *PTIM_data;

struct TIM_data {
    int ID; /* 10 00 00 00 */
    struct FLAG FLAG;
    struct CLUT_info CLUT;
    struct PXL_stuff PXL;
};

struct offset_then_TIM {
    undefined4 field0_0x0;
    struct TIM_data field1_0x4;
};

typedef struct playername playername, *Pplayername;

struct playername {
    byte syl1;
    byte syl2;
    byte syl3;
    byte syl4;
};

typedef struct pointers_to_something pointers_to_something, *Ppointers_to_something;

struct pointers_to_something {
    undefined4 *image_location;
    undefined4 *pointer2;
    struct playername *playername_pointer;
    undefined4 *honorifics_pointer;
    undefined4 *offsets_pointer;
    undefined4 pxl_x;
    undefined4 pxl_y;
    undefined4 clut_x;
    undefined4 clut_y;
    undefined4 *pointer6;
    undefined4 *pointer7;
    undefined4 *pointer8;
    undefined4 *pointer9;
};

typedef struct PXL_entry_16bit PXL_entry_16bit, *PPXL_entry_16bit;

struct PXL_entry_16bit {
    struct CLUT_entry CLUT_entry;
};

typedef struct PXL_entry_24bit PXL_entry_24bit, *PPXL_entry_24bit;

struct PXL_entry_24bit {
    byte G0;
    byte R0;
    byte R1;
    byte B0;
    byte B1;
    byte G1;
};

typedef struct PXL_entry_4bit PXL_entry_4bit, *PPXL_entry_4bit;

struct PXL_entry_4bit {
    int color_of_pixel1:4;
    int color_of_pixel2:4;
    int color_of_pixel3:4;
    int color_of_pixel4:4;
};

typedef struct PXL_entry_8bit PXL_entry_8bit, *PPXL_entry_8bit;

struct PXL_entry_8bit {
    byte CLUT_color1;
    byte CLUT_color2;
};

typedef struct PXL_info PXL_info, *PPXL_info;

struct PXL_info {
    int PXL_bnum;
    short PXL_DX;
    short PXL_DY;
    short PXL_H;
    short PXL_W;
};

typedef struct PXL_WH PXL_WH, *PPXL_WH;

struct PXL_WH {
    ushort PXL_W;
    ushort PXL_H;
};

typedef struct PXL_XY PXL_XY, *PPXL_XY;

struct PXL_XY {
    ushort PXL_X;
    ushort PXL_Y;
};

typedef enum SJIS_STUFF {
    MIN_BYTE1_VAL=129,
    NEWLINE_BYTE1=129,
    NEWLINE_BYTE2=148,
    MAX_BYTE2_VAL=154,
    DOWN_ARROW=33246
} SJIS_STUFF;

typedef struct stack_and_more stack_and_more, *Pstack_and_more;

struct stack_and_more {
    undefined4 field0_0x0;
    byte field1_0x4[52];
    undefined4 field2_0x38;
    undefined4 field3_0x3c;
    pointer __stack_end;
    undefined4 field5_0x44;
    undefined4 field6_0x48;
    undefined4 field7_0x4c;
    undefined4 field8_0x50;
    undefined4 field9_0x54;
    byte field10_0x58[8];
    undefined4 *addr1_function;
    undefined4 *addr2_function;
    pointer some_bios_addr;
    pointer __stackaddr;
    pointer end_of_executable_addr;
    pointer addr_80021704;
};

typedef struct string_and_padding string_and_padding, *Pstring_and_padding;

struct string_and_padding {
    uchar *field0_0x0;
    byte field1_0x4[12];
};

typedef struct TIM_data_without_ID TIM_data_without_ID, *PTIM_data_without_ID;

struct TIM_data_without_ID {
    struct FLAG FLAG;
    struct CLUT_info CLUT;
    struct PXL_info PXL;
};

typedef enum TIM_flags {
    NO_CLUT_4BIT=0,
    NO_CLUT_8BIT=1,
    CLUT_4BIT=8,
    CLUT_8BIT=9
} TIM_flags;

typedef struct TIM_header TIM_header, *PTIM_header;

struct TIM_header {
    int ID; /* 10 00 00 00 */
    struct FLAG FLAG;
    struct CLUT_info CLUT;
};

typedef struct TIM_headers_and_locations TIM_headers_and_locations, *PTIM_headers_and_locations;

struct TIM_headers_and_locations {
    struct FLAG FLAG;
    struct PXL_XY PXL_XY;
    struct PXL_WH PXL_WH;
    struct PXL_entry_4bit *PXL_address;
    struct CLUT_xy CLUT_XY;
    struct CLUT_WH CLUT_WH;
    struct CLUT_entry *CLUT_address;
};


typedef struct width_array width_array, *Pwidth_array;

struct width_array {
    ushort char_count;
};

typedef struct XYWH XYWH, *PXYWH;

struct XYWH {
    ushort X;
    ushort Y;
    ushort W;
    ushort H;
};

