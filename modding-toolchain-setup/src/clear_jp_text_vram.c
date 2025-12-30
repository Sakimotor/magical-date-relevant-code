#include <common.h>

int  make_pxl_for_string_custom(void* string,int* pxl_stuff, unsigned short vram_x,unsigned short vram_y, int* charcount_per_line){
							*pxl_stuff = 0;
							*(pxl_stuff + 1) = 0;
							*(pxl_stuff + 2) = 0;
							
							//*charcount_per_line = 0x0004000e;
							return 8;
					   }