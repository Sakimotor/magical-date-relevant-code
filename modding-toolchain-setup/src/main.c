#include <common.h>
#include <types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libapi.h>
#include <libgs.h>




#include "MAGICAL.h"

#define HIRAGANA_LIMIT 90



int count_width_ascii(unsigned char* string, short* width_array){
		*width_array = 0;
		*(width_array + 1) = 0;
		
	while (*string != '\0'){
		if (*string == '#') {
			++width_array;
			
		}
		else {
			if (*string > 0x84) {
				++string;
				*width_array = *width_array + 8;
				
			}
			else if (*string < 0x30) {++string;
			}
			++(*width_array);
		}
		++string;
	}

	return 0;
	
}

typedef int (*rand_fun) ();

rand_fun random = (rand_fun) 0x80079e24;

typedef void (*load_sprite_in_vram) (void*, int);
load_sprite_in_vram load_pointer = (load_sprite_in_vram) 0x800280e8;



typedef int (*MoveImageStuff) (RECT*, int, int);
MoveImageStuff move_image = (MoveImageStuff) 0x80072514;

typedef void (*load_text_from_vram) (void*);

load_text_from_vram text_loader = (load_text_from_vram) 0x800289ec;


typedef long (*DrawSyncStuff) (long);


DrawSyncStuff draw_sync = (DrawSyncStuff) 0x80072188;


typedef void (*StoreImageStuff) (RECT*, unsigned long*);
StoreImageStuff store_image = (StoreImageStuff) 0x800724b0;


typedef void (*ClearImageStuff) (RECT*, unsigned char, unsigned char, unsigned char);
ClearImageStuff clear_image = (ClearImageStuff) 0x8007231c;

typedef void (*LoadImageStuff) (RECT*, unsigned long*);
LoadImageStuff load_image = (LoadImageStuff) 0x8007244c;

typedef int (*strlen_func) (unsigned char*);

strlen_func strlen = (strlen_func) 0x80079df4;

typedef unsigned char* (*strcpy_func) (unsigned char*, unsigned char*);

strcpy_func strcpy = (strcpy_func) 0x80079de4;


typedef int (*make_clut_func) (unsigned char*, unsigned int*, unsigned short, unsigned short, unsigned short, unsigned short);


make_clut_func make_clut_for_string = (make_clut_func) 0x800491fc;

typedef void* (*memset_func) (unsigned char*, unsigned char, int);

memset_func memset = (memset_func) 0x80079e14;


typedef void (*TimInfo ) (unsigned long*, GsIMAGE*);

TimInfo GsGetTim = (TimInfo) 0x80058fb4;



/*0x80195bb0 is where our stuff is expected to be stored, the pointer to it will go to 0x800e1810*/

/*unsigned char font_loaded = 0;*/

void get_char_coords(unsigned char c, RECT* zone_cur){
	/*heart sign is 0x3C (<)
	playstation cross is 0x7C (|)
playstation circle is 0x7D (})
	・ is 0x80
	'...' is 0x81 
	 heart/"triangle" is 0x82
	 playername macros are 0x84-0x8D, 0x8E for the randomized one*/
	if (c == '!') 		c = 0x3D;
	else if (c == '~')  c = 0x3E;
	else if (c == '-')  c = 0x5F;
	else if (c =='\'')  c = 0x60;
	else if (c == ',')  c = 0x7E;
	else if (c == '.')  c = 0x7F;
	else if (c == '*')  c = 0x83;
	else if (c == 0x82) c = 0x3C;
	if (c == 0x20 || c < 0x30 || c > 0x83 || c == ' ') {
		zone_cur->x = 1016;
		zone_cur->y = 488;
		
		return;
	}
	unsigned char pos_x = (c - 0x30) % 4;
	unsigned char pos_y = (c - 0x30) / 4;
	
	zone_cur->x = 1016 + pos_x * zone_cur->w;
	zone_cur->y = 320 +  pos_y * zone_cur->h;
	return;
	
	
}

XYWH xywh_global = {0};

unsigned char* honorific_cur = NULL;
unsigned char* playername_cur = NULL;

/*trick can be:
0xff: none
0: first two chars + random syllabes
1: first two chars + uemon
2: ojisan
3: shortening + (random ?) honorific
4: just the name
*/
void insert_player_name(unsigned char* string_buffer, unsigned char* buffer_pos, char trick){
	
	
	/*specific "honorifics" that replace the playername*/
	if (honorific_cur != NULL && (unsigned int)honorific_cur < 0x80010e38) {
		
		unsigned char len_cur = strlen(honorific_cur);
		strcpy((string_buffer + *buffer_pos), honorific_cur);
		*buffer_pos = *buffer_pos + len_cur;
		
	}
	else {
	
unsigned char playername_size = 4;
unsigned char name_index_cur = '\0';

for (char j = 3; j >= 0; j--) {
	name_index_cur = playername_cur[j];
	
	if (name_index_cur != '\0') break;
	else --playername_size;
}
	

unsigned char syllabe_cur[4];
unsigned char syllabe_len = 0;

unsigned char** hiragana_table = (unsigned char**) 0x8008a23c;

for (unsigned char i = 0; i < playername_size; i++){
	memset(syllabe_cur, 0, 4);
	syllabe_len = 0;
	
	 
	if (trick == -1 || i < 2 ) name_index_cur = playername_cur[i];
	else {
		if (trick  == 0 ) {name_index_cur = random()%HIRAGANA_LIMIT;}
		else if (trick == 1 || trick == 3 ) {
			break;
		}
		
	}
	if (name_index_cur != '\0' && name_index_cur < 	HIRAGANA_LIMIT)  strcpy(syllabe_cur, (unsigned char*) (hiragana_table + name_index_cur - 1));
	
	syllabe_len = strlen(syllabe_cur);
	if (syllabe_len != 0) {
	strcpy((string_buffer + *buffer_pos), syllabe_cur);
	*buffer_pos = *buffer_pos + syllabe_len;	
	}
	else {
		string_buffer[*buffer_pos] = ' ';
		*buffer_pos = *buffer_pos + 1;
	}
}


if (honorific_cur != NULL && trick < 4) {
	/*honorifics management*/
string_buffer[*buffer_pos] = '-';
*buffer_pos = *buffer_pos + 1;
strcpy((string_buffer + *buffer_pos), honorific_cur);

*buffer_pos = *buffer_pos + strlen(honorific_cur);
}
	}

}


int make_pxl_for_string(unsigned char* string_param, unsigned short* PXL_header,
                unsigned short vram_x, unsigned short vram_y,
                unsigned short* width_array)
{
	

	/*load our font from memory to framebuffer, ideally we'd only do it once per screen*/
	/*if  (!font_loaded) {
	text_loader((void*)0x8000C000);
	font_loaded = 1;
	}
	*/
	
	text_loader((void*)0x8000F380);
	
	
	
	RECT zone_cur;
	
	unsigned char string[100];
	memset(string, 0, 100);
	unsigned char  c_cur = *string_param;
	unsigned char pos_cur = 0;
	while (c_cur != '\0') {
		
		if (c_cur < 0x84) {
			string[pos_cur] = c_cur;
			++pos_cur;
		}
		else {
			
			/*playername macros are 0x84-0x8D: uemon, suki, pyon, yan, ppe, rin, cchi, chan, kun, pi
			  special cases: 0x8E random, 0x8F uemon, 0x90 ojisan, 0x91 honorific
			
			*/
			if (c_cur > 0x8D) insert_player_name(string, &pos_cur,c_cur - 0x8E);
			else insert_player_name(string, &pos_cur, 0xff);
		}
			
		++string_param;
		c_cur = *string_param;
	}
	
	pos_cur = 0;
	
	/*clearing the previous text area */
	if (*string != '\0'){
	zone_cur.x = vram_x;
	zone_cur.y = vram_y;
	zone_cur.w = 64;
	if(width_array[1] > 0)
	zone_cur.h = 32;
	else 
	zone_cur.h = 16;
	clear_image(&zone_cur, 0, 0, 0);
	}
	
	
	/*copying the character's CLUT data into the text CLUT area*/
	zone_cur.x = 1008;
	zone_cur.y = 309;
	zone_cur.w = 16;
	zone_cur.h = 2;
	move_image(&zone_cur, 640, 511);
	
	/*copying the character's PXL data into the text PXL area*/
	zone_cur.w = TEXT_W / 4; /*width / 4bpp*/
	zone_cur.h = TEXT_H;
	unsigned char offset_x = 0;
	unsigned char offset_y = 0;
	unsigned char offset_x_max = 1;
	

	count_width_ascii(string, width_array); /*honestly this width array stuff is stupid but whatever let's roll with it*/
	unsigned char char_cur = *string;
	while (char_cur != '\0') {
	if (char_cur == '\n' || char_cur == '\r' || char_cur == '#') {
		if (offset_x > offset_x_max) offset_x_max = offset_x;
		offset_x = 0;
		++offset_y;
		++pos_cur;
		char_cur = string[pos_cur];
		continue;
	}
	if ((offset_x * zone_cur.w > 61)) {
		if ((*(string + 1)) != '\0' && (*(string + 1) != '#')) {
		get_char_coords('-', &zone_cur);
		move_image(&zone_cur, vram_x + offset_x * zone_cur.w, vram_y + offset_y * zone_cur.h);
		if (offset_x > offset_x_max) offset_x_max = offset_x;
		offset_x = 0;
		++offset_y;
		}
		
		
	}
	get_char_coords(char_cur, &zone_cur);	
	move_image(&zone_cur, vram_x + offset_x * zone_cur.w, vram_y + offset_y * zone_cur.h);
	draw_sync(0);
	++offset_x;
	++pos_cur;
	char_cur = string[pos_cur];
	}
	if (offset_x > offset_x_max) offset_x_max = offset_x;
	xywh_global.W = (ushort) offset_x_max * zone_cur.w;
	xywh_global.H = (ushort)TEXT_H + TEXT_H * offset_y;
	
	
	
	
	
	

	unsigned short final_width = zone_cur.w * offset_x_max; //w = 8 * width / 4
	unsigned short final_height = zone_cur.h * (offset_y + 1); //h = 16 * height
	*PXL_header = 0xc + final_width * 2 * final_height;
	*(PXL_header + 1) = 0;
	*(PXL_header + 2) = vram_x;
	*(PXL_header + 3) = vram_y;
	*(PXL_header + 4) = final_width; 
	*(PXL_header + 5) = final_height;
	
	
	/*copying the character's PXL data into memory*/
	zone_cur.x = vram_x;
	zone_cur.y = vram_y;
	
	zone_cur.w = final_width;
	zone_cur.h = final_height;
	store_image(&zone_cur, (unsigned long*) (PXL_header + 6));
	
	draw_sync(0);
	
return 0;







}





void playername_related_display_stuff(short some_val, unsigned char** string_pointers, TIM_data* some_tim, XYWH* xywh, pointers_to_something pointers_list){
	
GsIMAGE image_data;
RECT some_rect;

unsigned short width_array [4];
	
	
	
/*first we handle the name, then the honorifics, then the actual string... what if we want to handle the string first ?
like "Good job, saki-chan !"*/


unsigned char string_buffer[100];

memset(string_buffer, 0, sizeof(string_buffer));

unsigned char buffer_pos = 0;



int some_value = (int) some_val;

if (1 < some_value) {
	some_value = 1;
}


/*handling the name*/
/*
unsigned char* playername_pointer = (unsigned char*) pointers_list.playername_;
unsigned char name_index_cur = '\0';
unsigned char syllabe_cur[4];
unsigned char syllabe_len = 0;
*/

some_rect.x = (ushort) pointers_list.clut_x;
some_rect.y = (ushort) pointers_list.clut_y;
*(ushort*)pointers_list.image_location = some_rect.x;
*(ushort*)((int)pointers_list.image_location + 2) = some_rect.y;
some_rect.w = 16;
some_rect.h = 1;
/*load white clut*/
load_image(&some_rect, (unsigned long*)0x8008a3f4);
draw_sync(0);




unsigned char* string_cur;
unsigned short* offsets_pointer = (unsigned short*) pointers_list.offsets_pointer;

if (some_value > -1) {
honorific_cur = (unsigned char*) pointers_list.honorifics_pointer[*(short *)(((some_value << 0x10) >> 0xf) + (int)offsets_pointer) * 2];
}

else {
	honorific_cur = NULL;
}
playername_cur = (unsigned char*) pointers_list.playername_pointer;






/*supposedly the first string pointer*/



string_cur = *string_pointers;

unsigned char len_cur = 0;

if (string_cur != NULL && string_cur < (unsigned char*)0x80200000) {

strcpy((string_buffer + buffer_pos), string_cur);
len_cur = strlen(string_cur);
buffer_pos = buffer_pos + len_cur + 1;

if (len_cur > 0) { 

string_buffer[buffer_pos] = ' ';
++buffer_pos;
}

}



/*then the second one*/
string_cur = string_pointers[2];

if (string_cur != NULL) {
	
strcpy((string_buffer + buffer_pos), string_cur);

buffer_pos = buffer_pos + len_cur + 1;

if (len_cur > 0) { 

string_buffer[buffer_pos] = ' ';
++buffer_pos;
}
	
}



/*we'll remake the clut just like the original function*/
CLUT_entry* clut_entry_pointer = &(some_tim->CLUT).CLUT_entry1;

    unsigned char j = 0;
    some_tim->ID = 0x10;
	FLAG flag_cur;
	flag_cur.CF = 1;
    some_tim->FLAG = flag_cur;
    (some_tim->CLUT).CLUT_bnum.field0_0x0 = 0x2c;
    (some_tim->CLUT).CLUT_WH.W = 0x10;
    (some_tim->CLUT).CLUT_WH.H = 1;
	
	CLUT_xy clut_xy;
	
	clut_xy.Y = (ushort)pointers_list.clut_y;
    clut_xy.X = (ushort)pointers_list.clut_x;
    (some_tim->CLUT).CLUT_XY = clut_xy;
	
	    do {
      if (j == 0) {
        *(undefined4 *)clut_entry_pointer = CLUT_TRANSPARENT;
      }
      else {
        *(undefined4 *)clut_entry_pointer = CLUT_WHITE;
      }
      j = j + 1;
      ++clut_entry_pointer;
    } while (j < 8);
	


/*let's send our string into the mines, some_tim + 26 is to send the PXL section's address*/




count_width_ascii(string_buffer, width_array);


make_pxl_for_string(string_buffer, ((unsigned short*)some_tim) + 26, (ushort)pointers_list.pxl_x, (ushort)pointers_list.pxl_y, width_array);
GsGetTim((unsigned long*)(&some_tim->FLAG), &image_data);

unsigned char* string_iterator = (unsigned char*)string_buffer;

unsigned char char_cur;

unsigned char w_max = 0;

unsigned char w_cur = 0;


some_rect.x = (ushort) pointers_list.pxl_x;
some_rect.y = (ushort) pointers_list.pxl_y;

xywh->X = some_rect.x;
xywh->Y = some_rect.y;
xywh->W = xywh_global.W;
xywh->H = xywh_global.H;


/*
some_rect.w = xywh->W;
some_rect.h = xywh->H;


load_image(&some_rect, image_data.pixel);
draw_sync(0);
*/



return;
	
	
}





