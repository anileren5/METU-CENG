PROCESSOR    18F4620

#include <xc.inc>

; CONFIGURATION (DO NOT EDIT)
CONFIG OSC = HSPLL      ; Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
CONFIG FCMEN = OFF      ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
CONFIG IESO = OFF       ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
; CONFIG2L
CONFIG PWRT = ON        ; Power-up Timer Enable bit (PWRT enabled)
CONFIG BOREN = OFF      ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
CONFIG BORV = 3         ; Brown Out Reset Voltage bits (Minimum setting)
; CONFIG2H
CONFIG WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
; CONFIG3H
CONFIG PBADEN = OFF     ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
CONFIG LPT1OSC = OFF    ; Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
CONFIG MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
; CONFIG4L
CONFIG LVP = OFF        ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
CONFIG XINST = OFF      ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

; GLOBAL SYMBOLS
; You need to add your variables here if you want to debug them.
GLOBAL prev_R0
GLOBAL prev_R2
GLOBAL prev_R3
GLOBAL prev_R4
GLOBAL prev_R1
GLOBAL var1
GLOBAL var2
GLOBAL is_paused
GLOBAL bar_length
GLOBAL speed

    
; Define space for the variables in RAM
PSECT udata_acs
temp1:
    DS 1
temp2: 
    DS 1
temp3: 
    DS 1
var1:
    DS 1
var2:
    DS 1
prev_R0:
    DS 1
prev_R2:
    DS 1
prev_R3:
    DS 1
prev_R4:
    DS 1
prev_R1:
    DS 1
bar_length:
    DS 1
current_note:
    DS 1
is_paused:
    DS 1
speed: ; 0: 1x speed , 1: 2x speed
    DS 1
    
    
PSECT resetVec,class=CODE,reloc=2
resetVec:
    goto       main

PSECT CODE
main:
    CALL INIT
main_loop:
    CALL CHECK_BUTTON_R0
    CALL CHECK_BUTTON_R1
    CALL CHECK_BUTTON_R2
    CALL CHECK_BUTTON_R3
    CALL CHECK_BUTTON_R4
    CALL METRONOME_UPDATE
    GOTO main_loop
  

    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CHECK_BUTTON_R0:
    BTFSC prev_R0, 0
    GOTO PREV_IS_1_R0
    MOVFF PORTB, prev_R0
    RETURN    
PREV_IS_1_R0:
    BTFSS PORTB, 0
    GOTO CURRENT_IS_0_R0
    MOVFF PORTB, prev_R0
    RETURN
CURRENT_IS_0_R0:
    CALL RB0_CLICKED
    MOVFF PORTB, prev_R0
    RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RB0_CLICKED:
    COMF is_paused
    RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CHECK_BUTTON_R2:
    BTFSC prev_R2, 2
    GOTO PREV_IS_1_R2
    MOVFF PORTB, prev_R2
    RETURN    
PREV_IS_1_R2:
    BTFSS PORTB, 2
    GOTO CURRENT_IS_0_R2
    MOVFF PORTB, prev_R2
    RETURN
CURRENT_IS_0_R2:
    CALL RB2_CLICKED
    MOVFF PORTB, prev_R2
    RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RB2_CLICKED:
    MOVLW 4     
    MOVWF bar_length , a
    RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   
   
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CHECK_BUTTON_R3:
    BTFSC prev_R3, 3
    GOTO PREV_IS_1_R3
    MOVFF PORTB, prev_R3
    RETURN    
PREV_IS_1_R3:
    BTFSS PORTB, 3
    GOTO CURRENT_IS_0_R3
    MOVFF PORTB, prev_R3
    RETURN
CURRENT_IS_0_R3:
    CALL RB3_CLICKED
    MOVFF PORTB, prev_R3
    RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RB3_CLICKED:
    DECF bar_length
    RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CHECK_BUTTON_R4:
    BTFSC prev_R4, 4
    GOTO PREV_IS_1_R4
    MOVFF PORTB, prev_R4
    RETURN    
PREV_IS_1_R4:
    BTFSS PORTB, 4
    GOTO CURRENT_IS_0_R4
    MOVFF PORTB, prev_R4
    RETURN
CURRENT_IS_0_R4:
    CALL RB4_CLICKED
    MOVFF PORTB, prev_R4
    RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;    
RB4_CLICKED:
    INCF bar_length
    RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

   
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
CHECK_BUTTON_R1:
    BTFSC prev_R1, 1
    GOTO PREV_IS_1_R1
    MOVFF PORTB, prev_R1
    RETURN    
PREV_IS_1_R1:
    BTFSS PORTB, 1
    GOTO CURRENT_IS_0_R1
    MOVFF PORTB, prev_R1
    RETURN
CURRENT_IS_0_R1:
    CALL RB1_CLICKED
    MOVFF PORTB, prev_R1
    RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RB1_CLICKED:
    COMF speed
    RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Helper functions to update for metronome update
ADD_9_TO_VAR1:
    movlw 9
    addwf var1
    RETURN

ADD_5_TO_VAR1:
    nop
    nop
    nop
    movlw 5
    addwf var1
    RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;    
METRONOME_UPDATE:
    BTFSC is_paused, 0
    GOTO UPDATE_PAUSE
    BTFSC speed, 1
    CALL ADD_9_TO_VAR1
    BTFSS speed, 1
    CALL ADD_5_TO_VAR1
    nop
    nop
    nop
    nop
    bc var1_overflown
    return
    var1_overflown:
	incf var2
	incf var2
	bc var2_overflown
	return
    var2_overflown:
	BTFSC LATA, 0
	GOTO _CLOSE
	MOVF bar_length, 0
	SUBWF current_note, 0
	BZ _LAST
	MOVLW 0x01
	SUBWF current_note, 0
	BZ _START
	GOTO _INTERMEDIATE
	    _START:
		MOVLW 00000011B
		MOVWF LATA, a
		INCF current_note, 1
		RETURN

	    _INTERMEDIATE:
		MOVLW 00000001B
		MOVWF LATA, a
		INCF current_note, 1
		RETURN

	    _LAST:
    		MOVLW 00000001B
		MOVWF LATA, a
		MOVLW 0x01
		MOVWF current_note
		RETURN
	    _CLOSE:
		CLRF LATA
		RETURN
    UPDATE_PAUSE:
	BTFSC LATA, 2
	GOTO ALREADY_PAUSED
	MOVLW 00000100B
	MOVWF LATA, a
	ALREADY_PAUSED:
	    RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Turn on R0,R1,R2 for 1000 ms then turn off 
INIT:
              
    CLRF prev_R0
    CLRF prev_R2
    CLRF prev_R3
    CLRF prev_R4
    CLRF prev_R1
    CLRF var1
    CLRF var2
    CLRF is_paused
    CLRF speed

    ; set initial bar length to 4
    MOVLW 4     
    MOVWF bar_length , a
    
    ; set current note made to 4
    MOVLW 1
    MOVWF current_note, a    
    
    
    ; PORTA shall be used for output
    CLRF TRISA
    
    ; PORTB shall be used for input
    SETF TRISB
    
    MOVLW 00000111B
    MOVWF LATA, a
    
    CALL DELAY_1000
       
    CLRF LATA
    
    RETURN    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	  
  
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;		
; 1002 ms delay
DELAY_1000:
    MOVLW 13
    MOVWF temp3	
    _DELAY_1000_LOOP3:
	MOVLW 0x9F  
	MOVWF temp2    
	_DELAY_1000_LOOP2:
	    MOVLW 0xA0	
	    MOVWF temp1	
	    _DELAY_1000_LOOP1:
		DECFSZ temp1,F 
		GOTO _DELAY_1000_LOOP1
		DECFSZ temp2,F 
		GOTO _DELAY_1000_LOOP2 
		DECFSZ temp3,F 
		GOTO _DELAY_1000_LOOP3 
		RETURN
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;		

end resetVec
