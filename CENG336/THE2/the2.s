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
GLOBAL temp, defT0H,defT0L, timer0H,timer0L, portBTemp, speed, barLength, currentBar, var2, var3, var1, oneLed, twoLed, threeLed, fourLed, fiveLed, sixLed, sevenLed, eightLed, nineLed, displayP, displayHyphen, displayNone, barLengthDisplay, currentBarDisplay, speedDisplay, paused, display1, display2, display3, display4
    

; Define space for the variables in RAM
PSECT udata_acs
temp: ; temp variable when needed
  DS 1 
timer0H: ; to keep the timer0 values until the next refresh of timer0
  DS 1
timer0L:
  DS 1
defT0H:  ; default values for timer0
  DS 1
defT0L:
  DS 1
portBTemp: ; portb values before the interrupt
  DS 1 
speed: ; current speed
  DS 1 
barLength: ; current bar length
  DS 1 
currentBar: ; in which bar we operate
  DS 1 
speedDisplay: ; display led config for current speed
  DS 1 
barLengthDisplay: ; display led config for current bar length
  DS 1
currentBarDisplay: ; display led config for current bar number
  DS 1
var2: ; vars for waiting around 3ms via wait function between display selections
  DS 1
var1:
  DS 1
var3:
  DS 1
paused: ; paused flag, used in many ISRs to operate accordingly
  DS 1 
;;;;;; led configs, led configs of numbers and special characters are stored for ease 
oneLed: 
  DS 1 
twoLed:
  DS 1 
threeLed:
  DS 1 
fourLed:
  DS 1 
fiveLed:
  DS 1 
sixLed:
  DS 1 
sevenLed:
  DS 1 
eightLed:
  DS 1 
nineLed:; 9
  DS 1 
displayP: ; P
  DS 1 
displayNone: ;
  DS 1 
displayHyphen: ; -
  DS 1
;;;;;; current configs of 7 segment displays stored here, used in the display function in the main loop
display1: 
  DS 1
display2:
  DS 1
display3:
  DS 1
display4:
  DS 1

PSECT CODE
org 0x0000
  goto main

;;;;;;;;;;ISR;;;;;;;;;;
org 0x0008
interrupt_service_routine: ; can deal with multiple interrupts at the same time
  btfsc INTCON, 0
  call portBInterrupt ; deal with portB interrupts
  btfsc INTCON, 2
  call timer0_interrupt ; end of a note
  btfsc PIR1, 0
  call timer1_interrupt ; end of a pulse
  retfie  1                 ; No match

;;;;;;;;;;ISRs;;;;;;;;;;
portBInterrupt:
    bcf INTCON, 0 ; clear portB interrupt flag
    movf PORTB, W ; to check the portB bits safely, transfer to WREG
    xorwf portBTemp, 0 ; with xor, find which port pins caused the interrupt
    btfsc WREG, 4
    goto interrupt4
    btfsc WREG, 5
    goto interrupt5
    btfsc WREG, 6
    goto interrupt6
    btfsc WREG, 7
    goto interrupt7
    return
    ;;;;;; Rising Edge Checks for PORTB interrupts
    interrupt4: ; PORTB[4] 
	btg portBTemp, 4 ; change temp portB value for correct later interrupt compares
	btfss PORTB, 4 ; see if 1 to 0 interrupt
	call pauseResume
	return
    interrupt5: ; PORTB[5] 
	btg portBTemp, 5 ; change temp portB value for correct later interrupt compares
	btfss PORTB, 5 ; see if 1 to 0 interrupt
	call increaseBarSpeed
	return
    interrupt6: ; PORTB[6] 
	btg portBTemp, 6 ; change temp portB value for correct later interrupt compares
	btfss PORTB, 6 ; see if 1 to 0 interrupt
	call decreaseBarSpeed
	return
    interrupt7: ; PORTB[7] 
	btg portBTemp, 7 ; change temp portB value for correct later interrupt compares
	btfss PORTB, 7 ; see if 1 to 0 interrupt
	call resetMetronome
	return
;;;;;;;;;; PORTB[7] interrupt handler	
resetMetronome:
    btfsc paused, 0 ; check if metronome is paused when reset requested
    goto resetOnPause ; requested on pause, goto intended code piece
    ;;;;;; Reset when running
    btfss currentBar, 2 ; check if current bar > 4, if so; start from first bar
    goto backTo4 ; If not set barLength to 4 safely
    ;;;;;; If > 4, set currentBar to 3 (corresponds to note 4 in my implementation)
    ;;;;;; So that in next timer0 interrupt, it starts from first note 
    movlw 3
    movwf currentBar
    ;;;;;; IF reset is possible:
    backTo4:
    movlw 4
    movwf barLength ; barLength set to default
    movff fourLed, barLengthDisplay ; display 4
    movff barLengthDisplay, display4
    return
  ;;;;; Metronome was paused when reset requested
  resetOnPause:
    movff sixLed, speedDisplay ;set speed and display to 6
    movff speedDisplay, display4
    movlw 6
    movwf speed
    movff defT0H, timer0H ; set default timer values
    movff defT0L, timer0L
    movff timer0H, TMR0H
    movff timer0L, TMR0L
    return
;;;;;;;;;; Pause/resume PORTB[4] interrupt handler 
pauseResume:
    btfsc paused, 0 ; see if paused when pause/resume asked
    goto resume
    movff displayP, display1 ; case of running to paused, set displays accordingly
    movff displayNone, display2
    movff displayNone, display3
    movff speedDisplay, display4
    movlw 1
    movwf paused ; set paused
    clrf currentBar ; currentBar is set to 0 (not to 1 due to my implementation)
    movff oneLed, currentBarDisplay ; set bar display to 1
    bcf     PIE1, 0    ; Disable TMR1 interrupt
    bcf     INTCON, 5 ; Disable TMR0 interrupt
    return
  ;;;;; Metronomo was paused, resume
  resume: ; was paused, so resume
    movff displayNone, display1 ; set display to running configuration
    movff currentBarDisplay, display2
    movff displayHyphen, display3
    movff barLengthDisplay, display4
    clrf paused ; clear paused
    
    call configure_timer1 ; configure timer1, 50ms, initially
    call configure_timer0 ; configure timer0 with stored timer0 values
    
    bsf     PIE1, 0    ; Enable TMR1 interrupt
    bsf     INTCON, 5 ; Enable TMR0 interrupt
    bsf LATC, 0 ; light up both RC lights
    bsf LATC, 1
    return
;;;;;;;;;; Increase button handler, PORTB[5], bar or speed depending on paused or resumed    
increaseBarSpeed:
    btfsc paused, 0
    goto increaseSpeed ; if paused 
    ;INCREASE BAR LENGTH 
    movff barLength, temp
    movlw 8
    subwf temp
    BTFSC STATUS, 2
    goto barLength8 ; barlenght == 8 is checked, if so, just refresh display value, unnecessary but not problematic
    incf barLength ; if not 8, inc barLength
  
    btfsc barLength, 3 ; all those checks are to determine the new barlength
    goto barLength8
    btfsc barLength, 2
    goto barLength4567
    btfsc barLength, 1
    goto barLength23
    goto barLength1
  ;;;;;;;;;; BIT CHECKS to determine barLength, tags have the range of values barLength could take.
  barLength8: ; barLength is 8
    movlw 8
    movwf barLength
    movff eightLed, barLengthDisplay
    goto barLengthFound
  barLength4567: ; barLength is 4,5,6 or 7, remaining code pieces works with the same logic 
    btfsc barLength, 1
    goto barLength67
    goto barLength45
  barLength45:  ; barLength is 4 or 5
    btfsc barLength, 0
    goto barLength5
    goto barLength4
  barLength67: ; barLength is 6 or 7
    btfsc barLength, 0
    goto barLength7
    goto barLength6
  barLength23: ; barLength is 2 or 3
    btfsc barLength, 0
    goto barLength3
    goto barLength2
  barLength7: ; barLength is 7
    movff sevenLed, barLengthDisplay
    goto barLengthFound
  barLength6: ; barLength is 6
    movff sixLed, barLengthDisplay
    goto barLengthFound
  barLength5: ; barLength is 5
    movff fiveLed, barLengthDisplay
    goto barLengthFound
  barLength4: ; barLength is 4
    movff fourLed, barLengthDisplay
    goto barLengthFound
  barLength3: ; barLength is 3
    movff threeLed, barLengthDisplay
    goto barLengthFound
  barLength2: ; barLength is 2
    movff twoLed, barLengthDisplay
    goto barLengthFound
  barLength1: ; barLength is 1
    movff oneLed, barLengthDisplay
    goto barLengthFound
    
  barLengthFound: ; if this tag is hit, it is known that barLengthDisplay has the correct display value
    movff barLengthDisplay, display4
    return
    
  ;;;;;;;;;; Paused, therefore increase speed
  increaseSpeed:
    incf speed
    movff speed, temp
    movlw 10
    subwf temp
    btfsc STATUS, 2
    goto speed9 ; means incremented speed is 10, set it back to nine and set correct display configs
    movlw 0x18
    addwf timer0H ; safe to increase timer0H, no overflow guaranteed within the range
    movf timer0L, W     ; Load TMR0L into WREG
    addlw 0x6A         ; Add the predefined value to WREG
    btfsc STATUS, 0   ; Check if there was a carry from the addition
    incf timer0H     ; If so, increment TMR0H
    movwf timer0L     ; Move the result back into TMR0L
    ;;;;;;;;;; FIND THE NEW SPEED
    btfsc speed, 3  
    goto speed89
    btfsc speed, 2
    goto speed4567
    btfsc speed, 1
    goto speed23
    goto speed1
  speed89:  ; speed 8 or 9
    btfsc speed, 0
    goto speed9
    goto speed8
  speed8: ; speed 8
    movff eightLed, speedDisplay
    goto speedFound
  speed9: ; speed 9
    movlw 9
    movwf speed
    movff nineLed, speedDisplay
    goto speedFound
  speed4567: ; speed 4,5,6 or 7
    btfsc speed, 1
    goto speed67
    goto speed45
  speed45: ; speed 4 or 5
    btfsc speed, 0
    goto speed5
    goto speed4
  speed67: ; speed 6 or 7
    btfsc speed, 0
    goto speed7
    goto speed6
  speed23: ; speed 2 or 3
    btfsc speed, 0
    goto speed3
    goto speed2
  speed7: ; speed 7
    movff sevenLed, speedDisplay
    goto speedFound
  speed6: ; speed 6
    movff sixLed, speedDisplay
    goto speedFound
  speed5: ; speed 5
    movff fiveLed, speedDisplay
    goto speedFound
  speed4: ; speed 4
    movff fourLed, speedDisplay
    goto speedFound
  speed3: ; speed 3
    movff threeLed, speedDisplay
    goto speedFound
  speed2: ; speed 2
    movff twoLed, speedDisplay
    goto speedFound
  speed1: ; speed 1
    movff oneLed, speedDisplay
    goto speedFound
    
 
  speedFound: ; if this tag is hit, it is known that speedDisplay has the correct display value
    movff speedDisplay, display4
    return
;;;;;;;;;; Increase button handler, PORTB[5], bar or speed depending on paused or resumed       
decreaseBarSpeed:
    btfsc paused, 0
    goto decreaseSpeed
    ;DECREASE BAR LENGTH
    movff barLength, temp
    movlw 2
    subwf temp
    BTFSC STATUS, 2
    goto barLength2d ; if bar length was 2, set it to 2 again
    decf barLength ; else barLength can be decreased
    ;;;;;;;;;; FIND THE NEW BAR LENGTH
    btfsc barLength, 3 ; identify the new barLength, set it and the display accordingly
    goto barLength8d
    btfsc barLength, 2
    goto barLength4567d
    btfsc barLength, 1
    goto barLength23d
    goto barLength1d
  barLength8d: ; barLength is 8
    movff eightLed, barLengthDisplay
    goto barLengthFoundd
  barLength4567d: ; barLength is 4,5,6 or 7
    btfsc barLength, 1
    goto barLength67d
    goto barLength45d
  barLength45d: ; barLength is 4 or 5
    btfsc barLength, 0
    goto barLength5d
    goto barLength4d
  barLength67d: ; barLength is 6 or 7
    btfsc barLength, 0
    goto barLength7d
    goto barLength6d
  barLength23d: ; barLength is 2 or 3
    btfsc barLength, 0
    goto barLength3d
    goto barLength2d
  barLength7d: ; barLength is 7
    btfss currentBar, 2
    goto not7
    btfss currentBar, 1
    goto not7
    btfss currentBar, 0
    goto not7
    decf currentBar ; if current bar is 1 more than  last bar after decrement,
    ; decrement currentBar to make it last bar, starts from the beginning after refreshDisplay
   not7: ; currentBar is not7
    movff sevenLed, barLengthDisplay
    goto barLengthFoundd
  barLength6d: ; barLength is 6 
    btfss currentBar, 2
    goto not6
    btfss currentBar, 1
    goto not6
    btfsc currentBar, 0
    goto not6
    decf currentBar ; if current bar is 1 more than  last bar after decrement,
    ; decrement currentBar to make it last bar, starts from the beginning after refreshDisplay
   not6: ; currentBar is not6
    movff sixLed, barLengthDisplay
    goto barLengthFoundd
  barLength5d: ; barLength is 5
    btfss currentBar, 2
    goto not5
    btfsc currentBar, 1
    goto not5
    btfss currentBar, 0
    goto not5
    decf currentBar ; if current bar is 1 more than  last bar after decrement,
    ; decrement currentBar to make it last bar, starts from the beginning after refreshDisplay
   not5: ; currentBar is not5
    movff fiveLed, barLengthDisplay
    goto barLengthFoundd
  barLength4d: ; barLength is 4
    btfss currentBar, 2
    goto not4
    btfsc currentBar, 1
    goto not4
    btfsc currentBar, 0
    goto not4
    decf currentBar ; if current bar is 1 more than  last bar after decrement,
    ; decrement currentBar to make it last bar, starts from the beginning after refreshDisplay
   not4: ; currentBar is not4
    movff fourLed, barLengthDisplay
    goto barLengthFoundd
  barLength3d: ; barLength is 3
    btfss currentBar, 1
    goto not3
    btfss currentBar, 0
    goto not3
    decf currentBar ; if current bar is 1 more than  last bar after decrement,
    ; decrement currentBar to make it last bar, starts from the beginning after refreshDisplay
   not3: ; currentBar is not3
    movff threeLed, barLengthDisplay
    goto barLengthFoundd
  barLength2d: ; barLength is 2
    btfss currentBar, 1
    goto not2
    btfsc currentBar, 0
    goto not2
    decf currentBar ; if current bar is 1 more than  last bar after decrement,
    ; decrement currentBar to make it last bar, starts from the beginning after refreshDisplay
   not2: ; currentBar is not2
    movlw 2
    movwf barLength
    movff twoLed, barLengthDisplay
    goto barLengthFoundd
  barLength1d: ; barLength is 1
    btfss currentBar, 0
    goto not1
    decf currentBar ; if current bar is 1 more than  last bar after decrement,
    ; decrement currentBar to make it last bar, starts from the beginning after refreshDisplay
   not1: ; currentBar is not1
    movff oneLed, barLengthDisplay
    goto barLengthFoundd
    
  barLengthFoundd: ; reload display 4 accoringly
    movff barLengthDisplay, display4
    return
     
  ;;;;;;;;;; decrease was requested when paused, decrease Speed   
  decreaseSpeed: 
    decf speed
    movff speed, temp
    movlw 0
    subwf temp
    BTFSC STATUS, 2
    goto speed1d ; If decreased speed is 0, set it and the displays to 1
    movlw 0x18 ; here, subtract preknown values for 100ms of change with overflow handling
    subwf timer0H
    MOVFF timer0L, temp     ; Load TMR0L into temp
    movlW 0x6A         
    subwf temp
    BTFSS STATUS, 0   ; Check if there was a carry from the addition
    decf timer0H     ; If so, decrement TMR0H
    MOVff temp, timer0L     ; Move the result back into TMR0L
    ;;;;;;;;;; FIND THE NEW SPEED
    btfsc speed, 3 ; determine the new speed value to set it and the displays
    goto speed89d
    btfsc speed, 2
    goto speed4567d
    btfsc speed, 1
    goto speed23d
    goto speed1d
  speed89d: ; speed is 8 or 9
    btfsc speed, 0
    goto speed9d
    goto speed8d
  speed8d: ; speed is 8 
    movff eightLed, speedDisplay
    goto speedFoundd
  speed9d: ; speed is 9
    movff nineLed, speedDisplay
    goto speedFoundd
  speed4567d: ; speed is 4,5,6 or 7
    btfsc speed, 1
    goto speed67d
    goto speed45d
  speed45d: ; speed is 4 or 5
    btfsc speed, 0
    goto speed5d
    goto speed4d
  speed67d: ; speed is 6 or 7
    btfsc speed, 0
    goto speed7d
    goto speed6d
  speed23d: ; speed is 2 or 3
    btfsc speed, 0
    goto speed3d
    goto speed2d
  speed7d: ; speed is 7
    movff sevenLed, speedDisplay
    goto speedFoundd
  speed6d: ; speed is 6
    movff sixLed, speedDisplay
    goto speedFoundd
  speed5d: ; speed is 5
    movff fiveLed, speedDisplay
    goto speedFoundd
  speed4d: ; speed is 4
    movff fourLed, speedDisplay
    goto speedFoundd
  speed3d: ; speed is 3
    movff threeLed, speedDisplay
    goto speedFoundd
  speed2d: ; speed is 2
    movff twoLed, speedDisplay
    goto speedFoundd
  speed1d: ; speed is 1
    movlw 1
    movwf speed
    movff oneLed, speedDisplay
    goto speedFoundd
    
  speedFoundd: ; known that speedDisplay has the correct new display config for speed
    movff speedDisplay, display4 ; set the display 4 accordingly so that in next refresh it takes effect
    return    
;;;;;;;;;; TIMER0 Interrupt Handler
timer0_interrupt: ;Enable timer1, clear timer0 flag, always light up RC[0]. RC[1] if beginning of the bar.
    btfsc paused, 0
    goto done ; timer0 is disabled when metronome is paused, but in case somehow interrupt comes, just do deafult configurations
    incf currentBar ; Increment bar
    movff currentBar, temp
    movf barLength, W
    subwf temp
    BTFSC STATUS, 2 ; By my implementation, current Bar starts from 0.
    goto begins ; Therefore implement overflow if bLength and currentBar are equal.
    ;;;;;;;;;; Not beginning of a bar, operate as usual
    movlw 00000001B ; light only RC0
    movwf LATC 
    btfsc currentBar, 3 ; Find currentBar to set it and led config accordingly
    goto bar8
    btfsc currentBar, 2
    goto bar4567
    btfsc currentBar, 1
    goto bar23
    goto bar1
  bar8: ; new bar is 8
    movff eightLed, currentBarDisplay
    goto barFound
  bar4567: ; new bar is 4,5,6 or 7
    btfsc currentBar, 1
    goto bar67
    goto bar45
  bar45: ; new bar is 4 or 5
    btfsc currentBar, 0
    goto bar5
    goto bar4
  bar67: ; new bar is 6 or 7
    btfsc currentBar, 0
    goto bar7
    goto bar6
  bar23: ; new bar is 2 or 3
    btfsc currentBar, 0
    goto bar3
    goto bar2
  bar7: ; new bar is 7
    movff eightLed, currentBarDisplay
    goto barFound
  bar6: ; new bar is 6
    movff sevenLed, currentBarDisplay
    goto barFound
  bar5: ; new bar is 5
    movff sixLed, currentBarDisplay
    goto barFound
  bar4: ; new bar is 4
    movff fiveLed, currentBarDisplay
    goto barFound
  bar3: ; new bar is 3
    movff fourLed, currentBarDisplay
    goto barFound
  bar2: ; new bar is 2
    movff threeLed, currentBarDisplay
    goto barFound
  bar1: ; new bar is 1
    movff twoLed, currentBarDisplay
    goto barFound
  barFound: ; currentBarDisplay has the correct value
    movff currentBarDisplay, display2 ; set the display accordingly
    goto done 
begins: 
    movlw 00000011B ; first  beat of the bar, light up both RC 0 and 1
    movwf LATC
    movff oneLed, currentBarDisplay
    movff currentBarDisplay, display2
    clrf currentBar
done: ; configure timers, it is known that tmr1 was disabled, enable it now.
    call configure_timer0
    call configure_timer1    
    bsf     PIE1, 0    ; Enable TMR1 interrupt
    return
;;;;;;;;;; TIMER1 Interrupt Handler  
timer1_interrupt:
    bcf     PIE1, 0    ; Disable TMR1 interrupt
    btfsc paused, 0
    goto timer1_done
    clrf LATC ; just in case if timer works when paused, turn of RC lights
  timer1_done:
    bcf PIR1, 0 ; Clear Timer1 Interrupt Flag
    return

    
    
    
;;;;;METRONOME    
    
;;;;;;;;;;INITIALIZATION;;;;;;;;;;
main:
    call set_tris ; SET TRIS BITS
    call set_def_pars ; SET DEFAULT PARAMETERS
    call enable_interrupts ; ENABLE ALL INTERRUPTS EXCEPT FOR TIMERS
    
    
;;;;;;;;;;REFRESH DISPLAY - MAIN LOOP;;;;;;;;;
main_loop:
    call refreshDisplay ; refresh timer with display configs stored in variables,
    goto main_loop ; loop it

;;;;;

;;;;;



;;;;;;;;;; GLOBAL FUNCTIONS ;;;;;;;;;;
set_def_pars: ; parameters are all explained at the allocation part
    movlw 0x85 ; default timer0 values for 500 ms with prescaler 011
    movwf timer0H
    movwf defT0H
    movlw 0xEE 
    movwf timer0L
    movwf defT0L
    clrf var2
    clrf portBTemp ; PORTB bits are initially 0 at the beginning so the PORTBTemp
    movlw 2 
    movwf var3
    movlw 5 
    movwf var1
    movlw 1 
    movwf paused
    movlw 6
    movwf speed 
    movlw 4
    movwf barLength 
    movlw 0
    movwf currentBar ; 0 as default by my implementation
    movlw 00000110B ; 1
    movwf oneLed
    movwf currentBarDisplay
    movlw 01011011B ; 2
    movwf twoLed
    movlw 01001111B ; 3
    movwf threeLed
    movlw 01100110B ; 4
    movwf fourLed
    movwf barLengthDisplay ; 4 as deafult
    movlw 01101101B ; 5
    movwf fiveLed
    movlw 01111101B ; 6
    movwf sixLed
    movlw 00000111B ; 7
    movwf sevenLed
    movlw 01111111B ; 8
    movwf eightLed
    movlw 01101111B ; 9
    movwf nineLed
    movlw 01110011B ; P
    movwf displayP
    movlw 01000000B ; -
    movwf displayHyphen
    movlw 00000000B ; 
    movwf displayNone
    ;;;;;;;;;; DEFAULT DISPLAYS ARE ARRANGED BY PAUSED STATE ;;;;;;;;;;
    movlw 01111101B ; speedDisplay  6 as default
    movwf speedDisplay
    movff displayP, display1
    movff displayNone, display2
    movff displayNone, display3
    movff speedDisplay, display4
    return
;;;;;;;;;; SET TRIS BITS TO CONFIGURE BITS CORRECTLY AS INPUTS OR OUTPUTS
set_tris:
    ;OUTPUT: RA0-RA3: display selectors
    BCF TRISA, 0
    BCF TRISA, 1
    BCF TRISA, 2
    BCF TRISA, 3
    
    ;OUTPUT: RC0-RC1: bar and note indicator leds
    BCF TRISC, 0
    BCF TRISC, 1
    
    ;INPUT RB4-7: Interrupts
    BSF TRISB, 4
    BSF TRISB, 5
    BSF TRISB, 6
    BSF TRISB, 7
    
    ;OUTPUT: RD0-6:7 segment led bits
    BCF TRISD, 0
    BCF TRISD, 1
    BCF TRISD, 2
    BCF TRISD, 3
    BCF TRISD, 4
    BCF TRISD, 5
    BCF TRISD, 6
    return
;;;;;;;;;; WITH WAIT, REFRESH DISPLAY operates in timewise constant manner.
;;;;; There are no control flow. STABLE.
;;;;; therefore, operates with very small, if noticable flickers.
refreshDisplay:
    ;;;;; DISPLAY 1,2,3,4 variables keep the current and correct configurations.
    clrf PORTA ; switch to display1
    bsf PORTA, 0
    movf display1, W
    movwf LATD
    
    clrf var2 ; set variables to wait few ms between displays
    movlw 2
    movwf var3
    movlw 4 
    movwf var1
    
    call wait ; wait
    
    clrf PORTA ; switch to display2
    bsf PORTA, 1 
    movf display2, W
    movwf LATD
    
    clrf var2
    movlw 2
    movwf var3
    movlw 4 
    movwf var1
    
    call wait
    
    clrf PORTA ; switch to display3
    bsf PORTA, 2
    movf display3, W
    movwf LATD
    
    clrf var2 ; set variables to wait few ms between displays
    movlw 2 
    movwf var3
    movlw 4 
    movwf var1
    
    call wait ; wait
    
    clrf PORTA ; switch to display4
    bsf PORTA, 3
    movf display4, W
    movwf LATD
    
    clrf var2 ; set variables to wait few ms between displays
    movlw 2 
    movwf var3
    movlw 4 
    movwf var1
    
    call wait ; wait
    
    
    return
;;;;;;;;;; CONFIGURE TIMER0 BEFORE ENABLING IT
configure_timer0:
    movlw   10000011B ; prescaler is set with 011
    movwf   T0CON        ; Write the control register
    ;timer0H, timer0L keeps the last used timer0 values, so that when
    ;timer0 interrupt comes, it is initialized with the same values.
    movff timer0H, TMR0H 
    movff timer0L, TMR0L
    bcf INTCON, 2 ; Clear TMR0 interrupt flag to make it ready for interrupts, only enable is left for TMR0 to operate
    ; After calling this function, timer0 can be enabled if needed.
    return
;;;;;;;;;; CONFIGURE TIMER1, FIXED PROCEDURE   
configure_timer1:
    MOVLW 0x5F ; Timer 1 50 ms values, they are fixed as 50 ms duration is also fixed.
    MOVWF TMR1L
    MOVLW 0x9E
    MOVWF TMR1H
    BSF T1CON, 0 ; Internal Clock source
    BSF T1CON, 4 ; Prescale set
    BCF T1CON, 5 ; oscillator disabled
    bcf PIR1, 0 ; Clear Timer1 Interrupt Flag to make it ready for interrupts, only enable is left for TMR1 to operate
    return
;;;;;;;;;; ENABLE_Interrupts except timers.
enable_interrupts:
    bcf	    RCON, 7 ; Interrupt priority enable
    bsf     INTCON, 3 ; Enable PORTB[4-7] interrupt
    bsf	    INTCON, 6 ; Peripheral interrupt enable
    bsf     INTCON, 7  ; Enable global interrupts
    bcf     INTCON, 0 ; clr PORTB[4-7] interrupt flag
    return
;;;;;;;;;; wait function, used for a wait around 3ms between display switches       
wait:  
    decfsz var3
    goto loop1f
    return
    loop1f:
	decfsz var1
	goto loop2f
	goto wait
	loop2f:
	    incfsz var2
	    goto loop2f
	    goto loop1f
;;;;;;;;;;