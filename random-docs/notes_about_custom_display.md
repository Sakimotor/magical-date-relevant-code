```c
typedef enum color {
    WHITE,
    RED,
    AZURE,
    GREEN,
    CYAN,
    YELLOW,
    PURPLE,
    LIGHT_BLUE
} color;
```

| Data Type | Location | Length |
| --------- | -------- | ------ |
| Current Color   | 0x800b1b10 | 1 |
| Array of Colors | 0x800b16ac | 16 (?) |
| Current Text 	  | 0x801fffb0 | 16 (?) |
| Array of Strings| 0x8010bbe4 | 1120 |

 

function that displays `small text_80022840(int x_offset, int y_offset, char* string)`
```
             int               v0:4           <RETURN>
             int               a0:4           x_offset
             int               a1:4           y_offset                           
             char*             a2:4           string
```

- `x` 's maximal value is 0x27
- `y` 's maximal value is 0x1b

We can fit 1120 characters on the screen. That would make 28 lines, and 40 characters per line.


```c
small text_80022840(int x_offset, int y_offset, char* string)
{
  int iVar1;
  int iVar2;
  int i;
  char char_cur;
  
  index = DAT_8010bb00;
  if (DAT_8010bb00 < 1) {
    iVar1 = y_offset * 40 + x_offset; // a set (0x12, 0x5) would give the number (0xc8 + 0x12 = 0xda)
    index = -1;
    if (-1 < iVar1 * 0x10000) { //iVar1 * 0x10000 can only be negative if the 16th bit is equal to 1 (so something like 0xffff)
      index = 0;
      char_cur = *string;
      while (char_cur != '\0') { //basically this loops occurs until we reach a nul character or the end of screen
        iVar2 = (int)(short)iVar1; //only keep the last 2 bytes... is it even necessary?
        if (STRING_ON_SCREEN_ARRAY_SIZE < iVar2) { //the max value is 0x045f so iVar1's last 2 bytes must be smaller than that, basically we need to fit into the array
          return -1;
        }
        color_array_800b16ac[iVar2] = color_800b1b10; //the color of character iVar2 is the one at position 0x800b1b10
        string_array_8010bbe4[iVar2] = string[(short)i]; //the character iVar2 is the one located at string[i]
        iVar1 = iVar1 + 1; //increment cursor position on the screen
        if (string[(short)index] == ' ') {
          string_array_8010bbe4[iVar2] = 0xff; //if we meet a space we replace the current character to display with 0xff
        }
        index = index + 1; //increment cursor position for the string
        char_cur = string[index * 0x10000 >> 0x10]; //???????? it's literally just string[index]
      }
      index = 0; //if all went well
    }
  }
  return i; //if whatever is at 0x8010bb00 is bigger than 0
}