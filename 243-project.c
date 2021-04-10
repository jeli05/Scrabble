/* This files provides address values that exist in the system */

/* This files provides address values that exist in the system */

#define BOARD                 "DE1-SoC"

/* Memory */
#define DDR_BASE              0x00000000
#define DDR_END               0x3FFFFFFF
#define A9_ONCHIP_BASE        0xFFFF0000
#define A9_ONCHIP_END         0xFFFFFFFF
#define SDRAM_BASE            0xC0000000
#define SDRAM_END             0xC3FFFFFF
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_ONCHIP_END       0xC803FFFF
#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_END         0xC9001FFF

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define JP1_BASE              0xFF200060
#define JP2_BASE              0xFF200070
#define PS2_BASE              0xFF200100
#define PS2_DUAL_BASE         0xFF200108
#define JTAG_UART_BASE        0xFF201000
#define JTAG_UART_2_BASE      0xFF201008
#define IrDA_BASE             0xFF201020
#define TIMER_BASE            0xFF202000
#define AV_CONFIG_BASE        0xFF203000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define AUDIO_BASE            0xFF203040
#define VIDEO_IN_BASE         0xFF203060
#define ADC_BASE              0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE        0xFF709000
#define HPS_TIMER0_BASE       0xFFC08000
#define HPS_TIMER1_BASE       0xFFC09000
#define HPS_TIMER2_BASE       0xFFD00000
#define HPS_TIMER3_BASE       0xFFD01000
#define FPGA_BRIDGE           0xFFD0501C

/* ARM A9 MPCORE devices */
#define   PERIPH_BASE         0xFFFEC000    // base address of peripheral devices
#define   MPCORE_PRIV_TIMER   0xFFFEC600    // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF      0xFFFEC100    // PERIPH_BASE + 0x100
#define ICCICR                0x00          // offset to CPU interface control reg
#define ICCPMR                0x04          // offset to interrupt priority mask reg
#define ICCIAR                0x0C          // offset to interrupt acknowledge reg
#define ICCEOIR               0x10          // offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST       0xFFFED000    // PERIPH_BASE + 0x1000
#define ICDDCR                0x00          // offset to distributor control reg
#define ICDISER               0x100         // offset to interrupt set-enable regs
#define ICDICER               0x180         // offset to interrupt clear-enable regs
#define ICDIPTR               0x800         // offset to interrupt processor targets regs
#define ICDICFR               0xC00         // offset to interrupt configuration regs


#define SDRAM_BASE            0xC0000000
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_CHAR_BASE        0xC9000000

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* Constants for animation */
#define BOX_LEN 2
#define NUM_BOXES 8

#define FALSE 0
#define TRUE 1

// constants for drawing letters on board
#define LEFT 18
#define RIGHT 60
#define TOP 6
#define BOTTOM 48

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

// Begin part3.c code for Lab 7
void wait_for_vsync();
void clear_screen();
void draw_line(int x0, int y0, int x1, int y1, short int line_color);
void plot_pixel(int x, int y, short int line_color);
void draw_rectangle(int x,  int y);
void draw_board();
void draw_rack();
void highlight_tile(int x, int y);
void video_text(int x, int y, char * text_ptr);
void color_tiles(int row, int col);


volatile int pixel_buffer_start; // global variable
const char *letters[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
const char *values[] = {"1", "3", "3", "3", "1", "4", "2", "4", "1", "8", "5", "1", "3", "1", "1", "3", "9", "1", "1", "1", "1", "4", "4", "8", "4", "9"};
// const char *values[] = {1, 3, 3, 3, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 9, 1, 1, 1, 1, 4, 4, 8, 4, 9};
// int frequency[26] = {9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1};

int main(void) {
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

    volatile int *PS2_ptr = (int*)0xFF200100;

    volatile int *SW_ptr = (int *)0xFF200040;
    int PS2_data, SW_data, RVALID;
    char input = 0;
	char input2 = 0;
    char pattern = 0;


    int x, y;
    x = 7;
    y = 7;

    clear_screen();

    draw_board();
    draw_rack();

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            color_tiles(i, j);
        }
    }

    // setup text
    srand(time(NULL));
    video_text(18, 2, "COME PLAY A GAME OF SCRABBLE!");

    int score1 = 0;
    int score2 = 0;

    video_text(1, 1, "PLAYER 1");
    video_text(1, 3, "SCORE: ");
    char displayScore1[4];
    sprintf(displayScore1, "%d", score1);
    video_text(8, 3, displayScore1); // to be updated after each turn

    video_text(66, 1, "PLAYER 2");
    video_text(66, 3, "SCORE: ");
    char displayScore2[4];
    sprintf(displayScore2, "%d", score2);
    video_text(73, 3, displayScore2); // to be updated after each turn

    // show/hide player turn message
    video_text(1, 52, "PLAYER 1 TURN");
    // video_text(1, 52, "             ");

    video_text(66, 52, "PLAYER 2 TURN");
    // video_text(66, 52, "             ");

    video_text(29, 52, "THESE ARE YOUR TILES:");
    video_text(29, 58, "TILES LEFT IN BAG:");

    int letterCount[26] = {9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2, 6, 8, 2, 1, 6, 4, 6, 4, 2, 2, 1, 2, 1};
    int bagSize = 98; // will be set to 100 when blank tiles are included

    // hold each player's tiles on rack
    char* rack1[8];
    char* rack2[8];
    
    for (int i = 0; i < 14; i++) { // assign 7 random tiles each at the beginning
        // create the array for the tile bag
        char *bag[bagSize];
        int subcounter = 0;
        for (int a = 0; a < 26; a++) {
            for (int b = 0; b < letterCount[a]; b++) {
                bag[subcounter] = letters[a];
                subcounter++;
            }
        }

        // for (int j = 0; j < 80; j++) {
        //     video_text(j, 53+i, bag[j]);
        // }

        // pick random tile from bag
        int position = rand() % bagSize;
        
        // show taken out tile on rack
        if (i < 7) {
            rack1[i] = bag[position];
            video_text(30+3*i, 55, rack1[i]);
        }
        if (i > 7) {
            rack2[i] = bag[position];
            // video_text(30+3*i, 55, rack2[i]);
        }

        // char temp[3];
        // sprintf(temp, "%d", bagSize);
        // video_text(1, 40+i, temp);

        // update letter count to reflect tile taken out
        for (int j = 0; j < 26; j++) {
            if (bag[position] == letters[j]) {
                if (letterCount[j] == 0)
                    video_text(1, 6, "FUCK");
                letterCount[j] -= 1;
                if (i < 7)
        		    video_text(31+3*i, 56, values[j]);

                // char displayValue[2];
                // sprintf(displayValue, "%d", values[j]);
        		// video_text(31+3*i, 56, displayValue);
                break;
            }
        }

        // modify array to account for tile taken out
        for (int c = 0; c < position - 1; c++)
            bag[c] = bag[c];
        for (int c = position - 1; c < bagSize - 1; c++)
            bag[c] = bag[c+1];
        bagSize--;
        char displayBagSize[3];
        sprintf(displayBagSize, "%d", bagSize);
        video_text(48, 58, displayBagSize);
        if (bagSize < 10)
            video_text(49, 58, " ");

        // for (int k = 0; k < 26; k++) {
        //     char temp1[3];
        //     sprintf(temp1, "%d", letterCount[k]);
        //     video_text(3+2*i, 10+k, temp1);
        // }
    }

    // following two for loops are temporary
    int letter = 0;
    for (int i = 18; i < 61; i += 3) {
        for (int j = 6; j < 49; j+= 3) {
            // video_text(i, j, letters[letter]); // temporary
            letter++;
            if (letter == 26)
                letter = 0;
            // video_text(i, j, " "); // to clear whole board
        }
    }
    int value = 0;
    for (int i = 19; i < 62; i += 3) {
        for (int j = 7; j < 50; j+= 3) {
            // video_text(i, j, values[value]); // temporary
            value++;
            if (value == 26)
                value = 0;
            // video_text(i, j, " "); // to clear whole board
        } // Q and Z may need to be given values of 9 instead of 10, or we can use X to represent 10
    }

    highlight_tile(x, y);

    while (1) {
		PS2_data = *(PS2_ptr);
        RVALID = PS2_data & 0x8000;

         if (RVALID) {
            input2 = PS2_data & 0xFFFF;
		
            //LEFT ARROW
            if (input2 == (char)0xE06B) {
                while (1) {
                    PS2_data = *(PS2_ptr);
                    RVALID = PS2_data & 0x8000;

                    if (RVALID) {
                        input2 = PS2_data & 0xFFFFFF;

                        //if the key is released
                        if (input2 == (char)0xE0F06B) {
                            //move select highlight to the left
                            draw_board();
                            if (x > 0) {
								x--;
							}
                            highlight_tile(x, y);
                             break;
                        }
                    }
                }
            }

            //RIGHT ARROW
                if (input2 == (char)0xE074) {
                    while (1) {
                        PS2_data = *(PS2_ptr);
                        RVALID = PS2_data & 0x8000;

                        if (RVALID) {
                            input2 = PS2_data & 0xFFFFFF;

                            //if the key is released
                            if (input2 == (char)0xE0F074) {
                                //move select highlight to the left
                                draw_board();
                                if (x < 14) {
								    x++;
							    }
                            highlight_tile(x, y);
                            break;
                        }
                    }
                }
            }

            //UP ARROW
                        if (input2 == (char)0xE075) {
                while (1) {
                    PS2_data = *(PS2_ptr);
                    RVALID = PS2_data & 0x8000;

                    if (RVALID) {
                        input2 = PS2_data & 0xFFFFFF;

                        //if the key is released
                        if (input2 == (char)0xE0F075) {
                            //move select highlight to the left
                            draw_board();
                            if (y > 0) {
								y--;
							}
                            highlight_tile(x, y);
                             break;
                        }
                    }
                }
            }

            //DOWN ARROW
                        if (input2 == (char)0xE072) {
                while (1) {
                    PS2_data = *(PS2_ptr);
                    RVALID = PS2_data & 0x8000;

                    if (RVALID) {
                        input2 = PS2_data & 0xFFFFFF;

                        //if the key is released
                        if (input2 == (char)0xE0F072) {
                            //move select highlight to the left
                            draw_board();
                            if (y < 14) {
								y++;
							}
                            highlight_tile(x, y);
                            break;
                        }
                    }
                }
            }
         }


        SW_data = *(SW_ptr);
        RVALID = SW_data & 0x8000;

        if (RVALID) {
             pattern = SW_data & 0xFFFF;
		
            //switches???
            if (pattern & 0x00000001) {
               clear_screen();
            }
        }
    }
}

void draw_board() {
    for (int i = 68; i < 249; i+= 12) {
        draw_line(i, 21, i, 201, WHITE);
    }

    for (int i = 21; i < 202; i+= 12) {
        draw_line(68, i, 248, i, WHITE);
    }
}

void draw_rack() {
    for (int i = 116; i <= 200; i+= 12) {
        draw_line(i, 217, i, 229, WHITE);
    }

    for (int i = 217; i <= 229; i+= 12) {
        draw_line(116, i, 200, i, WHITE);
    }
}

void color_tiles(int row, int col) {
    if (row == 0 ||  row == 14) {
        if (col == 0 || col == 7 || col == 14) {
            color_square(row, col, MAGENTA);
        }
    }
    if (row == 8 && (col == 0 || col == 14)) {
        color_square(row, col, MAGENTA);
    }

    if (row == 1 || row == 2 || row == 3 || row == 4 || row ==7 || row == 10 || row == 11 || row == 12 || row == 13) {
        if (col == 1 || col == 2 || col == 3 || col == 4 || col ==7 || col == 10 || col == 11 || col == 12 || col == 13) {
             if (row == col) {
                color_square(row, col, PINK);
             }

             if ((row == 13 && col == 1) || (row == 12 && col == 2) || (row == 11 && col == 3) || (row == 10 && col == 4)) {
                 color_square(row, col, PINK);
             }

             if ((col == 13 && row == 1) || (col == 12 && row == 2) || (col == 11 && row == 3) || (col == 10 && row == 4)) {
                 color_square(row, col, PINK);
             }
        }
    }

    if ((row == 0 || row == 14 || row == 7) && (col == 3 || col == 11)) {
        color_square(row, col, CYAN);
    }

    if ((row == 2 || row == 12) && (col == 6 || col == 8)) {
        color_square(row, col, CYAN);
    }

    if ((row == 3 || row == 11) && (col == 0 || col == 7 || col == 14)) {
        color_square(row, col, CYAN);
    }

    if ((row == 6 || row == 8) && (col == 2 || col == 6 || col == 8 || col == 12)) {
        color_square(row, col, CYAN);
    }

    if ((row == 1 || row == 13) && (col == 5 || col == 9)) {
        color_square(row, col, BLUE);
    }

    if ((row == 5 || row == 9) && (col == 5 || col == 9 || col == 1 || col == 13)) {
        color_square(row, col, BLUE);
    }

}

void color_square(int x, int y, short int color) {
     int x_coord = 68 + 12*x;
    int y_coord = 21 + 12*y;

    for (int i = 0; i < 12; i++) {
        draw_line(x_coord+1, y_coord+1+i, x_coord+11, y_coord+1+i, color); //top line
    }

    //draw_line(x_coord, y_coord, x_coord+12, y_coord, RED); //top line
    //draw_line(x_coord, y_coord+12, x_coord+12, y_coord+12, RED); // bottom line
    //draw_line(x_coord, y_coord, x_coord, y_coord+12, RED); //left line
    //draw_line(x_coord+12, y_coord, x_coord+12, y_coord+12, RED); //right line
}

// void draw_board() {
//     for (int i = 47; i < 274; i+= 15) {
//         draw_line(i, 7, i, 232, WHITE);
//     }

//     for (int i = 7; i < 233; i+= 15) {
//         draw_line(47, i, 273, i, WHITE);
//     }
// }

void highlight_tile(int x, int y) {

    int x_coord = 68 + 12*x;
    int y_coord = 21 + 12*y;

    draw_line(x_coord, y_coord, x_coord+12, y_coord, RED); //top line
    draw_line(x_coord, y_coord+12, x_coord+12, y_coord+12, RED); // bottom line
    draw_line(x_coord, y_coord, x_coord, y_coord+12, RED); //left line
    draw_line(x_coord+12, y_coord, x_coord+12, y_coord+12, RED); //right line
}

void video_text(int x, int y, char * text_ptr) {
    int offset;
    volatile char * character_buffer = (char *)FPGA_CHAR_BASE; // video character buffer
    /* assume that the text string fits on one line */
    offset = (y << 7) + x;
    while (*(text_ptr)) {
        *(character_buffer + offset) = *(text_ptr); // write to the character buffer
        ++text_ptr;
        ++offset;
    }
}

void clear_screen() {
    for (int x = 0; x<320; x++) {
        for (int y = 0; y < 240; y++) {
            plot_pixel(x, y, 0x0000);
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, short int line_color) {
    bool is_steep = abs(y1 - y0) > abs(x1 - x0);
    if (is_steep) {
         int temp = x0;
    	 x0 = y0;
    	 y0 = temp;
		
		temp = x1;
    	x1 = y1;
    	y1 = temp;
        
    }
    if (x0 > x1) {
		int temp = x0;
    	x0 = x1;
    	x1 = temp;
        
		temp = y0;
    	y0 = y1;
    	y1 = temp;
    }
    
    int deltax = x1 - x0;
    int deltay = abs(y1 - y0);
    int error = -(deltax / 2);
    int y_step = 0;

    int y = y0;
    if (y0 < y1 ) { 
        y_step = 1;
    } 
    else {
        y_step = -1;
    }

    for (int x = x0; x <= x1; x++) {
        if (is_steep) {
            plot_pixel(y, x, line_color);
        }
        else {
            plot_pixel(x, y, line_color);
        }
        error = error + deltay;
        if (error >= 0) {
            y = y + y_step;
            error = error - deltax;
        }
    }
}


void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void wait_for_vsync() {
	
	volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    register int status;

    *pixel_ctrl_ptr = 1; //Start synchronization process
    status = *(pixel_ctrl_ptr + 3);
    while ((status & 0x01) != 0) {
        status = *(pixel_ctrl_ptr + 3);
    }
}
