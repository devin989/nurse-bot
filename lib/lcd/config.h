#ifndef CONFIG_H_
#define CONFIG_H_

#define LCD_DATA B //Port AVR's PB0-PB3 are connected to LCD's DB4-DB7
#define LCD_DATA_POS 0

#define LCD_E B		  //Enable/strobe signal (E)-> PB4
#define LCD_E_POS PB1 //Position of enable in above port

#define LCD_RS B //Register Select signal (RS)-> PB0
#define LCD_RS_POS PB0

//#define LCD_RW D //Read/Write signal (R/W) ->PD6
//#define LCD_RW_POS PD6

//#define LCD_TYPE_202	//For 20 Chars by 2 lines

//#define LCD_TYPE_204	//For 20 Chars by 4 lines

#define LCD_TYPE_162 //For 16 Chars by 2 lines

//#define LCD_TYPE_164	//For 16 Chars by 4 lines

#endif /* CONFIG_H_ */