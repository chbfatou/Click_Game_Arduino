#include <stdio.h>
#include<stdlib.h>
#include <util/delay.h>

#include "uart.hpp"
#include "HD44780.hpp"
#include "libADC.hpp"



int main(void) {
    // Initializer functions
    LCD_Initalize();
    ADC_Init();
    sei();

    // Variables
    uint16_t raw, rawOld;
    bool updateScreen;
    char buf[10];

    // Game variables
    int player1Score = 0;
    int player2Score = 0;

    while (1) {
        // Measure ADC
        raw = ADC_conversion();

        // Check if ADC measurement has changed
        if ((raw - rawOld) < 50) {
            rawOld = raw;
        } else {
            // RIGHT button (Player 2)
            if (raw < 100) {

                updateScreen = true;
                player2Score++;
            }
            // LEFT button (Player 1)
            else if (raw < 500) {

                updateScreen = true;
                player1Score++;
            }
        }
        // If a button was touch do that
        if (updateScreen) {

            // put the name of the player
            LCD_WriteCommand(HD44780_CLEAR);
            _delay_ms(10);
			LCD_Clear();
            LCD_GoTo(0,0);
			LCD_WriteText("PLAYER1");
			LCD_GoTo(9,0);
			LCD_WriteText("PLAYER2");
            //write the score of the player
            LCD_GoTo(0,1);
            sprintf(buf, "%d", player1Score);
            LCD_WriteText(buf);
            LCD_GoTo(9,1);
            sprintf(buf,"%d",player2Score);
            LCD_WriteText(buf);
            _delay_ms(200);

            updateScreen = false;
        }
        if (player1Score == 100 ){
            LCD_Clear();
            LCD_WriteText("PLAYER 1 WIN !");
            // renitialize the score
            cli();
            player1Score = 0;
            player2Score = 0;
            _delay_ms(300);
        }
        if (player2Score == 100 ){
            LCD_Clear();
            LCD_WriteText("PLAYER 2 WIN !");
            // renitialize the score
            cli();
            player1Score = 0;
            player2Score = 0;
            _delay_ms(300);

        }
    }
    return 0;
}
