#include "Pragmas.h"
#include "lcd.h"
#include "the3.h"
#include <string.h>
#include <stdio.h>

// Positions
char A1x = 3;
char A1y = 2;
char A2x = 3;
char A2y = 3;
char B1x = 14;
char B1y = 2;
char B2x = 14;
char B2y = 3;
char frisbee_x = 9;
char frisbee_y = 2;
char flag = 1;
// Target
char target_x = 0;
char target_y = 0;
char is_target_active = 0;


// Selected player
char selected_player = 0; // 0: A1, 1:A2, 2:B1, 3:B2

char pos1 = 1;
char pos2 = 1;

char GAME_MODE = 0; // 0: INACTIVE_MODE, 1: ACTIVE_MODE

unsigned short n_steps = 0;
unsigned short current_step = 0; 

char blink = 1;

char switchFlag = 0;

int scoreA = 0;
int scoreB = 0;

void movePlayerRandom(char player, unsigned short n_random);

int SPEED = 0; // 0: 400 ms , 1: 800 ms , 2: 1200 ms, 3: 1600 ms FOR TIMER0 INTERRUPT INTERVAL FOR MOVEMENTS.  

int timer1counter = 1;

char x = 1;

void busyWait()
{
    for(char i = 2; i < 4; i++ )
    {
        for(char j = 1; j > 0; j--)
        {
            for(char k = 0; k < 255; k++)
            {
                ;
            }
        }
    }
}
void refreshSegmentDisplay(void)
{
    /*
    LATA = 0b00000100;
    switch(scoreA)
    {
        case 0:{
            LATD = 0b00000000;
            break;
        }
        case 1:{
            LATD = 0b00000000;
            break;
        }
        case 2:{
            LATD = 0b00000000;
            break;
        }
        case 3:{
            LATD = 0b00000000;
            break;
        }
        case 4:{
            LATD = 0b00000000;
            break;
        }
        case 5:{
            LATD = 0b00000000;
            break;
        }
    }


    busyWait(); */

    LATA = 0b00001000;
    switch(scoreA)
    {
        case 0:{
            LATD = 0b00111111;
            break;
        }
        case 1:{
            LATD = 0b00000110;
            break;
        }
        case 2:{
            LATD = 0b01011011;
            break;
        }
        case 3:{
            LATD = 0b01001111;
            break;
        }
        case 4:{
            LATD = 0b01100110;
            break;
        }
        case 5:{
            LATD = 0b01101101;
            break;
        }
    }

    busyWait();

    LATA = 0b00010000;
    switch(scoreA)
    {
        case 0:{
            LATD = 0b01000000;
            break;
        }
        case 1:{
            LATD = 0b01000000;
            break;
        }
        case 2:{
            LATD = 0b01000000;
            break;
        }
        case 3:{
            LATD = 0b01000000;
            break;
        }
        case 4:{
            LATD = 0b01000000;
            break;
        }
        case 5:{
            LATD = 0b01000000;
            break;
        }
    }


    busyWait();

    LATA = 0b00100000;
    switch(scoreB)
    {
        case 0:{
            LATD = 0b00111111;
            break;
        }
        case 1:{
            LATD = 0b00000110;
            break;
        }
        case 2:{
            LATD = 0b01011011;
            break;
        }
        case 3:{
            LATD = 0b01001111;
            break;
        }
        case 4:{
            LATD = 0b01100110;
            break;
        }
        case 5:{
            LATD = 0b01101101;
            break;
        }
    }

    busyWait();
}

/*void configureTimer0(void)
{
    INTCONbits.TMR0IE = 0;
    T0CON = 0b11000111;
    TMR0H = 0x00;
    TMR0L = 0x00;
    INTCONbits.TMR0IF = 0;
}*/

unsigned short readADCChannel(unsigned char channel){
    ADCON0bits.CHS0 =  channel & 0x1; 
    ADCON0bits.CHS1 = (channel >> 1) & 0x1;
    ADCON0bits.CHS2 = (channel >> 2) & 0x1;
    ADCON0bits.CHS3 = (channel >> 3) & 0x1;
    
    ADCON0bits.GODONE = 1; 
    while(ADCON0bits.GODONE == 1); 
        
    PIR1bits.ADIF = 0; 
    return (unsigned short)((ADRESH << 8)+ADRESL);    
}


void __interrupt(high_priority) highPriorityISR(void){
    LATAbits.LA2 = 0;
    LATAbits.LA3 = 0;
    LATAbits.LA4 = 0;
    LATAbits.LA5 = 0;
    if (PIR1bits.TMR1IF && PIE1bits.TMR1IE){
        if (timer1counter){
            PIR1bits.TMR1IF = 0;
            timer1counter--;
            return ;
        }
        
        // Move frisbee
        LCDGoto(frisbee_x, frisbee_y);
        
        if (current_step == 0){
            switch (selected_player){
                case 0: {
                    LCDDat(2);
                    break;
                }
                case 1: {
                    LCDDat(2);
                    break;
                }
                case 2: {
                    LCDDat(3);
                    break;
                }
                case 3: {
                    LCDDat(3);
                    break;
                }                   
            }
        }
        else if ((A1x == frisbee_x && A1y == frisbee_y) || (A2x == frisbee_x && A2y == frisbee_y) || (B1x == frisbee_x && B1y == frisbee_y) || (B2x == frisbee_x && B2y == frisbee_y)) {
            if (A1x == frisbee_x && A1y == frisbee_y){
                if (selected_player == 0) LCDDat(2);
                else LCDDat(0);
            }
            else if (A2x == frisbee_x && A2y == frisbee_y){
                if (selected_player == 1) LCDDat(2);
                else LCDDat(0);
            }
            else if (B1x == frisbee_x && B1y == frisbee_y && selected_player != 2){
                if (selected_player == 2) LCDDat(3);
                else LCDDat(1);
            }
            else if (B2x == frisbee_x && B2y == frisbee_y && selected_player != 3){
                if (selected_player == 3) LCDDat(3);
                else LCDDat(1);
            }
        }
        else {
            LCDStr(" ");
        }
        
        
        //if (current_step == 0 && (selected_player == 0 || selected_player == 1)) LCDDat(2);
        //else if (current_step == 0 && (selected_player == 2 || selected_player == 3)) LCDDat(2);
        //else LCDStr(" ");
        
        
        
        frisbee_x = frisbee_steps[current_step][0];
        frisbee_y = frisbee_steps[current_step][1];
        LCDGoto(frisbee_x, frisbee_y);
        LCDDat(6);
        current_step++;
        //refreshSegmentDisplay();
        // Move other players
        switch(selected_player){
            case 0:{
                refreshSegmentDisplay();
                    LATA = 0;
                movePlayerRandom(1, random_generator(9));
                refreshSegmentDisplay();
                    LATA = 0;
                movePlayerRandom(2, random_generator(9));
                refreshSegmentDisplay();
                    LATA = 0;
                movePlayerRandom(3, random_generator(9));
                refreshSegmentDisplay();
                    LATA = 0;
                break;
            }
            case 1:{
                refreshSegmentDisplay();
                    LATA = 0;
                movePlayerRandom(0, random_generator(9));
                refreshSegmentDisplay();
                    LATA = 0;
                movePlayerRandom(2, random_generator(9));
                refreshSegmentDisplay();
                    LATA = 0;
                movePlayerRandom(3, random_generator(9));
                refreshSegmentDisplay();
                    LATA = 0;
                break;
            }
            case 2:{
                refreshSegmentDisplay();
                    LATA = 0;
                movePlayerRandom(0, random_generator(9));
                refreshSegmentDisplay();
                    LATA = 0;
                movePlayerRandom(1, random_generator(9));
                refreshSegmentDisplay();
                    LATA = 0;
                movePlayerRandom(3, random_generator(9));
                refreshSegmentDisplay();
                    LATA = 0;
                break;
            }
            case 3:{
                refreshSegmentDisplay();
                    LATA = 0;
                movePlayerRandom(0, random_generator(9));
                refreshSegmentDisplay();
                    LATA = 0;
                movePlayerRandom(1, random_generator(9));
                refreshSegmentDisplay();
                    LATA = 0;
                movePlayerRandom(2, random_generator(9));
                refreshSegmentDisplay();
                    LATA = 0;
                break;
            }
        }
        if (frisbee_x == target_x && frisbee_y == target_y){
            LCDGoto(target_x, target_y);
            LCDDat(6);
            
            switch(selected_player){
                case 0: {
                    if (A1x == target_x && A1y == target_y){
                        scoreA++;
                        LCDGoto(target_x, target_y);
                        LCDDat(4);
                    }
                    break;
                }
                case 1: {
                    if (A2x == target_x && A2y == target_y){
                        scoreA++;
                        LCDGoto(target_x, target_y);
                        LCDDat(4);
                    }                    
                    break;
                }
                case 2: {
                    if (B1x == target_x && B1y == target_y){
                        scoreB++;
                        LCDGoto(target_x, target_y);
                        LCDDat(5);
                    }
                    break;
                }
                case 3: {
                    if (B2x == target_x && B2y == target_y){
                        scoreB++;
                        LCDGoto(target_x, target_y);
                        LCDDat(5);
                    }
                    break;                    
                }
            }
            
            current_step = 0;
            PIE1bits.TMR1IE = 0;
            PIE1bits.TMR2IE = 0;
            GAME_MODE = 0;
        }   
        
        unsigned short convertion = readADCChannel(0);
        if (convertion >= 0 && convertion <= 255) SPEED = 0;
        else if (convertion >= 256 && convertion <= 511) SPEED = 1;
        else if (convertion >= 512 && convertion <= 767) SPEED = 2;
        else if (convertion >= 768 && convertion <= 1023) SPEED = 3;
        
        switch(SPEED){
            case 0: {
                timer1counter = 1;
                break;
            }
            case 1: {             
                timer1counter = 2;
                break;                
            }
            case 2: {              
                timer1counter = 3;
                break;                 
            }
            case 3: {             
                timer1counter = 4;
                break; 
            }
        }       
        TMR1L = 0x00;
        TMR1H = 0x80;  
        PIR1bits.TMR1IF = 0;
        return ;
    }
    if (INTCONbits.RBIF ){//&& INTCONbits.RBIE && (PORTBbits.RB4 == 0 || PORTBbits.RB5 == 0 || PORTBbits.RB6 == 0 || PORTBbits.RB7 == 0)){       
        if (PORTBbits.RB4 == 0){ // Up
            switch(selected_player){
                case 0: {
                    if (A1y == 1) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}  // Border Check
                    if ((A1x == A2x && A1y-1 == A2y) || (A1x == B1x && A1y-1 == B1y) || (A1x == B2x && A1y-1 == B2y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;} // Another player exist check
                    LCDGoto(A1x,A1y);
                    if (A1x == frisbee_x && A1y == frisbee_y) LCDDat(6); // If currently holding frisbee, leave it
                    else LCDStr(" ");
                    A1y--;
                    LCDGoto(A1x,A1y);
                    if (A1x == frisbee_x && A1y == frisbee_y) LCDDat(4); // Hold the frisbee
                    else LCDDat(2);            
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
                case 1: {
                    if (A2y == 1) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((A2x == A1x && A2y-1 == A1y) || (A2x == B1x && A2y-1 == B1y) || (A2x == B2x && A2y-1 == B2y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(A2x, A2y);
                    if (A2x == frisbee_x && A2y == frisbee_y) LCDDat(6);      
                    else LCDStr(" ");
                    A2y--;
                    LCDGoto(A2x, A2y);
                    if (A2x == frisbee_x && A2y == frisbee_y) LCDDat(4);
                    else LCDDat(2);            
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
                case 2: {
                    if (B1y == 1) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((B1x == A2x && B1y-1 == A2y) || (B1x == A1x && B1y-1 == A1y) || (B1x == B2x && B1y-1 == B2y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(B1x, B1y);
                    if (B1x == frisbee_x && B1y == frisbee_y) LCDDat(6);
                    else LCDStr(" ");
                    B1y--;
                    LCDGoto(B1x, B1y);
                    if (B1x == frisbee_x && B1y == frisbee_y) LCDDat(5);
                    else LCDDat(3);            
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
                case 3: {
                    if (B2y == 1) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((B2x == A2x && B2y-1 == A2y) || (B2x == B1x && B2y-1 == B1y) || (B2x == A1x && B2y-1 == A1y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(B2x, B2y);
                    if (B2x == frisbee_x && B2y == frisbee_y) LCDDat(6);
                    else LCDStr(" ");
                    B2y--;
                    LCDGoto(B2x, B2y);
                    if (B2x == frisbee_x && B2y == frisbee_y) LCDDat(5);
                    else LCDDat(3);            
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
            }
            
        }
        else if (PORTBbits.RB5 == 0){ // Right
            switch(selected_player){
                case 0: {
                    if (A1x == 16) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((A1x+1 == A2x && A1y == A2y) || (A1x+1 == B1x && A1y == B1y) || (A1x+1 == B2x && A1y == B2y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(A1x,A1y);
                    if (A1x == frisbee_x && A1y == frisbee_y) LCDDat(6);
                    else LCDStr(" ");
                    A1x++;
                    LCDGoto(A1x,A1y);
                    if (A1x == frisbee_x && A1y == frisbee_y) LCDDat(4); // Hold the frisbee
                    else LCDDat(2);           
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
                case 1: {
                    if (A2x == 16) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((A2x+1 == A1x && A2y == A1y) || (A2x+1 == B1x && A2y == B1y) || (A2x+1 == B2x && A2y == B2y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(A2x, A2y);
                    if (A2x == frisbee_x && A2y == frisbee_y) LCDDat(6);      
                    else LCDStr(" ");
                    A2x++;
                    LCDGoto(A2x, A2y);
                    if (A2x == frisbee_x && A2y == frisbee_y) LCDDat(4);
                    else LCDDat(2);           
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
                case 2: {
                    if (B1x == 16) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((B1x+1 == A2x && B1y == A2y) || (B1x+1 == A1x && B1y == A1y) || (B1x+1 == B2x && B1y == B2y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(B1x, B1y);
                    if (B1x == frisbee_x && B1y == frisbee_y) LCDDat(6);
                    else LCDStr(" ");
                    B1x++;
                    LCDGoto(B1x, B1y);
                    if (B1x == frisbee_x && B1y == frisbee_y) LCDDat(5);
                    else LCDDat(3);            
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
                case 3: {
                    if (B2x == 16) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((B2x+1 == A2x && B2y == A2y) || (B2x+1 == B1x && B2y == B1y) || (B2x+1 == A1x && B2y == A1y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(B2x, B2y);
                    if (B2x == frisbee_x && B2y == frisbee_y) LCDDat(6);
                    else LCDStr(" ");
                    B2x++;
                    if (B2x == frisbee_x && B2y == frisbee_y) LCDDat(5);
                    else LCDDat(3);           
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
            }
        }
        else if (PORTBbits.RB6 == 0){ // Down
            switch(selected_player){
                case 0: {
                    if (A1y == 4) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((A1x == A2x && A1y+1 == A2y) || (A1x == B1x && A1y+1 == B1y) || (A1x == B2x && A1y+1 == B2y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(A1x,A1y);
                    if (A1x == frisbee_x && A1y == frisbee_y) LCDDat(6);
                    else LCDStr(" ");
                    A1y++;
                    LCDGoto(A1x,A1y);
                    if (A1x == frisbee_x && A1y == frisbee_y) LCDDat(4); // Hold the frisbee
                    else LCDDat(2);                     
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
                case 1: {
                    if (A2y == 4) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((A2x == A1x && A2y+1 == A1y) || (A2x == B1x && A2y+1 == B1y) || (A2x == B2x && A2y+1 == B2y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(A2x, A2y);
                    if (A2x == frisbee_x && A2y == frisbee_y) LCDDat(6);      
                    else LCDStr(" ");
                    A2y++;
                    LCDGoto(A2x, A2y);
                    if (A2x == frisbee_x && A2y == frisbee_y) LCDDat(4);
                    else LCDDat(2);                       
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
                case 2: {
                    if (B1y == 4) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((B1x == A2x && B1y+1 == A2y) || (B1x == A1x && B1y+1 == A1y) || (B1x == B2x && B1y+1 == B2y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(B1x, B1y);
                    if (B1x == frisbee_x && B1y == frisbee_y) LCDDat(6);
                    else LCDStr(" ");
                    B1y++;
                    LCDGoto(B1x, B1y);
                    if (B1x == frisbee_x && B1y == frisbee_y) LCDDat(5);
                    else LCDDat(3);             
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
                case 3: {
                    if (B2y == 4) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((B2x == A2x && B2y+1 == A2y) || (B2x == B1x && B2y+1 == B1y) || (B2x == A1x && B2y+1 == A1y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(B2x, B2y);
                    if (B2x == frisbee_x && B2y == frisbee_y) LCDDat(6);
                    else LCDStr(" ");
                    B2y++;
                    LCDGoto(B2x, B2y);
                    if (B2x == frisbee_x && B2y == frisbee_y) LCDDat(5);
                    else LCDDat(3);               
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
            }
            
        }
        else if (PORTBbits.RB7 == 0){ // Left
            switch(selected_player){
                case 0: {
                    if (A1x == 1){ char temp = PORTB;
                    INTCONbits.RBIF = 0; return; }
                    if ((A1x-1 == A2x && A1y == A2y) || (A1x-1 == B1x && A1y == B1y) || (A1x-1 == B2x && A1y == B2y)) { char temp = PORTB;
                    INTCONbits.RBIF = 0; return; }
                    LCDGoto(A1x,A1y);
                    if (A1x == frisbee_x && A1y == frisbee_y) LCDDat(6);
                    else LCDStr(" ");
                    A1x--;
                    LCDGoto(A1x,A1y);
                    if (A1x == frisbee_x && A1y == frisbee_y) LCDDat(4); // Hold the frisbee
                    else LCDDat(2);               
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
                case 1: {
                    if (A2x == 1) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((A2x-1 == A1x && A2y == A1y) || (A2x-1 == B1x && A2y == B1y) || (A2x-1 == B2x && A2y == B2y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(A2x, A2y);
                    if (A2x == frisbee_x && A2y == frisbee_y) LCDDat(6);      
                    else LCDStr(" ");
                    A2x--;
                    LCDGoto(A2x, A2y);
                    if (A2x == frisbee_x && A2y == frisbee_y) LCDDat(4);
                    else LCDDat(2);           
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
                case 2: {
                    if (B1x == 1) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((B1x-1 == A2x && B1y == A2y) || (B1x-1 == A1x && B1y == A1y) || (B1x-1 == B2x && B1y == B2y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(B1x, B1y);
                    if (B1x == frisbee_x && B1y == frisbee_y) LCDDat(6);
                    else LCDStr(" ");
                    B1x--;
                    LCDGoto(B1x, B1y);
                    if (B1x == frisbee_x && B1y == frisbee_y) LCDDat(5);
                    else LCDDat(3);           
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
                case 3: {
                    if (B2x == 1) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    if ((B2x-1 == A2x && B2y == A2y) || (B2x-1 == B1x && B2y == B1y) || (B2x-1 == A1x && B2y == A1y)) {char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;}
                    LCDGoto(B2x, B2y);
                    if (B2x == frisbee_x && B2y == frisbee_y) LCDDat(6);
                    else LCDStr(" ");
                    B2x--;
                    LCDGoto(B2x, B2y);
                    if (B2x == frisbee_x && B2y == frisbee_y) LCDDat(5);
                    else LCDDat(3);            
                    char temp = PORTB;
                    INTCONbits.RBIF = 0;
                    return;
                }
            }
        } 
        char temp = PORTB;
        INTCONbits.RBIF = 0;
        
        return ;
    }
    
    
    
    else if (INTCONbits.INT0IF){// && INTCONbits.INT0F && (PORTBbits.RB0 == 0)){
        if (GAME_MODE == 1) {
            char temp = PORTB;
            INTCONbits.INT0IF = 0;
            //for (char i=0; i<255;i++) for (char j=0; j<255;j++) ;
            return ;
        }
        switch(selected_player){
            case 0: {  
                if (!(A1x == frisbee_x && A1y == frisbee_y)){INTCONbits.INT0IF = 0; return;}
                GAME_MODE = 1;
                LCDGoto(A1x, A1y);
                LCDDat(2);
                INTCONbits.INT0IF = 0;
                n_steps = compute_frisbee_target_and_route(frisbee_x, frisbee_y);
                target_x = frisbee_steps[n_steps-1][0];
                target_y = frisbee_steps[n_steps-1][1];
                PIE1bits.TMR2IE = 1;
                PIE1bits.TMR1IE = 1;                  
                break;
            }
            case 1: {
                if (!(A2x == frisbee_x && A2y == frisbee_y)) {INTCONbits.INT0IF = 0; return;}
                GAME_MODE = 1;
                LCDGoto(A2x, A2y);
                LCDDat(2);
                INTCONbits.INT0IF = 0;    
                n_steps = compute_frisbee_target_and_route(frisbee_x, frisbee_y);
                target_x = frisbee_steps[n_steps-1][0];
                target_y = frisbee_steps[n_steps-1][1];                
                PIE1bits.TMR2IE = 1;
                PIE1bits.TMR1IE = 1;                  
                break;
            }
            case 2: {
                if (!(B1x == frisbee_x && B1y == frisbee_y)) {INTCONbits.INT0IF = 0; return;}
                GAME_MODE = 1;
                LCDGoto(B1x, B1y);
                LCDDat(3);
                INTCONbits.INT0IF = 0;     
                n_steps = compute_frisbee_target_and_route(frisbee_x, frisbee_y);
                target_x = frisbee_steps[n_steps-1][0];
                target_y = frisbee_steps[n_steps-1][1];                
                PIE1bits.TMR2IE = 1;
                PIE1bits.TMR1IE = 1;                  
                break;
            }
            case 3: {
                if (!(B2x == frisbee_x && B2y == frisbee_y)) {INTCONbits.INT0IF = 0; return;}
                GAME_MODE = 1;
                LCDGoto(B2x, B2y);
                LCDDat(3);                
                INTCONbits.INT0IF = 0;
                n_steps = compute_frisbee_target_and_route(frisbee_x, frisbee_y);
                target_x = frisbee_steps[n_steps-1][0];
                target_y = frisbee_steps[n_steps-1][1];                
                PIE1bits.TMR2IE = 1;
                PIE1bits.TMR1IE = 1;                  
                break;
            }
        }
       
        INTCONbits.INT0IF = 0;
        //for (char i=0; i<255;i++) for (char j=0; j<255;j++) ;
        return ;
    }
    
    else if (INTCON3bits.INT1IF){// && INTCON3bits.INT1IE && (PORTBbits.RB1 == 0)){
        if (x){
            refreshSegmentDisplay();
            refreshSegmentDisplay();
            x--;
            INTCON3bits.INT1IF = 0;
            return;
        }
        else x = 1;
        refreshSegmentDisplay();
        refreshSegmentDisplay();
        if (selected_player == 0 && GAME_MODE == 0 && A1x == frisbee_x && A1y == frisbee_y) {
            INTCON3bits.INT1IF = 0;
            //for (char i=0; i<255;i++) for (char j=0; j<255;j++) ;            
            return;
        }
        else if (selected_player == 1 && GAME_MODE == 0 && A2x == frisbee_x && A2y == frisbee_y) {
            INTCON3bits.INT1IF = 0;
            //for (char i=0; i<255;i++) for (char j=0; j<255;j++) ;            
            return;
        }
        else if (selected_player == 2 && GAME_MODE == 0 && B1x == frisbee_x && B1y == frisbee_y) {
            INTCON3bits.INT1IF = 0;
            //for (char i=0; i<255;i++) for (char j=0; j<255;j++) ;            
            return;
        }
        else if (selected_player == 3 && GAME_MODE == 0 && B2x == frisbee_x && B2y == frisbee_y) {
            INTCON3bits.INT1IF = 0;
            //for (char i=0; i<255;i++) for (char j=0; j<255;j++) ;
            return ;
        }
        
        selected_player = ((selected_player+1)%4);
        if (selected_player == 0){
            LCDGoto(B2x, B2y);
            LCDDat(1);
            LCDGoto(A1x, A1y);
            LCDDat(2);
              
        }
        else if (selected_player == 1){
            LCDGoto(A1x, A1y);
            LCDDat(0);
            LCDGoto(A2x, A2y);
            LCDDat(2);            
        }
        else if (selected_player == 2){
            LCDGoto(A2x, A2y);
            LCDDat(0);
            LCDGoto(B1x, B1y);
            LCDDat(3);             
        }
        else if (selected_player == 3){
            LCDGoto(B1x, B1y);
            LCDDat(1);
            LCDGoto(B2x, B2y);
            LCDDat(3);              
        }
        
        INTCON3bits.INT1IF = 0;
        //TMR0H = 0x00;
        //TMR0L = 0x00;
        //INTCONbits.TMR0IE = 1;
        //refreshSegmentDisplay();
        //for (char i=0; i<255;i++) for(char j=0; j<255; j++ ) ;
        //for(char j=0; j<30; j++ ) refreshSegmentDisplay();
        return;
    }
    
    
    
    
    if (PIE1bits.TMR2IE && PIR1bits.TMR2IF){
        switch(blink)
        {
            case 0:{
                LCDGoto(target_x, target_y);
                LCDStr(" ");
                blink = 1;
                break;
            }
            case 1:{
                LCDGoto(target_x, target_y);
                LCDDat(7);
                blink = 0;  
                break;
            }
        }
        //refreshSegmentDisplay();
        PIR1bits.TMR2IF = 0;
        TMR2 = 0x80;
        return ;
    }

    /*if(INTCONbits.TMR0IF)
    {
        LCDGoto(pos1, 1);
        pos1++;
        LCDDat(6);
        INTCONbits.TMR0IF = 0;
        INTCONbits.TMR0IE = 0;
        INTCON3bits.INT1IE = 1;
        return;
        
    }*/
    //if(switchFlag)
    //{
        //for(char j=0; j<40; j++ ) refreshSegmentDisplay();
        //switchFlag = 1;
    //}
        
    
}


void movePlayerRandom(char player, unsigned short n_random){
    switch(n_random){
        case 0: { // Up
            switch (player){
                case 0: {
                    if (A1y == 1) return;
                    if ((A1x == frisbee_x && A1y-1 == frisbee_y) || (A1x == target_x && A1y-1 == target_y) || (A1x == A2x && A1y-1 == A2y) || (A1x == B1x && A1y-1 == B1y) || (A1x == B2x && A1y-1 == B2y)) return;
                    LCDGoto(A1x, A1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A1y--;
                    LCDGoto(A1x, A1y);
                    LCDDat(0);     
                    break;
                }
                case 1: {
                    if (A2y == 1) return;
                    if ((A2x == frisbee_x && A2y-1 == frisbee_y) || (A2x == target_x && A2y-1 == target_y) || (A2x == A1x && A2y-1 == A1y) || (A2x == B1x && A2y-1 == B1y) || (A2x == B2x && A2y-1 == B2y)) return;
                    LCDGoto(A2x, A2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A2y--;
                    LCDGoto(A2x, A2y);
                    LCDDat(0);
                    break;
                }
                case 2: {
                    if (B1y == 1) return;
                    if ((B1x == frisbee_x && B1y-1 == frisbee_y) || (B1x == target_x && B1y-1 == target_y) || (B1x == A2x && B1y-1 == A2y) || (B1x == A1x && B1y-1 == A1y) || (B1x == B2x && B1y-1 == B2y)) return;
                    LCDGoto(B1x, B1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B1y--;
                    LCDGoto(B1x, B1y);
                    LCDDat(1);                
                    break;
                }
                case 3: {
                    if (B2y == 1) return;
                    if ((B2x == frisbee_x && B2y-1 == frisbee_y) || (B2x == target_x && B2y-1 == target_y) || (B2x == A2x && B2y-1 == A2y) || (B2x == B1x && B2y-1 == B1y) || (B2x == A1x && B2y-1 == A1y)) return;
                    LCDGoto(B2x, B2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B2y--;
                    LCDGoto(B2x, B2y);
                    LCDDat(1);   
                    break;
                }

            }
            break;
        }
        case1: { // Right
            switch (player){
                case 0: {
                    if (A1x == 16) return;
                    if ((A1x+1 == frisbee_x && A1y == frisbee_y) || (A1x+1 == target_x && A1y == target_y) || (A1x+1 == A2x && A1y == A2y) || (A1x+1 == B1x && A1y == B1y) || (A1x+1 == B2x && A1y == B2y)) return;
                    LCDGoto(A1x, A1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A1x++;
                    LCDGoto(A1x, A1y);
                    LCDDat(0);   
                    break;
                }
                case 1: {
                    if (A2x == 16) return;
                    if ((A2x+1 == frisbee_x && A2y == frisbee_y) || (A2x+1 == target_x && A2y == target_y) || (A2x+1 == A1x && A2y == A1y) || (A2x+1 == B1x && A2y == B1y) || (A2x+1 == B2x && A2y == B2y)) return;
                    LCDGoto(A2x, A2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A2x++;
                    LCDGoto(A2x, A2y);
                    LCDDat(0);  
                    break;
                }
                case 2: {
                    if (B1x == 16) return;
                    if ((B1x+1 == frisbee_x && B1y == frisbee_y) || (B1x+1 == target_x && B1y == target_y) || (B1x+1 == A2x && B1y == A2y) || (B1x+1 == A1x && B1y == A1y) || (B1x+1 == B2x && B1y == B2y)) return;
                    LCDGoto(B1x, B1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B1x++;
                    LCDGoto(B1x, B1y);
                    LCDDat(1);   
                    break;
                }
                case 3: {
                    if (B2x == 16) return;
                    if ((B2x+1 == frisbee_x && B2y == frisbee_y) || (B2x+1 == target_x && B2y == target_y) || (B2x+1 == A2x && B2y == A2y) || (B2x+1 == B1x && B2y == B1y) || (B2x+1 == A1x && B2y == A1y)) return;
                    LCDGoto(B2x, B2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B2x++;
                    LCDGoto(B2x, B2y);
                    LCDDat(1);
                    break;
                }

            }
            break;
        }
        case 2:{ // Down
            switch (player){
                case 0: {
                    if (A1y == 4) return;
                    if ((A1x == frisbee_x && A1y+1 == frisbee_y) || (A1x == target_x && A1y+1 == target_y) || (A1x == A2x && A1y+1 == A2y) || (A1x == B1x && A1y+1 == B1y) || (A1x == B2x && A1y+1 == B2y)) return;
                    LCDGoto(A1x, A1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A1y++;
                    LCDGoto(A1x, A1y);
                    LCDDat(0);  
                    break;
                }
                case 1: {
                    if (A2y == 4) return;
                    if ((A2x == frisbee_x && A2y+1 == frisbee_y) || (A2x == target_x && A2y+1 == target_y) || (A2x == A1x && A2y+1 == A1y) || (A2x == B1x && A2y+1 == B1y) || (A2x == B2x && A2y+1 == B2y)) return;
                    LCDGoto(A2x, A2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A2y++;
                    LCDGoto(A2x, A2y);
                    LCDDat(0);
                    break;
                }
                case 2: {
                    if (B1y == 4) return;
                    if ((B1x == frisbee_x && B1y+1 == frisbee_y) || (B1x == target_x && B1y+1 == target_y) || (B1x == A2x && B1y+1 == A2y) || (B1x == A1x && B1y+1 == A1y) || (B1x == B2x && B1y+1 == B2y)) return;
                    LCDGoto(B1x, B1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B1y++;
                    LCDGoto(B1x, B1y);
                    LCDDat(1);  
                    break;
                }
                case 3: {
                    if (B2y == 4) return;
                    if ((B2x == frisbee_x && B2y+1 == frisbee_y) || (B2x == target_x && B2y+1 == target_y) || (B2x == A2x && B2y+1 == A2y) || (B2x == B1x && B2y+1 == B1y) || (B2x == A1x && B2y+1 == A1y)) return;
                    LCDGoto(B2x, B2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B2y++;
                    LCDGoto(B2x, B2y);
                    LCDDat(1);   
                    break;
                }

            }    
            break;
        }
        case 3: { // Left
            switch (player){
                case 0: {
                    if (A1x == 1) return;
                    if ((A1x-1 == frisbee_x && A1y == frisbee_y) || (A1x-1 == target_x && A1y == target_y) || (A1x-1 == A2x && A1y == A2y) || (A1x-1 == B1x && A1y == B1y) || (A1x-1 == B2x && A1y == B2y)) return;
                    LCDGoto(A1x, A1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A1x--;
                    LCDGoto(A1x, A1y);
                    LCDDat(0);  
                    break;
                }
                case 1: {
                    if (A2x == 1) return;
                    if ((A2x-1 == frisbee_x && A2y == frisbee_y) || (A2x-1 == target_x && A2y == target_y) || (A2x-1 == A1x && A2y == A1y) || (A2x-1 == B1x && A2y == B1y) || (A2x-1 == B2x && A2y == B2y)) return;
                    LCDGoto(A2x, A2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A2x--;
                    LCDGoto(A2x, A2y);
                    LCDDat(0);          
                    break;
                }
                case 2: {
                    if (B1x == 1) return;
                    if ((B1x-1 == frisbee_x && B1y == frisbee_y) || (B1x-1 == target_x && B1y == target_y) || (B1x-1 == A2x && B1y == A2y) || (B1x-1 == A1x && B1y == A1y) || (B1x-1 == B2x && B1y == B2y)) return;
                    LCDGoto(B1x, B1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B1x--;
                    LCDGoto(B1x, B1y);
                    LCDDat(1);
                    break;
                }
                case 3: {
                    if (B2x == 1) return;
                    if ((B2x-1 == frisbee_x && B2y == frisbee_y) || (B2x-1 == target_x && B2y == target_y) || (B2x-1 == A2x && B2y == A2y) || (B2x-1 == B1x && B2y == B1y) || (B2x-1 == A1x && B2y == A1y)) return;
                    LCDGoto(B2x, B2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B2x--;
                    LCDGoto(B2x, B2y);
                    LCDDat(1);      
                    break;
                }

            }   
            break;
        }
        case 4: { // Up-right
            switch (player){
                case 0: {
                    if (A1x == 16 || A1y == 1) return;
                    if ((A1x+1 == frisbee_x && A1y-1 == frisbee_y) || (A1x+1 == target_x && A1y-1 == target_y) || (A1x+1 == A2x && A1y-1 == A2y) || (A1x+1 == B1x && A1y-1 == B1y) || (A1x+1 == B2x && A1y-1 == B2y)) return;
                    LCDGoto(A1x, A1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A1y--;
                    A1x++;
                    LCDGoto(A1x, A1y);
                    LCDDat(0); 
                    break;                
                }
                case 1: {
                    if (A2x == 16 || A2y == 1) return;
                    if ((A2x+1 == frisbee_x && A2y-1 == frisbee_y) || (A2x+1 == target_x && A2y-1 == target_y) || (A2x+1 == A1x && A2y-1 == A1y) || (A2x+1 == B1x && A2y-1 == B1y) || (A2x+1 == B2x && A2y-1 == B2y)) return;
                    LCDGoto(A2x, A2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A2y--;
                    A2x++;
                    LCDGoto(A2x, A2y);
                    LCDDat(0);   
                    break;
                }
                case 2: {
                    if (B1x == 16 || B1y == 1) return;
                    if ((B1x+1 == frisbee_x && B1y-1 == frisbee_y) || (B1x+1 == target_x && B1y-1 == target_y) || (B1x+1 == A2x && B1y-1 == A2y) || (B1x+1 == A1x && B1y-1 == A1y) || (B1x+1 == B2x && B1y-1 == B2y)) return;
                    LCDGoto(B1x, B1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B1y--;
                    B1x++;
                    LCDGoto(B1x, B1y);
                    LCDDat(1);  
                    break;
                }
                case 3: {
                    if (B2x == 16 || B2y == 1) return;
                    if ((B2x+1 == frisbee_x && B2y-1 == frisbee_y) || (B2x+1 == target_x && B2y-1 == target_y) || (B2x+1 == A2x && B2y-1 == A2y) || (B2x+1 == B1x && B2y-1 == B1y) || (B2x+1 == A1x && B2y-1 == A1y)) return;
                    LCDGoto(B2x, B2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B2y--;
                    B2x++;
                    LCDGoto(B2x, B2y);
                    LCDDat(1); 
                    break;                
                }     
            }      
            break;
        }
        case 5: { // Down-right
            switch (player){
                case 0: {
                    if (A1x == 16 || A1y == 4) return;
                    if ((A1x+1 == frisbee_x && A1y+1 == frisbee_y) || (A1x+1 == target_x && A1y+1 == target_y) || (A1x+1 == A2x && A1y+1 == A2y) || (A1x+1 == B1x && A1y+1 == B1y) || (A1x+1 == B2x && A1y+1 == B2y)) return;
                    LCDGoto(A1x, A1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A1y++;
                    A1x++;
                    LCDGoto(A1x, A1y);
                    LCDDat(0); 
                    break;
                }
                case 1: {
                    if (A2x == 16 || A2y == 4) return;
                    if ((A2x+1 == frisbee_x && A2y+1 == frisbee_y) || (A2x+1 == target_x && A2y+1 == target_y) || (A2x+1 == A1x && A2y+1 == A1y) || (A2x+1 == B1x && A2y+1 == B1y) || (A2x+1 == B2x && A2y+1 == B2y)) return;
                    LCDGoto(A2x, A2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A2y++;
                    A2x++;
                    LCDGoto(A2x, A2y);
                    LCDDat(0);
                    break;
                }
                case 2: {
                    if (B1x == 16 || B1y == 4) return;
                    if ((B1x+1 == frisbee_x && B1y+1 == frisbee_y) || (B1x+1 == target_x && B1y+1 == target_y) || (B1x+1 == A2x && B1y+1 == A2y) || (B1x+1 == A1x && B1y+1 == A1y) || (B1x+1 == B2x && B1y+1 == B2y)) return;
                    LCDGoto(B1x, B1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B1y++;
                    B1x++;
                    LCDGoto(B1x, B1y);
                    LCDDat(1);   
                    break;
                }
                case 3: {
                    if (B2x == 16 || B2y == 4) return;
                    if ((B2x+1 == frisbee_x && B2y+1 == frisbee_y) || (B2x+1 == target_x && B2y+1 == target_y) || (B2x+1 == A2x && B2y+1 == A2y) || (B2x+1 == B1x && B2y+1 == B1y) || (B2x+1 == A1x && B2y+1 == A1y)) return;
                    LCDGoto(B2x, B2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B2y++;
                    B2x++;
                    LCDGoto(B2x, B2y);
                    LCDDat(1);  
                    break;
                }     
            }     
            break;
        }
        case 6: { // Down-left
            switch (player){
                case 0: {
                    if (A1x == 1 || A1y == 4) return;
                    if ((A1x-1 == frisbee_x && A1y+1 == frisbee_y) || (A1x-1 == target_x && A1y+1 == target_y) || (A1x-1 == A2x && A1y+1 == A2y) || (A1x-1 == B1x && A1y+1 == B1y) || (A1x-1 == B2x && A1y+1 == B2y)) return;
                    LCDGoto(A1x, A1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A1y++;
                    A1x--;
                    LCDGoto(A1x, A1y);
                    LCDDat(0);   
                    break;
                }
                case 1: {
                    if (A2x == 1 || A2y == 4) return;
                    if ((A2x-1 == frisbee_x && A2y+1 == frisbee_y) || (A2x-1 == target_x && A2y+1 == target_y) || (A2x-1 == A1x && A2y+1 == A1y) || (A2x-1 == B1x && A2y+1 == B1y) || (A2x-1 == B2x && A2y+1 == B2y)) return;
                    LCDGoto(A2x, A2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A2y++;
                    A2x--;
                    LCDGoto(A2x, A2y);
                    LCDDat(0);   
                    break;
                }
                case 2: {
                    if (B1x == 1 || B1y == 4) return;
                    if ((B1x-1 == frisbee_x && B1y+1 == frisbee_y) || (B1x-1 == target_x && B1y+1 == target_y) || (B1x-1 == A2x && B1y+1 == A2y) || (B1x-1 == A1x && B1y+1 == A1y) || (B1x-1 == B2x && B1y+1 == B2y)) return;
                    LCDGoto(B1x, B1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B1y++;
                    B1x--;
                    LCDGoto(B1x, B1y);
                    LCDDat(1);   
                    break;
                }
                case 3: {
                    if (B2x == 1 || B2y == 4) return;
                    if ((B2x-1 == frisbee_x && B2y+1 == frisbee_y) || (B2x-1 == target_x && B2y+1 == target_y) || (B2x-1 == A2x && B2y+1 == A2y) || (B2x-1 == B1x && B2y+1 == B1y) || (B2x-1 == A1x && B2y+1 == A1y)) return;
                    LCDGoto(B2x, B2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B2y++;
                    B2x--;
                    LCDGoto(B2x, B2y);
                    LCDDat(1);     
                    break;
                }     
            }   
            break;
        }
        case 7: { // Up-left
            switch (player){
                case 0: {
                    if (A1x == 1 || A1y == 1) return;
                    if ((A1x-1 == frisbee_x && A1y-1 == frisbee_y) || (A1x-1 == target_x && A1y-1 == target_y) ||  (A1x-1 == A2x && A1y-1 == A2y) || (A1x-1 == B1x && A1y-1 == B1y) || (A1x-1 == B2x && A1y-1 == B2y)) return;
                    LCDGoto(A1x, A1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A1y--;
                    A1x--;
                    LCDGoto(A1x, A1y);
                    LCDDat(0);      
                    break;
                }
                case 1: {
                    if (A2x == 1 || A2y == 1) return;
                    if ((A2x-1 == frisbee_x && A2y-1 == frisbee_y) || (A2x-1 == A1x && A2y-1 == A1y) || (A2x-1 == B1x && A2y-1 == B1y) || (A2x-1 == B2x && A2y-1 == B2y)) return;
                    LCDGoto(A2x, A2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    A2y--;
                    A2x--;
                    LCDGoto(A2x, A2y);
                    LCDDat(0);       
                    break;
                }
                case 2: {
                    if (B1x == 1 || B1y == 1) return;
                    if ((B1x-1 == frisbee_x && B1y-1 == frisbee_y) || (B1x-1 == target_x && B1y-1 == target_y) || (B1x-1 == A2x && B1y-1 == A2y) || (B1x-1 == A1x && B1y-1 == A1y) || (B1x-1 == B2x && B1y-1 == B2y)) return;
                    LCDGoto(B1x, B1y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B1y--;
                    B1x--;
                    LCDGoto(B1x, B1y);
                    LCDDat(1);  
                    break;
                }
                case 3: {
                    if (B2x == 1 || B2y == 1) return;
                    if ((B2x-1 == frisbee_x && B2y-1 == frisbee_y) || (B2x-1 == target_x && B2y-1 == target_y) || (B2x-1 == A2x && B2y-1 == A2y) || (B2x-1 == B1x && B2y-1 == B1y) || (B2x-1 == A1x && B2y-1 == A1y)) return;
                    LCDGoto(B2x, B2y);
                    LCDStr(" ");
                    refreshSegmentDisplay();
                    LATA = 0;
                    B2y--;
                    B2x--;
                    LCDGoto(B2x, B2y);
                    LCDDat(1);   
                    break;
                }     
            }      
            break;
        }        
    }

    return ;
}


void initialLCDConfiguaration(void){
    // Initialize LCD
    InitLCD();

    // Load special characters onto LCD
    LCDAddSpecialCharacter(0, teamA_player);
    LCDAddSpecialCharacter(1, teamB_player);
    LCDAddSpecialCharacter(2, selected_teamA_player);
    LCDAddSpecialCharacter(3, selected_teamB_player);
    LCDAddSpecialCharacter(4, selected_teamA_player_with_frisbee);
    LCDAddSpecialCharacter(5, selected_teamB_player_with_frisbee);
    LCDAddSpecialCharacter(6, frisbee);
    LCDAddSpecialCharacter(7, frisbee_target);
    
    // Put objects into their initial positions
    LCDGoto(3,2);
    LCDDat(2);
    
    LCDGoto(3,3);
    LCDDat(0);
    
    LCDGoto(14,2);
    LCDDat(1);
    
    LCDGoto(14,3);
    LCDDat(1);
    
    LCDGoto(9,2);
    LCDDat(6);
    
    return ;
}


void init(void){
    // TRISA = 0;
    TRISD = 0;
    TRISB = 0xF3;

    PORTB = 0;

    RCONbits.IPEN = 0; // Disable priority levels on interrupts
    PIE1bits.TMR1IE = 0;
    PIR1bits.TMR1IF = 0;
    
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 0;
    INTCON3bits.INT1IE = 1;
    INTCON3bits.INT1IF = 0;
    
    
    // Configure ADC
    ////////////////////////////////////////
    ADCON1bits.PCFG3 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG0 = 0;
    
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    
    TRISAbits.RA0 = 1;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    TRISAbits.RA4 = 0;
    TRISAbits.RA5 = 0;

    
    ADCON2bits.ADCS2 = 0;
    ADCON2bits.ADCS1 = 1;
    ADCON2bits.ADCS0 = 0;
    
    
    ADCON2bits.ACQT2 = 0;
    ADCON2bits.ACQT1 = 1;
    ADCON2bits.ACQT0 = 0;
    
    
    ADCON2bits.ADFM = 1;
    
    ADCON0bits.ADON = 1;
    ////////////////////////////////////////
    
    INTCONbits.RBIE = 0;
    
    
    
    INTCONbits.GIE = 1; // Enable interrupts
    INTCONbits.GIEL = 1; // Enable peripheral interrupts
    
    PORTB = PORTB;
    INTCONbits.RBIF = 0;
    INTCONbits.RBIE = 1;
    
    T3CONbits.T3CKPS = 0b10;  // Prescaler 1:4
    T3CONbits.T3RD16 = 1;     // Enables register read/write in one 16-bit operation
    T3CONbits.T3CCP1 = 0;     // Timer3 is not used with CCP1 module
    T3CONbits.T3CCP2 = 0;     // Timer3 is not used with CCP2 module
    T3CONbits.T3SYNC = 0;     // No synchronization required
    T3CONbits.TMR3CS = 0;     // Internal clock (Fosc/4)
    
    TMR3H = 0;               // Clear timer registers
    TMR3L = 0;
    
    T3CONbits.TMR3ON = 1;  // Enable Timer3
    
    // TIMER2 for blinking
    T2CON = 0x7F; 
    PIE1bits.TMR2IE = 0;
    

    TMR2 = 0x80;
    
    // TIMER1 for random movements
    T1CONbits.RD16 = 0;
    T1CONbits.T1RUN = 1;
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.T1CKPS0 = 1;
    T1CONbits.T1SYNC = 0;
    T1CONbits.TMR1CS = 0;
    T1CONbits.TMR1ON = 1;
    
    // TIMER1 is disabled above, look at it
    TMR1L = 0x00;
    TMR1H = 0x80;   
}



void main(void){
    char values[10] = {0};    
    initialLCDConfiguaration();
    //configureTimer0();
    init();
    
    while (1){
        refreshSegmentDisplay();
        //INTCON3bits.INT1IE = 1;
        //INTCONbits.INT0IE = 1;
    }
    
    return ;
}