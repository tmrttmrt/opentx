EESchema Schematic File Version 4
LIBS:esp32-wroom-32-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "OpenTX board based on ESP32 WROOM module"
Date "2019-01-16"
Rev "0"
Comp "Copyright F. Aguerre & T. Mertelj"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L esp32-wroom-32-rescue:SW_PUSH-MEGA2560-esp-wroom-32-rescue .3
U 1 1 5499AF9B
P 6350 2900
F 0 ".3" H 6450 2900 50  0001 C CNN
F 1 "Push" H 6350 2900 50  0000 C CNN
F 2 "" H 6350 2900 60  0001 C CNN
F 3 "" H 6350 2900 60  0001 C CNN
	1    6350 2900
	-1   0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR017
U 1 1 5499AE76
P 5950 2750
F 0 "#PWR017" H 5950 2750 30  0001 C CNN
F 1 "GND" H 5950 2680 30  0001 C CNN
F 2 "" H 5950 2750 60  0001 C CNN
F 3 "" H 5950 2750 60  0001 C CNN
	1    5950 2750
	1    0    0    -1  
$EndComp
Text Notes 7950 3950 0    42   ~ 0
JACK
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR019
U 1 1 5499ABF7
P 7750 4300
F 0 "#PWR019" H 7750 4300 30  0001 C CNN
F 1 "GND" H 7750 4230 30  0001 C CNN
F 2 "" H 7750 4300 60  0001 C CNN
F 3 "" H 7750 4300 60  0001 C CNN
	1    7750 4300
	1    0    0    -1  
$EndComp
Text Label 7200 4500 0    42   ~ 0
DSC_PPM
$Comp
L esp32-wroom-32-rescue:C-Device-esp-wroom-32-rescue C1
U 1 1 5499AA3F
P 10600 5800
F 0 "C1" V 10550 5650 50  0000 L CNN
F 1 "100n" V 10650 5650 50  0000 L CNN
F 2 "" H 10600 5800 60  0001 C CNN
F 3 "" H 10600 5800 60  0001 C CNN
	1    10600 5800
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR04
U 1 1 549993C2
P 2500 2600
F 0 "#PWR04" H 2500 2600 30  0001 C CNN
F 1 "GND" H 2500 2530 30  0001 C CNN
F 2 "" H 2500 2600 60  0001 C CNN
F 3 "" H 2500 2600 60  0001 C CNN
	1    2500 2600
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:C-Device-esp-wroom-32-rescue C1
U 1 1 54998B4F
P 8350 2350
F 0 "C1" V 8300 2200 50  0000 L CNN
F 1 "100n" V 8400 2200 50  0000 L CNN
F 2 "" H 8350 2350 60  0001 C CNN
F 3 "" H 8350 2350 60  0001 C CNN
	1    8350 2350
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:C-Device-esp-wroom-32-rescue C1
U 1 1 54998B49
P 8100 2350
F 0 "C1" V 8050 2200 50  0000 L CNN
F 1 "100n" V 8150 2200 50  0000 L CNN
F 2 "" H 8100 2350 60  0001 C CNN
F 3 "" H 8100 2350 60  0001 C CNN
	1    8100 2350
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR032
U 1 1 54998B1E
P 10750 3050
F 0 "#PWR032" H 10750 3050 30  0001 C CNN
F 1 "GND" H 10750 2980 30  0001 C CNN
F 2 "" H 10750 3050 60  0001 C CNN
F 3 "" H 10750 3050 60  0001 C CNN
	1    10750 3050
	1    0    0    -1  
$EndComp
Text Notes 8400 3300 0    60   ~ 0
Power management
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR020
U 1 1 5499853A
P 7750 5600
F 0 "#PWR020" H 7750 5600 30  0001 C CNN
F 1 "GND" H 7750 5530 30  0001 C CNN
F 2 "" H 7750 5600 60  0001 C CNN
F 3 "" H 7750 5600 60  0001 C CNN
	1    7750 5600
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR023
U 1 1 549984D9
P 8750 6150
F 0 "#PWR023" H 8750 6150 30  0001 C CNN
F 1 "GND" H 8750 6080 30  0001 C CNN
F 2 "" H 8750 6150 60  0001 C CNN
F 3 "" H 8750 6150 60  0001 C CNN
	1    8750 6150
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R7
U 1 1 54997DEC
P 8750 5800
F 0 "R7" V 8830 5800 50  0000 C CNN
F 1 "100K" V 8750 5800 50  0000 C CNN
F 2 "" H 8750 5800 60  0001 C CNN
F 3 "" H 8750 5800 60  0001 C CNN
	1    8750 5800
	-1   0    0    1   
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R5
U 1 1 54997CFE
P 7750 5250
F 0 "R5" V 7830 5250 50  0000 C CNN
F 1 "6.8K" V 7750 5250 50  0000 C CNN
F 2 "" H 7750 5250 60  0001 C CNN
F 3 "" H 7750 5250 60  0001 C CNN
	1    7750 5250
	-1   0    0    1   
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R4
U 1 1 54997CE8
P 8000 5000
F 0 "R4" V 8080 5000 50  0000 C CNN
F 1 "10K" V 8000 5000 50  0000 C CNN
F 2 "" H 8000 5000 60  0001 C CNN
F 3 "" H 8000 5000 60  0001 C CNN
	1    8000 5000
	0    -1   -1   0   
$EndComp
Text Label 7200 5000 0    42   ~ 0
DSC_Activated
$Comp
L esp32-wroom-32-rescue:SPST-MEGA2560-esp-wroom-32-rescue SW2
U 1 1 5499444A
P 8250 4350
F 0 "SW2" H 8250 4450 50  0000 C CNN
F 1 "Jack plugged" H 8250 4250 50  0000 C CNN
F 2 "" H 8250 4350 60  0001 C CNN
F 3 "" H 8250 4350 60  0001 C CNN
	1    8250 4350
	0    -1   -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:DIODESCH-MEGA2560-esp-wroom-32-rescue D1
U 1 1 549943E4
P 8250 5750
F 0 "D1" H 8250 5850 40  0000 C CNN
F 1 "MBR0520" H 8250 5650 40  0000 C CNN
F 2 "" H 8250 5750 60  0001 C CNN
F 3 "" H 8250 5750 60  0001 C CNN
	1    8250 5750
	0    -1   -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:DIODESCH-MEGA2560-esp-wroom-32-rescue D1
U 1 1 549943CB
P 8250 5350
F 0 "D1" H 8250 5450 40  0000 C CNN
F 1 "MBR0520" H 8250 5250 40  0000 C CNN
F 2 "" H 8250 5350 60  0001 C CNN
F 3 "" H 8250 5350 60  0001 C CNN
	1    8250 5350
	0    -1   1    0   
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R4
U 1 1 549943BB
P 8500 5550
F 0 "R4" V 8580 5550 50  0000 C CNN
F 1 "5.1K" V 8500 5550 50  0000 C CNN
F 2 "" H 8500 5550 60  0001 C CNN
F 3 "" H 8500 5550 60  0001 C CNN
	1    8500 5550
	0    -1   -1   0   
$EndComp
Text Label 11100 5500 2    42   ~ 0
RF_Activated
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR027
U 1 1 54994281
P 10350 6000
F 0 "#PWR027" H 10350 6000 30  0001 C CNN
F 1 "GND" H 10350 5930 30  0001 C CNN
F 2 "" H 10350 6000 60  0001 C CNN
F 3 "" H 10350 6000 60  0001 C CNN
	1    10350 6000
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R4
U 1 1 54994280
P 10350 5250
F 0 "R4" V 10430 5250 50  0000 C CNN
F 1 "10K" V 10350 5250 50  0000 C CNN
F 2 "" H 10350 5250 60  0001 C CNN
F 3 "" H 10350 5250 60  0001 C CNN
	1    10350 5250
	-1   0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R5
U 1 1 5499427F
P 10350 5750
F 0 "R5" V 10430 5750 50  0000 C CNN
F 1 "6.8K" V 10350 5750 50  0000 C CNN
F 2 "" H 10350 5750 60  0001 C CNN
F 3 "" H 10350 5750 60  0001 C CNN
	1    10350 5750
	-1   0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:DIODESCH-MEGA2560-esp-wroom-32-rescue D1
U 1 1 549941C1
P 9900 3850
F 0 "D1" H 9900 3950 40  0000 C CNN
F 1 "MBR0520" H 9900 3750 40  0000 C CNN
F 2 "" H 9900 3850 60  0001 C CNN
F 3 "" H 9900 3850 60  0001 C CNN
	1    9900 3850
	1    0    0    1   
$EndComp
$Comp
L esp32-wroom-32-rescue:SPST-MEGA2560-esp-wroom-32-rescue SW1
U 1 1 54994193
P 9050 3850
F 0 "SW1" H 9050 3950 50  0000 C CNN
F 1 "Power switch" H 9050 3750 50  0000 C CNN
F 2 "~" H 9050 3850 50  0001 C CNN
F 3 "" H 9050 3850 60  0001 C CNN
	1    9050 3850
	1    0    0    -1  
$EndComp
Text Label 6450 1050 2    42   ~ 0
U_batt
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R3
U 1 1 54994034
P 6050 1300
F 0 "R3" V 6130 1300 50  0000 C CNN
F 1 "2.7K" V 6050 1300 50  0000 C CNN
F 2 "" H 6050 1300 60  0001 C CNN
F 3 "" H 6050 1300 60  0001 C CNN
	1    6050 1300
	-1   0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R4
U 1 1 54994033
P 5800 1050
F 0 "R4" V 5880 1050 50  0000 C CNN
F 1 "5.1K" V 5800 1050 50  0000 C CNN
F 2 "" H 5800 1050 60  0001 C CNN
F 3 "" H 5800 1050 60  0001 C CNN
	1    5800 1050
	0    1    -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR016
U 1 1 54994032
P 6050 1650
F 0 "#PWR016" H 6050 1650 30  0001 C CNN
F 1 "GND" H 6050 1580 30  0001 C CNN
F 2 "" H 6050 1650 60  0001 C CNN
F 3 "" H 6050 1650 60  0001 C CNN
	1    6050 1650
	1    0    0    -1  
$EndComp
Text Notes 5400 700  0    60   ~ 0
Battery voltage reading
Text Notes 7100 1900 0    60   ~ 0
Rotary encoders (x2)
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R1
U 1 1 54993DF7
P 2750 1650
F 0 "R1" V 2830 1650 50  0000 C CNN
F 1 "200" V 2750 1650 50  0000 C CNN
F 2 "" H 2750 1650 60  0001 C CNN
F 3 "" H 2750 1650 60  0001 C CNN
	1    2750 1650
	0    1    -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R1
U 1 1 54993D2B
P 7150 2750
F 0 "R1" V 7230 2750 50  0000 C CNN
F 1 "200" V 7150 2750 50  0000 C CNN
F 2 "" H 7150 2750 60  0001 C CNN
F 3 "" H 7150 2750 60  0001 C CNN
	1    7150 2750
	0    1    -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:C-Device-esp-wroom-32-rescue C1
U 1 1 54993C14
P 7850 2350
F 0 "C1" V 7800 2200 50  0000 L CNN
F 1 "100n" V 7900 2200 50  0000 L CNN
F 2 "" H 7850 2350 60  0001 C CNN
F 3 "" H 7850 2350 60  0001 C CNN
	1    7850 2350
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR022
U 1 1 54993C0D
P 8650 2200
F 0 "#PWR022" H 8650 2200 30  0001 C CNN
F 1 "GND" H 8650 2130 30  0001 C CNN
F 2 "" H 8650 2200 60  0001 C CNN
F 3 "" H 8650 2200 60  0001 C CNN
	1    8650 2200
	1    0    0    -1  
$EndComp
Text Label 9050 2900 2    42   ~ 0
Rot_x_Push
Text Label 9050 2750 2    42   ~ 0
Rot_x_B
Text Label 9050 2650 2    42   ~ 0
Rot_x_A
$Comp
L esp32-wroom-32-rescue:DIODESCH-MEGA2560-esp-wroom-32-rescue D1
U 1 1 5499378E
P 2600 900
F 0 "D1" H 2600 1000 40  0000 C CNN
F 1 "MBR0520" H 2600 800 40  0000 C CNN
F 2 "" H 2600 900 60  0001 C CNN
F 3 "" H 2600 900 60  0001 C CNN
	1    2600 900 
	1    0    0    -1  
$EndComp
Text Notes 1600 750  0    60   ~ 0
PPM management
Text Notes 9650 750  0    60   ~ 0
LCD backlight (5V)
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR025
U 1 1 5499359A
P 9600 1600
F 0 "#PWR025" H 9600 1600 30  0001 C CNN
F 1 "GND" H 9600 1530 30  0001 C CNN
F 2 "" H 9600 1600 60  0001 C CNN
F 3 "" H 9600 1600 60  0001 C CNN
	1    9600 1600
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:C-Device-esp-wroom-32-rescue C1
U 1 1 54993599
P 9600 1300
F 0 "C1" V 9350 1200 50  0000 L CNN
F 1 "100n" V 9450 1200 50  0000 L CNN
F 2 "" H 9600 1300 60  0001 C CNN
F 3 "" H 9600 1300 60  0001 C CNN
	1    9600 1300
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:C-Device-esp-wroom-32-rescue C1
U 1 1 54993563
P 10400 1300
F 0 "C1" V 10350 1150 50  0000 L CNN
F 1 "100n" V 10450 1150 50  0000 L CNN
F 2 "" H 10400 1300 60  0001 C CNN
F 3 "" H 10400 1300 60  0001 C CNN
	1    10400 1300
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR029
U 1 1 54993562
P 10400 1600
F 0 "#PWR029" H 10400 1600 30  0001 C CNN
F 1 "GND" H 10400 1530 30  0001 C CNN
F 2 "" H 10400 1600 60  0001 C CNN
F 3 "" H 10400 1600 60  0001 C CNN
	1    10400 1600
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR026
U 1 1 549934C9
P 10000 1600
F 0 "#PWR026" H 10000 1600 30  0001 C CNN
F 1 "GND" H 10000 1530 30  0001 C CNN
F 2 "" H 10000 1600 60  0001 C CNN
F 3 "" H 10000 1600 60  0001 C CNN
	1    10000 1600
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:LED-Device-esp-wroom-32-rescue .5
U 1 1 549933C8
P 10750 1450
F 0 ".5" H 10800 1350 50  0000 C CNN
F 1 "LED" H 10750 1350 50  0000 C CNN
F 2 "" H 10750 1450 60  0001 C CNN
F 3 "" H 10750 1450 60  0001 C CNN
	1    10750 1450
	0    -1   -1   0   
$EndComp
Text Notes 6850 750  0    60   ~ 0
Switches, trims & keybs deboucing (x22)
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR021
U 1 1 54992E9C
P 8100 1500
F 0 "#PWR021" H 8100 1500 30  0001 C CNN
F 1 "GND" H 8100 1430 30  0001 C CNN
F 2 "" H 8100 1500 60  0001 C CNN
F 3 "" H 8100 1500 60  0001 C CNN
	1    8100 1500
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:C-Device-esp-wroom-32-rescue C1
U 1 1 54992E49
P 8100 1200
F 0 "C1" V 7850 1100 50  0000 L CNN
F 1 "100n" V 7950 1100 50  0000 L CNN
F 2 "" H 8100 1200 60  0001 C CNN
F 3 "" H 8100 1200 60  0001 C CNN
	1    8100 1200
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R1
U 1 1 54992E3A
P 7850 1000
F 0 "R1" V 7930 1000 50  0000 C CNN
F 1 "200" V 7850 1000 50  0000 C CNN
F 2 "" H 7850 1000 60  0001 C CNN
F 3 "" H 7850 1000 60  0001 C CNN
	1    7850 1000
	0    -1   -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR018
U 1 1 54992E1E
P 6900 1500
F 0 "#PWR018" H 6900 1500 30  0001 C CNN
F 1 "GND" H 6900 1430 30  0001 C CNN
F 2 "" H 6900 1500 60  0001 C CNN
F 3 "" H 6900 1500 60  0001 C CNN
	1    6900 1500
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:SW_PUSH-MEGA2560-esp-wroom-32-rescue SW_xx1
U 1 1 54992E18
P 7300 1000
F 0 "SW_xx1" H 7300 1150 50  0000 C CNN
F 1 "." H 7300 1100 50  0001 C CNN
F 2 "" H 7300 1000 60  0001 C CNN
F 3 "" H 7300 1000 60  0001 C CNN
	1    7300 1000
	-1   0    0    -1  
$EndComp
Text Label 8650 1000 2    42   ~ 0
MCP23017_Input
$Comp
L esp32-wroom-32-rescue:VCC-power-esp-wroom-32-rescue 105
U 1 1 5498A277
P 9450 950
F 0 "105" H 9450 1050 30  0001 C CNN
F 1 "VCC" H 9450 1100 50  0000 C CNN
F 2 "" H 9450 950 60  0001 C CNN
F 3 "" H 9450 950 60  0001 C CNN
	1    9450 950 
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:7805-MEGA2560-esp-wroom-32-rescue U3
U 1 1 5498A254
P 10000 1150
F 0 "U3" H 10150 954 50  0000 C CNN
F 1 "MCP1826 5V" H 10000 1350 50  0000 C CNN
F 2 "" H 10000 1150 60  0001 C CNN
F 3 "" H 10000 1150 60  0001 C CNN
	1    10000 1150
	1    0    0    -1  
$EndComp
Text Label 2950 2550 0    42   ~ 0
LCD_Light
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue .6
U 1 1 5498A214
P 10750 1900
F 0 ".6" V 10750 1900 50  0000 C CNN
F 1 " R" V 10750 1900 50  0000 C CNN
F 2 "" H 10750 1900 60  0001 C CNN
F 3 "" H 10750 1900 60  0001 C CNN
	1    10750 1900
	-1   0    0    1   
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R8
U 1 1 5498A213
P 3600 2550
F 0 "R8" V 3680 2550 50  0000 C CNN
F 1 "100" V 3600 2550 50  0000 C CNN
F 2 "" H 3600 2550 60  0001 C CNN
F 3 "" H 3600 2550 60  0001 C CNN
	1    3600 2550
	0    -1   -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR02
U 1 1 54989B15
P 1100 5300
F 0 "#PWR02" H 1100 5300 50  0001 C CNN
F 1 "GND" H 1100 5230 50  0001 C CNN
F 2 "" H 1100 5300 60  0001 C CNN
F 3 "" H 1100 5300 60  0001 C CNN
	1    1100 5300
	0    1    1    0   
$EndComp
$Comp
L esp32-wroom-32-rescue:4066-MEGA2560-esp-wroom-32-rescue U1
U 1 1 549891BA
P 1600 2500
F 0 "U1" H 1800 2351 40  0000 C CNN
F 1 "4066" H 1800 2650 40  0000 C CNN
F 2 "" H 1600 2500 60  0001 C CNN
F 3 "" H 1600 2500 60  0001 C CNN
	1    1600 2500
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:VCC-power-esp-wroom-32-rescue 102
U 1 1 549891B9
P 1600 2250
F 0 "102" H 1600 2350 30  0001 C CNN
F 1 "VCC" H 1600 2350 50  0000 C CNN
F 2 "" H 1600 2250 60  0001 C CNN
F 3 "" H 1600 2250 60  0001 C CNN
	1    1600 2250
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR03
U 1 1 549891B8
P 1600 2750
F 0 "#PWR03" H 1600 2750 30  0001 C CNN
F 1 "GND" H 1600 2680 30  0001 C CNN
F 2 "" H 1600 2750 60  0001 C CNN
F 3 "" H 1600 2750 60  0001 C CNN
	1    1600 2750
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:4066-MEGA2560-esp-wroom-32-rescue U1
U 2 1 549891B7
P 2050 1250
F 0 "U1" H 2250 1101 40  0000 C CNN
F 1 "4066" H 2250 1400 40  0000 C CNN
F 2 "" H 2050 1250 60  0001 C CNN
F 3 "" H 2050 1250 60  0001 C CNN
	2    2050 1250
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:4066-MEGA2560-esp-wroom-32-rescue U1
U 4 1 549891B6
P 1450 1650
F 0 "U1" H 1650 1501 40  0000 C CNN
F 1 "4066" H 1650 1800 40  0000 C CNN
F 2 "" H 1450 1650 60  0001 C CNN
F 3 "" H 1450 1650 60  0001 C CNN
	4    1450 1650
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:4066-MEGA2560-esp-wroom-32-rescue U1
U 3 1 549891B5
P 2200 2050
F 0 "U1" H 2400 1901 40  0000 C CNN
F 1 "4066" H 2400 2200 40  0000 C CNN
F 2 "" H 2200 2050 60  0001 C CNN
F 3 "" H 2200 2050 60  0001 C CNN
	3    2200 2050
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R6
U 1 1 549891B4
P 2000 1650
F 0 "R6" V 2080 1650 50  0000 C CNN
F 1 "10K" V 2000 1650 50  0000 C CNN
F 2 "" H 2000 1650 60  0001 C CNN
F 3 "" H 2000 1650 60  0001 C CNN
	1    2000 1650
	0    1    -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:VCC-power-esp-wroom-32-rescue 103
U 1 1 549891B3
P 2350 1600
F 0 "103" H 2350 1700 30  0001 C CNN
F 1 "VCC" H 2350 1700 50  0000 C CNN
F 2 "" H 2350 1600 60  0001 C CNN
F 3 "" H 2350 1600 60  0001 C CNN
	1    2350 1600
	1    0    0    -1  
$EndComp
Text Label 600  2650 0    42   ~ 0
Sim_Control
$Comp
L esp32-wroom-32-rescue:VCC-power-esp-wroom-32-rescue 101
U 1 1 549891AD
P 1250 2250
F 0 "101" H 1250 2350 30  0001 C CNN
F 1 "VCC" H 1250 2350 50  0000 C CNN
F 2 "" H 1250 2250 60  0001 C CNN
F 3 "" H 1250 2250 60  0001 C CNN
	1    1250 2250
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R6
U 1 1 549891AC
P 2150 2500
F 0 "R6" V 2230 2500 50  0000 C CNN
F 1 "10K" V 2150 2500 50  0000 C CNN
F 2 "" H 2150 2500 60  0001 C CNN
F 3 "" H 2150 2500 60  0001 C CNN
	1    2150 2500
	0    -1   -1   0   
$EndComp
Text Label 600  2050 0    42   ~ 0
PPM_IN
Text Label 600  1250 0    42   ~ 0
PPM_OUT
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR01
U 1 1 549891A9
P 850 1800
F 0 "#PWR01" H 850 1800 30  0001 C CNN
F 1 "GND" H 850 1730 30  0001 C CNN
F 2 "" H 850 1800 60  0001 C CNN
F 3 "" H 850 1800 60  0001 C CNN
	1    850  1800
	1    0    0    -1  
$EndComp
Text Label 5850 4050 2    42   ~ 0
Key_►
Text Label 5850 3850 2    42   ~ 0
Key_▼
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R1
U 1 1 54988260
P 7600 2900
F 0 "R1" V 7680 2900 50  0000 C CNN
F 1 "200" V 7600 2900 50  0000 C CNN
F 2 "" H 7600 2900 60  0001 C CNN
F 3 "" H 7600 2900 60  0001 C CNN
	1    7600 2900
	0    1    -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R1
U 1 1 5498825E
P 7600 2650
F 0 "R1" V 7680 2650 50  0000 C CNN
F 1 "200" V 7600 2650 50  0000 C CNN
F 2 "" H 7600 2650 60  0001 C CNN
F 3 "" H 7600 2650 60  0001 C CNN
	1    7600 2650
	0    1    -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R4
U 1 1 549881F1
P 6750 2350
F 0 "R4" V 6830 2350 50  0000 C CNN
F 1 "5.1K" V 6750 2350 50  0000 C CNN
F 2 "" H 6750 2350 60  0001 C CNN
F 3 "" H 6750 2350 60  0001 C CNN
	1    6750 2350
	-1   0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R4
U 1 1 549881F0
P 6900 2350
F 0 "R4" V 6980 2350 50  0000 C CNN
F 1 "5.1K" V 6900 2350 50  0000 C CNN
F 2 "" H 6900 2350 60  0001 C CNN
F 3 "" H 6900 2350 60  0001 C CNN
	1    6900 2350
	-1   0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:VCC-power-esp-wroom-32-rescue 104
U 1 1 549881ED
P 6750 2050
F 0 "104" H 6750 2150 30  0001 C CNN
F 1 "VCC" H 6750 2200 50  0000 C CNN
F 2 "" H 6750 2050 60  0001 C CNN
F 3 "" H 6750 2050 60  0001 C CNN
	1    6750 2050
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR024
U 1 1 549881DD
P 9050 6150
F 0 "#PWR024" H 9050 6150 30  0001 C CNN
F 1 "GND" H 9050 6080 30  0001 C CNN
F 2 "" H 9050 6150 60  0001 C CNN
F 3 "" H 9050 6150 60  0001 C CNN
	1    9050 6150
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R4
U 1 1 549881DA
P 9050 4800
F 0 "R4" V 9130 4800 50  0000 C CNN
F 1 "5.1K" V 9050 4800 50  0000 C CNN
F 2 "" H 9050 4800 60  0001 C CNN
F 3 "" H 9050 4800 60  0001 C CNN
	1    9050 4800
	-1   0    0    1   
$EndComp
$Comp
L esp32-wroom-32-rescue:MOSFET_P-MEGA2560-esp-wroom-32-rescue Q2
U 1 1 549881D9
P 9050 4350
F 0 "Q2" V 9000 4600 60  0000 R CNN
F 1 "NDT2955" V 9250 4450 40  0000 R CNN
F 2 "" H 9050 4350 60  0001 C CNN
F 3 "" H 9050 4350 60  0001 C CNN
	1    9050 4350
	0    1    -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R7
U 1 1 549881D8
P 8800 4550
F 0 "R7" V 8880 4550 50  0000 C CNN
F 1 "100K" V 8800 4550 50  0000 C CNN
F 2 "" H 8800 4550 60  0001 C CNN
F 3 "" H 8800 4550 60  0001 C CNN
	1    8800 4550
	0    1    -1   0   
$EndComp
Text Label 7200 6050 0    42   ~ 0
Hold_Power
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR030
U 1 1 549881D0
P 10600 6000
F 0 "#PWR030" H 10600 6000 30  0001 C CNN
F 1 "GND" H 10600 5930 30  0001 C CNN
F 2 "" H 10600 6000 60  0001 C CNN
F 3 "" H 10600 6000 60  0001 C CNN
	1    10600 6000
	1    0    0    -1  
$EndComp
Text Label 3550 7600 1    42   ~ 0
SW_AilDR
Text Label 3250 7600 1    42   ~ 0
SW_ThCut
Text Label 3350 7600 1    42   ~ 0
SW_RudDR
Text Label 3150 7600 1    42   ~ 0
SW_Gear
Text Label 5850 3750 2    42   ~ 0
Key_ESC
Text Label 5850 3650 2    42   ~ 0
Key_MENU
Text Label 3450 7600 1    42   ~ 0
SW_EleDR
Text Label 5850 3950 2    42   ~ 0
Key_▲
Text Label 5850 4150 2    42   ~ 0
Key_◄
Text Label 700  5200 0    42   ~ 0
LCD_SI
Text Label 700  5000 0    42   ~ 0
LCD_A0
Text Label 2850 7600 1    42   ~ 0
SW_ID2
Text Label 2950 7600 1    42   ~ 0
SW_ID1
Text Label 2650 6100 1    42   ~ 0
LCD_Light
Text Label 700  5100 0    42   ~ 0
LCD_SCL
Text Label 5950 4850 2    42   ~ 0
Trim_LV-►
Text Label 5950 4750 2    42   ~ 0
Trim_LV◄-
Text Label 5950 5050 2    42   ~ 0
Trim_RV-►
Text Label 5950 4950 2    42   ~ 0
Trim_RV◄-
Text Label 5950 5150 2    42   ~ 0
Trim_RH◄-
Text Label 5950 5250 2    42   ~ 0
Trim_RH-►
Text Label 5950 4550 2    42   ~ 0
Trim_LH◄-
Text Label 5950 4650 2    42   ~ 0
Trim_LH-►
Text Label 700  4300 0    42   ~ 0
Stick_LV
Text Label 700  4200 0    42   ~ 0
Stick_RH
Text Label 700  4800 0    42   ~ 0
U_batt
Text Label 700  4600 0    42   ~ 0
Pot_1
Text Label 700  4700 0    42   ~ 0
Pot_2
Text Label 700  4400 0    42   ~ 0
Stick_RV
Text Label 700  4500 0    42   ~ 0
Stick_LH
Text Label 700  4900 0    42   ~ 0
AUDIO
Text Label 4450 7650 1    42   ~ 0
Rot_1_Push
Text Label 3550 4750 2    42   ~ 0
Telemetry_TX
Text Label 3550 4850 2    42   ~ 0
Telemetry_RX
Text Label 3750 7600 1    42   ~ 0
Hold_Power
Text Label 1850 6100 1    42   ~ 0
LCD_N_CS
Text Label 4350 7600 1    42   ~ 0
Rot_1_A
Text Label 4250 7600 1    42   ~ 0
Rot_1_B
Text Label 5900 4350 2    42   ~ 0
RF_Activated
Text Label 5900 4250 2    42   ~ 0
DSC_Activated
Text Label 3850 7700 1    42   ~ 0
Sim_Control
Text Label 2550 6100 1    42   ~ 0
PPM_OUT
$Comp
L esp32-wroom-32-rescue:CONN_1-MEGA2560-esp-wroom-32-rescue +1
U 1 1 549E784B
P 7450 3700
F 0 "+1" V 7500 3700 40  0000 L CNN
F 1 "CONN_1" H 7450 3755 30  0001 C CNN
F 2 "" H 7450 3700 60  0000 C CNN
F 3 "" H 7450 3700 60  0000 C CNN
	1    7450 3700
	0    1    -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:MOSFET_N-MEGA2560-esp-wroom-32-rescue Q1
U 1 1 549E7A7F
P 8950 5550
F 0 "Q1" H 9200 5600 50  0000 R CNN
F 1 "MMFT960" H 9000 5800 50  0000 R CNN
F 2 "~" H 8950 5550 60  0000 C CNN
F 3 "~" H 8950 5550 60  0000 C CNN
	1    8950 5550
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:CONN_1-MEGA2560-esp-wroom-32-rescue RF_Module1
U 1 1 549E7AB4
P 3000 900
F 0 "RF_Module1" H 3080 900 40  0000 L CNN
F 1 "CONN_1" H 3000 955 30  0001 C CNN
F 2 "" H 3000 900 60  0000 C CNN
F 3 "" H 3000 900 60  0000 C CNN
	1    3000 900 
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:VCC-power-esp-wroom-32-rescue 3.3V01
U 1 1 54989ABB
P 1050 4000
F 0 "3.3V01" H 1050 4100 60  0001 C CNN
F 1 "VCC" H 1050 4100 50  0000 C CNN
F 2 "" H 1050 4000 60  0001 C CNN
F 3 "" H 1050 4000 60  0001 C CNN
	1    1050 4000
	0    -1   -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:+BATT-power-esp-wroom-32-rescue U07
U 1 1 549E8260
P 5450 950
F 0 "U07" H 5450 900 60  0001 C CNN
F 1 "+BATT" H 5450 1050 50  0000 C BNN
F 2 "" H 5450 950 60  0001 C CNN
F 3 "" H 5450 950 60  0001 C CNN
	1    5450 950 
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:MOSFET_N-MEGA2560-esp-wroom-32-rescue Q1
U 1 1 549E84D3
P 10650 2450
F 0 "Q1" H 10650 2600 50  0000 R CNN
F 1 "MMFT960" H 11100 2500 50  0000 R CNN
F 2 "~" H 10650 2450 60  0000 C CNN
F 3 "~" H 10650 2450 60  0000 C CNN
	1    10650 2450
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:CONN_1-MEGA2560-esp-wroom-32-rescue -1
U 1 1 549E84E5
P 7250 3700
F 0 "-1" V 7300 3700 40  0000 L CNN
F 1 "CONN_1" H 7250 3755 30  0001 C CNN
F 2 "" H 7250 3700 60  0000 C CNN
F 3 "" H 7250 3700 60  0000 C CNN
	1    7250 3700
	0    1    -1   0   
$EndComp
Text Label 7050 3600 0    50   ~ 0
Battery (2S LiPo)
Text Label 3400 1650 2    42   ~ 0
DSC_PPM
$Comp
L esp32-wroom-32-rescue:SW-MEGA2560-esp-wroom-32-rescue ROT_ENC_x1
U 1 1 549E8A70
P 6350 2650
F 0 "ROT_ENC_x1" H 6350 2760 50  0000 C CNN
F 1 "A" H 6350 2650 50  0000 C CNN
F 2 "~" H 6350 2650 60  0000 C CNN
F 3 "~" H 6350 2650 60  0000 C CNN
	1    6350 2650
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:SW-MEGA2560-esp-wroom-32-rescue .2
U 1 1 549E8A96
P 6350 2750
F 0 ".2" H 6350 2860 50  0001 C CNN
F 1 "B" H 6350 2750 50  0000 C CNN
F 2 "~" H 6350 2750 60  0000 C CNN
F 3 "~" H 6350 2750 60  0000 C CNN
	1    6350 2750
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR031
U 1 1 549E8FF9
P 10450 3050
F 0 "#PWR031" H 10450 3050 30  0001 C CNN
F 1 "GND" H 10450 2980 30  0001 C CNN
F 2 "" H 10450 3050 60  0001 C CNN
F 3 "" H 10450 3050 60  0001 C CNN
	1    10450 3050
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R6
U 1 1 549E8FFF
P 10450 2700
F 0 "R6" V 10530 2700 50  0000 C CNN
F 1 "10K" V 10450 2700 50  0000 C CNN
F 2 "" H 10450 2700 60  0001 C CNN
F 3 "" H 10450 2700 60  0001 C CNN
	1    10450 2700
	-1   0    0    -1  
$EndComp
Text Label 9050 3000 2    42   ~ 0
or key ESC / MENU
Wire Wire Line
	7850 2650 9050 2650
Wire Wire Line
	8050 4200 7750 4200
Wire Wire Line
	7200 4500 8050 4500
Wire Notes Line
	7950 4750 8400 4750
Wire Notes Line
	7950 4750 7950 3950
Wire Wire Line
	7750 4200 7750 4300
Wire Notes Line
	8400 4750 8400 3950
Wire Wire Line
	600  1250 1500 1250
Wire Wire Line
	600  2050 1900 2050
Wire Wire Line
	600  2650 1050 2650
Wire Wire Line
	1900 2500 1900 2200
Wire Wire Line
	9600 1100 9450 1100
Wire Wire Line
	9700 3850 9550 3850
Wire Wire Line
	8850 4250 8550 4250
Wire Wire Line
	8250 5950 8250 6050
Wire Wire Line
	8650 2100 8650 2200
Wire Wire Line
	9450 1100 9450 950 
Wire Notes Line
	10900 1200 10900 2200
Wire Wire Line
	8100 1400 8100 1500
Wire Wire Line
	6900 1500 6900 1000
Wire Wire Line
	2500 2050 2500 1650
Wire Wire Line
	1500 900  1500 1250
Wire Wire Line
	3100 4750 3550 4750
Wire Wire Line
	5450 4350 5900 4350
Wire Wire Line
	3850 7250 3850 7700
Wire Wire Line
	3250 7250 3250 7600
Wire Wire Line
	6750 2050 6750 2100
Wire Wire Line
	6750 2100 6900 2100
Connection ~ 6750 2650
Wire Wire Line
	3350 7250 3350 7600
Wire Wire Line
	2500 1250 2350 1250
Connection ~ 1500 1250
Wire Wire Line
	1250 2250 1250 2500
Wire Wire Line
	1250 2500 1300 2500
Connection ~ 1050 2650
Wire Wire Line
	2350 1650 2350 1600
Wire Wire Line
	850  1800 850  1650
Wire Wire Line
	2550 5650 2550 6100
Wire Wire Line
	1850 5650 1850 6100
Wire Wire Line
	3100 4850 3550 4850
Wire Wire Line
	5450 4250 5900 4250
Wire Wire Line
	1050 2650 1050 1800
Wire Notes Line
	10900 1200 10600 1200
Wire Notes Line
	10900 2200 10600 2200
Wire Notes Line
	10600 2200 10600 1200
Connection ~ 8100 2750
Connection ~ 8550 4250
Connection ~ 10600 5500
Wire Wire Line
	9050 6150 9050 5750
Wire Wire Line
	8250 4850 8250 5000
Wire Wire Line
	10750 2650 10750 3050
Wire Wire Line
	6450 1050 6050 1050
Wire Wire Line
	1750 1400 1750 1650
Wire Wire Line
	1050 1800 1150 1800
Wire Wire Line
	850  1650 1150 1650
Wire Wire Line
	2500 2500 2500 2600
Wire Wire Line
	6900 1000 7000 1000
Wire Wire Line
	10000 1400 10000 1600
Wire Wire Line
	10750 1100 10400 1100
Wire Wire Line
	8250 6050 7200 6050
Wire Wire Line
	9250 4250 9550 4250
Wire Notes Line
	8400 3950 7950 3950
Wire Wire Line
	2800 900  2850 900 
Wire Wire Line
	5450 950  5450 1050
Wire Wire Line
	7450 3850 8250 3850
Connection ~ 8250 3850
Wire Wire Line
	1500 900  2400 900 
$Comp
L esp32-wroom-32-rescue:+BATT-power-esp-wroom-32-rescue U08
U 1 1 54DF6192
P 10500 3850
F 0 "U08" H 10500 3800 60  0001 C CNN
F 1 "+BATT" V 10450 3900 50  0000 C BNN
F 2 "" H 10500 3850 60  0001 C CNN
F 3 "" H 10500 3850 60  0001 C CNN
	1    10500 3850
	0    1    1    0   
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue GND01
U 1 1 54DF675E
P 7250 3950
F 0 "GND01" H 7250 3950 50  0001 C CNN
F 1 "GND" H 7250 3850 50  0000 C CNN
F 2 "" H 7250 3950 60  0001 C CNN
F 3 "" H 7250 3950 60  0001 C CNN
	1    7250 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 3850 7250 3950
$Comp
L esp32-wroom-32-rescue:LED-Device-esp-wroom-32-rescue .1
U 1 1 5568D052
P 4200 2550
F 0 ".1" H 4250 2450 50  0000 C CNN
F 1 "LED" H 4200 2450 50  0000 C CNN
F 2 "" H 4200 2550 60  0001 C CNN
F 3 "" H 4200 2550 60  0001 C CNN
	1    4200 2550
	-1   0    0    1   
$EndComp
Wire Notes Line
	3950 2400 3950 2700
Wire Notes Line
	4450 2400 4450 2700
Wire Notes Line
	3950 2400 4450 2400
Wire Notes Line
	4450 2700 3950 2700
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR09
U 1 1 5568D616
P 4600 2800
F 0 "#PWR09" H 4600 2800 30  0001 C CNN
F 1 "GND" H 4600 2730 30  0001 C CNN
F 2 "" H 4600 2800 60  0001 C CNN
F 3 "" H 4600 2800 60  0001 C CNN
	1    4600 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2550 4600 2800
Text Notes 3000 2300 0    60   ~ 0
LCD direct backlight (3.3V, 20 mA)
Text Label 9450 2450 0    42   ~ 0
LCD_Light
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R4
U 1 1 5568D86B
P 10100 2450
F 0 "R4" V 10180 2450 50  0000 C CNN
F 1 "5.1K" V 10100 2450 50  0000 C CNN
F 2 "" H 10100 2450 60  0001 C CNN
F 3 "" H 10100 2450 60  0001 C CNN
	1    10100 2450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6650 2650 6750 2650
Wire Wire Line
	6900 2750 6650 2750
Wire Wire Line
	7850 2100 8100 2100
Connection ~ 8100 2100
Connection ~ 8350 2100
Connection ~ 8350 2900
$Comp
L esp32-wroom-32-rescue:POT-MEGA2560-esp-wroom-32-rescue Pxx1
U 1 1 55731CC9
P 5050 2500
F 0 "Pxx1" H 5050 2400 50  0000 C CNN
F 1 "POT" H 5050 2500 50  0000 C CNN
F 2 "~" H 5050 2500 60  0000 C CNN
F 3 "~" H 5050 2500 60  0000 C CNN
	1    5050 2500
	0    1    1    0   
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR015
U 1 1 55731CE2
P 5400 2900
F 0 "#PWR015" H 5400 2900 50  0001 C CNN
F 1 "GND" H 5400 2830 50  0001 C CNN
F 2 "" H 5400 2900 60  0001 C CNN
F 3 "" H 5400 2900 60  0001 C CNN
	1    5400 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 2150 5050 2250
$Comp
L esp32-wroom-32-rescue:C-Device-esp-wroom-32-rescue C1
U 1 1 55732064
P 5400 2700
F 0 "C1" V 5350 2550 50  0000 L CNN
F 1 "100n" V 5550 2550 50  0000 L CNN
F 2 "" H 5400 2700 60  0001 C CNN
F 3 "" H 5400 2700 60  0001 C CNN
	1    5400 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 2900 5050 2900
Text Label 5850 2500 2    42   ~ 0
Stick_xx
Wire Wire Line
	5200 2500 5400 2500
Text Notes 4900 2000 0    60   ~ 0
Stick anti-jitter filter
Wire Wire Line
	1500 1250 1750 1250
Wire Wire Line
	1050 2650 1300 2650
Wire Wire Line
	8100 2750 9050 2750
Wire Wire Line
	8550 4250 8550 4550
Wire Wire Line
	10600 5500 11100 5500
Wire Wire Line
	8250 3850 8550 3850
Wire Wire Line
	8100 2100 8350 2100
Wire Wire Line
	8350 2100 8650 2100
Wire Wire Line
	8350 2900 9050 2900
$Comp
L esp32-wroom-32-rescue:ESP32-WROOM-ESP32-footprints-Shem-Lib-esp-wroom-32-rescue U1
U 1 1 5C2D33A7
P 2200 4600
F 0 "U1" H 2175 5987 60  0000 C CNN
F 1 "ESP32-WROOM" H 2175 5881 60  0000 C CNN
F 2 "ESP32-footprints-Lib:ESP32-WROOM" H 2550 5950 60  0001 C CNN
F 3 "" H 1750 5050 60  0001 C CNN
	1    2200 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 1000 7700 1000
Wire Wire Line
	8000 1000 8100 1000
Wire Wire Line
	8100 1050 8100 1000
Connection ~ 8100 1000
Wire Wire Line
	8100 1000 8650 1000
Wire Wire Line
	6900 2200 6900 2100
Wire Wire Line
	6750 2200 6750 2100
Connection ~ 6750 2100
Wire Wire Line
	6750 2500 6750 2650
Wire Wire Line
	6900 2500 6900 2750
Wire Wire Line
	7000 2750 6900 2750
Connection ~ 6900 2750
Wire Wire Line
	7300 2750 8100 2750
Wire Wire Line
	6750 2650 7450 2650
Wire Wire Line
	6650 2900 7450 2900
Wire Wire Line
	7750 2900 8350 2900
Wire Wire Line
	8350 2500 8350 2900
Wire Wire Line
	8100 2500 8100 2750
Wire Wire Line
	7850 2500 7850 2650
Wire Wire Line
	7850 2100 7850 2200
Wire Wire Line
	8100 2100 8100 2200
Wire Wire Line
	8350 2100 8350 2200
Wire Wire Line
	9600 1150 9600 1100
Connection ~ 9600 1100
Wire Wire Line
	9600 1450 9600 1600
Wire Wire Line
	10400 1450 10400 1600
Wire Wire Line
	10400 1150 10400 1100
Connection ~ 10400 1100
Wire Wire Line
	10750 1100 10750 1300
Wire Wire Line
	10750 1750 10750 1600
Wire Wire Line
	10750 2050 10750 2250
Wire Wire Line
	10250 2450 10450 2450
Wire Wire Line
	9450 2450 9950 2450
Wire Wire Line
	10450 2550 10450 2450
Connection ~ 10450 2450
Wire Wire Line
	10450 2850 10450 3050
Wire Wire Line
	10350 5400 10350 5500
Wire Wire Line
	10350 5600 10350 5500
Wire Wire Line
	10350 5500 10600 5500
Wire Wire Line
	10350 5900 10350 6000
Wire Wire Line
	10600 5950 10600 6000
Wire Wire Line
	10600 5500 10600 5650
Wire Wire Line
	8650 4550 8550 4550
Wire Wire Line
	8950 4550 9050 4550
Wire Wire Line
	9050 4650 9050 4550
Connection ~ 9050 4550
Wire Wire Line
	8350 5550 8250 5550
Connection ~ 8250 5550
Wire Wire Line
	8650 5550 8750 5550
Wire Wire Line
	8750 5650 8750 5550
Connection ~ 8750 5550
Wire Wire Line
	8750 5950 8750 6150
Wire Wire Line
	6050 1450 6050 1650
Wire Wire Line
	6050 1150 6050 1050
Wire Wire Line
	6050 1050 5950 1050
Connection ~ 6050 1050
Wire Wire Line
	5450 1050 5650 1050
Wire Wire Line
	2300 2500 2500 2500
Wire Wire Line
	2000 2500 1900 2500
Connection ~ 1900 2500
Wire Wire Line
	2900 1650 3400 1650
Wire Wire Line
	2600 1650 2500 1650
Connection ~ 2500 1650
Wire Wire Line
	2500 1650 2500 1250
Wire Wire Line
	2150 1650 2350 1650
Wire Wire Line
	1850 1650 1750 1650
Connection ~ 1750 1650
Wire Wire Line
	5400 2550 5400 2500
Connection ~ 5400 2500
Wire Wire Line
	5400 2500 5850 2500
Wire Wire Line
	5400 2900 5400 2850
Wire Wire Line
	2950 2550 3450 2550
Wire Wire Line
	3750 2550 4050 2550
Wire Wire Line
	4350 2550 4600 2550
Wire Wire Line
	1050 4000 1250 4000
Wire Wire Line
	1100 5300 1250 5300
Wire Wire Line
	1250 4500 700  4500
Wire Wire Line
	1250 4600 700  4600
Wire Wire Line
	1250 4400 700  4400
Wire Wire Line
	1250 4200 700  4200
Wire Wire Line
	1250 4300 700  4300
Wire Wire Line
	1250 4700 700  4700
Wire Wire Line
	1250 4800 700  4800
Wire Wire Line
	1250 5100 700  5100
Wire Wire Line
	1250 5200 700  5200
$Comp
L esp32-wroom-32-rescue:MCP23017_SP-Interface_Expansion-esp-wroom-32-rescue U2
U 1 1 5CECE7B2
P 4750 4450
F 0 "U2" V 4700 4150 50  0000 C CNN
F 1 "MCP23017_SP" V 4700 4550 50  0000 C CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 4950 3450 50  0001 L CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf" H 4950 3350 50  0001 L CNN
	1    4750 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 5000 7750 5000
Wire Wire Line
	7750 5100 7750 5000
Connection ~ 7750 5000
Wire Wire Line
	7750 5000 7850 5000
Wire Wire Line
	7750 5400 7750 5600
Wire Wire Line
	8150 5000 8250 5000
Connection ~ 8250 5000
Wire Wire Line
	8250 5000 8250 5150
$Comp
L esp32-wroom-32-rescue:GND-MEGA2560-esp-wroom-32-rescue #PWR011
U 1 1 5D013A16
P 4750 5700
F 0 "#PWR011" H 4750 5700 30  0001 C CNN
F 1 "GND" H 4750 5630 30  0001 C CNN
F 2 "" H 4750 5700 60  0000 C CNN
F 3 "" H 4750 5700 60  0000 C CNN
	1    4750 5700
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:VCC-MEGA2560-esp-wroom-32-rescue #PWR010
U 1 1 5D013C96
P 4750 3150
F 0 "#PWR010" H 4750 3250 30  0001 C CNN
F 1 "VCC" H 4800 3250 30  0000 C CNN
F 2 "" H 4750 3150 60  0000 C CNN
F 3 "" H 4750 3150 60  0000 C CNN
	1    4750 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 3850 3250 3850
Wire Wire Line
	3100 3950 3350 3950
Wire Wire Line
	4050 5500 4750 5500
Wire Wire Line
	700  5000 1250 5000
Wire Wire Line
	2650 6100 2650 5650
Wire Wire Line
	5450 4550 5950 4550
Wire Wire Line
	5450 4650 5950 4650
Wire Wire Line
	5450 4750 5950 4750
Wire Wire Line
	5450 4850 5950 4850
Wire Wire Line
	5450 4950 5950 4950
Wire Wire Line
	5450 5050 5950 5050
Wire Wire Line
	5450 5150 5950 5150
Wire Wire Line
	5450 5250 5950 5250
Wire Wire Line
	3550 7250 3550 7600
Wire Wire Line
	3150 7250 3150 7600
Wire Wire Line
	3050 7250 3050 7600
Text Label 3050 7600 1    42   ~ 0
SW_Trn
$Comp
L esp32-wroom-32-rescue:GND-MEGA2560-esp-wroom-32-rescue #PWR07
U 1 1 5D5494FA
P 3300 5450
F 0 "#PWR07" H 3300 5450 30  0001 C CNN
F 1 "GND" H 3300 5380 30  0001 C CNN
F 2 "" H 3300 5450 60  0000 C CNN
F 3 "" H 3300 5450 60  0000 C CNN
	1    3300 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 5150 3300 5150
Wire Wire Line
	3100 5250 3300 5250
Wire Wire Line
	3300 5150 3300 5250
Connection ~ 3300 5250
Wire Wire Line
	3300 5250 3300 5450
Wire Wire Line
	7750 2650 7850 2650
Connection ~ 7850 2650
Connection ~ 8550 3850
Wire Wire Line
	3350 3950 3350 3750
Wire Wire Line
	3350 3750 3650 3750
Wire Wire Line
	3250 3650 3750 3650
Wire Wire Line
	4350 5850 4350 5800
Connection ~ 3750 3650
Wire Wire Line
	4450 5850 4450 5700
Wire Wire Line
	3650 3750 4050 3750
$Comp
L esp32-wroom-32-rescue:VCC-MEGA2560-esp-wroom-32-rescue #PWR012
U 1 1 5C32810C
P 4750 6400
F 0 "#PWR012" H 4750 6500 30  0001 C CNN
F 1 "VCC" H 4753 6528 30  0000 C CNN
F 2 "" H 4750 6400 60  0000 C CNN
F 3 "" H 4750 6400 60  0000 C CNN
	1    4750 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 6400 4750 6550
$Comp
L esp32-wroom-32-rescue:GND-MEGA2560-esp-wroom-32-rescue #PWR05
U 1 1 5C3374EC
P 2500 6700
F 0 "#PWR05" H 2500 6700 30  0001 C CNN
F 1 "GND" H 2500 6630 30  0001 C CNN
F 2 "" H 2500 6700 60  0000 C CNN
F 3 "" H 2500 6700 60  0000 C CNN
	1    2500 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 6550 2500 6550
Wire Wire Line
	2500 6550 2500 6700
Wire Wire Line
	3050 5850 3050 5750
Wire Wire Line
	2950 5850 2950 5750
Wire Wire Line
	2950 5750 3050 5750
Connection ~ 3050 5750
Wire Wire Line
	3050 5750 3050 5600
$Comp
L esp32-wroom-32-rescue:VCC-MEGA2560-esp-wroom-32-rescue #PWR06
U 1 1 5C366CB1
P 2850 5800
F 0 "#PWR06" H 2850 5900 30  0001 C CNN
F 1 "VCC" H 2853 5928 30  0000 C CNN
F 2 "" H 2850 5800 60  0000 C CNN
F 3 "" H 2850 5800 60  0000 C CNN
	1    2850 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 5800 2850 5850
$Comp
L esp32-wroom-32-rescue:MCP23017_SP-Interface_Expansion-esp-wroom-32-rescue U3
U 1 1 5C2FC793
P 3650 6550
F 0 "U3" V 3650 6150 50  0000 C CNN
F 1 "MCP23017_SP" V 3650 6500 50  0000 C CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 3850 5550 50  0001 L CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf" H 3850 5450 50  0001 L CNN
	1    3650 6550
	0    1    1    0   
$EndComp
Wire Wire Line
	4450 5700 3750 5700
Wire Wire Line
	4350 5800 3650 5800
Wire Wire Line
	2850 7250 2850 7600
Wire Wire Line
	2950 7250 2950 7600
Text Label 3250 3650 0    50   ~ 0
I2C_SDA
Text Label 3350 3750 0    50   ~ 0
I2C_SCL
Wire Wire Line
	4250 7250 4250 7600
Wire Wire Line
	4350 7250 4350 7600
Wire Wire Line
	3450 7250 3450 7600
Wire Wire Line
	3950 5050 3950 4550
Wire Wire Line
	3950 4550 4050 4550
Wire Wire Line
	3100 5050 3550 5050
Wire Wire Line
	3550 5850 3550 5050
Connection ~ 3550 5050
Wire Wire Line
	3550 5050 3950 5050
Text Label 3550 4950 2    42   ~ 0
SD_N_CS
Wire Wire Line
	3100 4950 3550 4950
Wire Wire Line
	700  4900 1250 4900
Wire Wire Line
	3850 5850 3850 4250
Wire Wire Line
	3850 4250 3100 4250
Text Label 3550 4650 2    42   ~ 0
SD_MOSI
Wire Wire Line
	3100 4650 3550 4650
Text Label 3550 4550 2    42   ~ 0
SD_SCK
Wire Wire Line
	3100 4550 3550 4550
Text Label 3550 4450 2    42   ~ 0
SD_MISO
Wire Wire Line
	3750 7250 3750 7600
Wire Wire Line
	3750 3650 3750 5700
Wire Wire Line
	4750 5550 4750 5700
Connection ~ 4050 5500
Wire Wire Line
	4050 5500 4050 5600
Wire Wire Line
	3250 3650 3250 3850
Connection ~ 3650 3750
Wire Wire Line
	3650 3750 3650 5800
Wire Wire Line
	3750 3650 4050 3650
Wire Wire Line
	3050 5600 4050 5600
Text Label 3300 5050 0    42   ~ 0
N_RESET
Wire Wire Line
	4450 7250 4450 7650
Wire Wire Line
	5450 3650 5850 3650
Wire Wire Line
	5450 3750 5850 3750
Wire Wire Line
	5450 3850 5850 3850
Wire Wire Line
	5450 3950 5850 3950
Wire Wire Line
	5450 4050 5850 4050
Wire Wire Line
	5450 4150 5850 4150
Wire Wire Line
	4050 5250 4050 5500
Connection ~ 4050 5150
Connection ~ 4050 5250
Wire Wire Line
	4050 5050 4050 5150
Wire Wire Line
	4050 5150 4050 5250
$Comp
L Device:C C2
U 1 1 5CBA5BB9
P 4750 6700
F 0 "C2" H 4865 6746 50  0000 L CNN
F 1 "100n" H 4865 6655 50  0000 L CNN
F 2 "" H 4788 6550 50  0001 C CNN
F 3 "~" H 4750 6700 50  0001 C CNN
	1    4750 6700
	1    0    0    -1  
$EndComp
Connection ~ 4750 6550
$Comp
L Device:C 100n1
U 1 1 5CBA5F4B
P 5050 3300
F 0 "100n1" V 4798 3300 50  0000 C CNN
F 1 "C?" V 4889 3300 50  0000 C CNN
F 2 "" H 5088 3150 50  0001 C CNN
F 3 "~" H 5050 3300 50  0001 C CNN
	1    5050 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	4750 3350 4750 3300
Wire Wire Line
	4900 3300 4750 3300
Connection ~ 4750 3300
Wire Wire Line
	4750 3300 4750 3150
$Comp
L power:GND #PWR014
U 1 1 5CBC8A57
P 5200 3300
F 0 "#PWR014" H 5200 3050 50  0001 C CNN
F 1 "GND" V 5205 3172 50  0000 R CNN
F 2 "" H 5200 3300 50  0001 C CNN
F 3 "" H 5200 3300 50  0001 C CNN
	1    5200 3300
	0    -1   -1   0   
$EndComp
Connection ~ 5400 2900
Wire Wire Line
	5050 2650 5050 2750
$Comp
L power:GND #PWR013
U 1 1 5CBDB2BC
P 4750 6850
F 0 "#PWR013" H 4750 6600 50  0001 C CNN
F 1 "GND" H 4755 6677 50  0000 C CNN
F 2 "" H 4750 6850 50  0001 C CNN
F 3 "" H 4750 6850 50  0001 C CNN
	1    4750 6850
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AZ1117-3.3 U?
U 1 1 5CBEE637
P 4350 1100
F 0 "U?" H 4350 1342 50  0000 C CNN
F 1 "AZ1117-3.3" H 4350 1251 50  0000 C CNN
F 2 "" H 4350 1350 50  0001 C CIN
F 3 "https://www.diodes.com/assets/Datasheets/AZ1117.pdf" H 4350 1100 50  0001 C CNN
	1    4350 1100
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:VCC-power-esp-wroom-32-rescue #PWR?
U 1 1 5CBEE779
P 4050 1100
F 0 "#PWR?" H 4050 950 50  0001 C CNN
F 1 "VCC-power" V 4068 1227 50  0000 L CNN
F 2 "" H 4050 1100 50  0001 C CNN
F 3 "" H 4050 1100 50  0001 C CNN
	1    4050 1100
	0    -1   -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR?
U 1 1 5CBEEBD3
P 4350 1700
F 0 "#PWR?" H 4350 1450 50  0001 C CNN
F 1 "GND-power" H 4355 1527 50  0000 C CNN
F 2 "" H 4350 1700 50  0001 C CNN
F 3 "" H 4350 1700 50  0001 C CNN
	1    4350 1700
	1    0    0    -1  
$EndComp
Text Label 4950 1100 0    50   ~ 0
Vref
Wire Wire Line
	4650 1100 4800 1100
Text Label 5050 2150 0    50   ~ 0
Vref
$Comp
L Device:C C?
U 1 1 5CBFFF64
P 4800 1400
F 0 "C?" H 4650 1500 50  0000 L CNN
F 1 "100n" H 4500 1350 50  0000 L CNN
F 2 "" H 4838 1250 50  0001 C CNN
F 3 "~" H 4800 1400 50  0001 C CNN
	1    4800 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C?
U 1 1 5CC000D6
P 5100 1400
F 0 "C?" H 5218 1446 50  0000 L CNN
F 1 "1u" H 5218 1355 50  0000 L CNN
F 2 "" H 5138 1250 50  0001 C CNN
F 3 "~" H 5100 1400 50  0001 C CNN
	1    5100 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 1700 4350 1550
Wire Wire Line
	5100 1550 4800 1550
Wire Wire Line
	4800 1550 4350 1550
Connection ~ 4800 1550
Connection ~ 4350 1550
Wire Wire Line
	4350 1550 4350 1400
Wire Wire Line
	4800 1250 4800 1100
Wire Wire Line
	4800 1100 5100 1100
Wire Wire Line
	5100 1100 5100 1250
Connection ~ 4800 1100
Connection ~ 5050 2750
Wire Wire Line
	5050 2750 5050 2900
Wire Wire Line
	6050 2650 6050 3050
Text Notes 4200 700  0    60   ~ 0
Stick Vref
$Comp
L Device:R R?
U 1 1 5CD04313
P 6050 7300
F 0 "R?" H 6120 7346 50  0000 L CNN
F 1 "2k2" H 6120 7255 50  0000 L CNN
F 2 "" V 5980 7300 50  0001 C CNN
F 3 "~" H 6050 7300 50  0001 C CNN
	1    6050 7300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5CD0446A
P 6050 6900
F 0 "R?" H 6120 6946 50  0000 L CNN
F 1 "1K" H 6120 6855 50  0000 L CNN
F 2 "" V 5980 6900 50  0001 C CNN
F 3 "~" H 6050 6900 50  0001 C CNN
	1    6050 6900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5CD045A9
P 6050 7450
F 0 "#PWR?" H 6050 7200 50  0001 C CNN
F 1 "GND" H 6055 7277 50  0000 C CNN
F 2 "" H 6050 7450 50  0001 C CNN
F 3 "" H 6050 7450 50  0001 C CNN
	1    6050 7450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 7150 6050 7050
Wire Wire Line
	5650 6750 6050 6750
Text Label 5650 6750 0    50   ~ 0
AUDIO
$Comp
L Device:C C?
U 1 1 5CD6D080
P 6400 7300
F 0 "C?" H 6515 7346 50  0000 L CNN
F 1 "100n" H 6515 7255 50  0000 L CNN
F 2 "" H 6438 7150 50  0001 C CNN
F 3 "~" H 6400 7300 50  0001 C CNN
	1    6400 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 7150 6050 7150
Connection ~ 6050 7150
Wire Wire Line
	6400 7450 6050 7450
Connection ~ 6050 7450
Wire Wire Line
	6400 7150 6800 7150
Connection ~ 6400 7150
Text Label 6500 7150 0    50   ~ 0
AU_AMPL
Text Notes 5950 6650 0    60   ~ 0
Audio filter
Text Notes 5200 5800 0    50   ~ 0
Power and PPM managment has not been \nimplemented in firmware.\nSD card SPI needs external pull ups (not drawn).
Text Label 3950 7700 1    42   ~ 0
RF_Power
Wire Wire Line
	3950 7250 3950 7700
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR?
U 1 1 5CF84FA3
P 9750 6000
F 0 "#PWR?" H 9750 6000 30  0001 C CNN
F 1 "GND" H 9750 5930 30  0001 C CNN
F 2 "" H 9750 6000 60  0001 C CNN
F 3 "" H 9750 6000 60  0001 C CNN
	1    9750 6000
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R?
U 1 1 5CF84FA9
P 9750 5800
F 0 "R?" V 9830 5800 50  0000 C CNN
F 1 "100K" V 9750 5800 50  0000 C CNN
F 2 "" H 9750 5800 60  0001 C CNN
F 3 "" H 9750 5800 60  0001 C CNN
	1    9750 5800
	-1   0    0    1   
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R?
U 1 1 5CF84FB0
P 9500 5550
F 0 "R?" V 9580 5550 50  0000 C CNN
F 1 "5.1K" V 9500 5550 50  0000 C CNN
F 2 "" H 9500 5550 60  0001 C CNN
F 3 "" H 9500 5550 60  0001 C CNN
	1    9500 5550
	0    -1   -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:GND-power-esp-wroom-32-rescue #PWR?
U 1 1 5CF84FC5
P 10050 6000
F 0 "#PWR?" H 10050 6000 30  0001 C CNN
F 1 "GND" H 10050 5930 30  0001 C CNN
F 2 "" H 10050 6000 60  0001 C CNN
F 3 "" H 10050 6000 60  0001 C CNN
	1    10050 6000
	1    0    0    -1  
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R?
U 1 1 5CF84FCB
P 10050 5100
F 0 "R?" V 10130 5100 50  0000 C CNN
F 1 "5.1K" V 10050 5100 50  0000 C CNN
F 2 "" H 10050 5100 60  0001 C CNN
F 3 "" H 10050 5100 60  0001 C CNN
	1    10050 5100
	-1   0    0    1   
$EndComp
$Comp
L esp32-wroom-32-rescue:MOSFET_P-MEGA2560-esp-wroom-32-rescue Q?
U 1 1 5CF84FD2
P 10050 4650
F 0 "Q?" V 10000 4900 60  0000 R CNN
F 1 "NDT2955" V 10250 4750 40  0000 R CNN
F 2 "" H 10050 4650 60  0001 C CNN
F 3 "" H 10050 4650 60  0001 C CNN
	1    10050 4650
	0    1    -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:R-Device-esp-wroom-32-rescue R?
U 1 1 5CF84FD9
P 9800 4850
F 0 "R?" V 9880 4850 50  0000 C CNN
F 1 "100K" V 9800 4850 50  0000 C CNN
F 2 "" H 9800 4850 60  0001 C CNN
F 3 "" H 9800 4850 60  0001 C CNN
	1    9800 4850
	0    1    -1   0   
$EndComp
$Comp
L esp32-wroom-32-rescue:MOSFET_N-MEGA2560-esp-wroom-32-rescue Q?
U 1 1 5CF84FE0
P 9950 5550
F 0 "Q?" H 10200 5600 50  0000 R CNN
F 1 "MMFT960" H 9950 5750 50  0000 R CNN
F 2 "~" H 9950 5550 60  0000 C CNN
F 3 "~" H 9950 5550 60  0000 C CNN
	1    9950 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 4550 9550 4550
Wire Wire Line
	10050 6000 10050 5750
Wire Wire Line
	9550 4550 9550 4850
Wire Wire Line
	9650 4850 9550 4850
Wire Wire Line
	9950 4850 10050 4850
Wire Wire Line
	10050 4950 10050 4850
Connection ~ 10050 4850
Wire Wire Line
	10050 5350 10050 5250
Wire Wire Line
	9650 5550 9750 5550
Wire Wire Line
	9750 5650 9750 5550
Connection ~ 9750 5550
Wire Wire Line
	9750 5950 9750 6000
Text Label 10350 4550 0    50   ~ 0
RF_¸VCC
Wire Wire Line
	10250 4550 10350 4550
Wire Wire Line
	9550 4550 9550 4250
Connection ~ 9550 4550
Wire Wire Line
	8550 3850 8550 4250
Wire Wire Line
	9050 4950 9050 5350
Wire Wire Line
	10100 3850 10200 3850
Wire Wire Line
	9550 4250 10200 4250
Wire Wire Line
	10200 4250 10200 3850
Connection ~ 9550 4250
Connection ~ 10200 3850
Wire Wire Line
	10200 3850 10500 3850
Connection ~ 10350 5500
Wire Wire Line
	10350 4550 10350 5100
Connection ~ 10350 4550
Wire Wire Line
	10350 4550 10650 4550
Wire Wire Line
	9350 5550 9350 5100
Text Label 9350 5400 1    50   ~ 0
RF_POWER
Wire Wire Line
	3000 4450 3100 4450
Connection ~ 3100 4450
Wire Wire Line
	3100 4450 3550 4450
$EndSCHEMATC
