EESchema Schematic File Version 4
LIBS:esp32-WiFi-kit-32-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "OpenTX board based on Heltec ESP32 WiFi Kit 32"
Date "2019-06-08"
Rev "0"
Comp "Copyright T. Mertelj"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L esp32-WiFi-kit-32-rescue:C-Device-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue C6
U 1 1 5499AA3F
P 10600 5800
F 0 "C6" V 10550 5650 50  0000 L CNN
F 1 "100n" V 10650 5650 50  0000 L CNN
F 2 "" H 10600 5800 60  0001 C CNN
F 3 "" H 10600 5800 60  0001 C CNN
	1    10600 5800
	1    0    0    -1  
$EndComp
Text Notes 8400 3300 0    60   ~ 0
Power management
Text Label 11100 5500 2    42   ~ 0
RF_Activated
$Comp
L esp32-WiFi-kit-32-rescue:GND-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR014
U 1 1 54994281
P 10350 6000
F 0 "#PWR014" H 10350 6000 30  0001 C CNN
F 1 "GND" H 10350 5930 30  0001 C CNN
F 2 "" H 10350 6000 60  0001 C CNN
F 3 "" H 10350 6000 60  0001 C CNN
	1    10350 6000
	1    0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:R-Device-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue R10
U 1 1 54994280
P 10350 5250
F 0 "R10" V 10430 5250 50  0000 C CNN
F 1 "10K" V 10350 5250 50  0000 C CNN
F 2 "" H 10350 5250 60  0001 C CNN
F 3 "" H 10350 5250 60  0001 C CNN
	1    10350 5250
	-1   0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:R-Device-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue R11
U 1 1 5499427F
P 10350 5750
F 0 "R11" V 10430 5750 50  0000 C CNN
F 1 "6.8K" V 10350 5750 50  0000 C CNN
F 2 "" H 10350 5750 60  0001 C CNN
F 3 "" H 10350 5750 60  0001 C CNN
	1    10350 5750
	-1   0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:SPST-MEGA2560-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue SW1
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
L esp32-WiFi-kit-32-rescue:R-Device-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue R4
U 1 1 54994034
P 6050 1300
F 0 "R4" V 6130 1300 50  0000 C CNN
F 1 "2.7K" V 6050 1300 50  0000 C CNN
F 2 "" H 6050 1300 60  0001 C CNN
F 3 "" H 6050 1300 60  0001 C CNN
	1    6050 1300
	-1   0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:R-Device-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue R3
U 1 1 54994033
P 5800 1050
F 0 "R3" V 5880 1050 50  0000 C CNN
F 1 "5.1K" V 5800 1050 50  0000 C CNN
F 2 "" H 5800 1050 60  0001 C CNN
F 3 "" H 5800 1050 60  0001 C CNN
	1    5800 1050
	0    1    -1   0   
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:GND-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR09
U 1 1 54994032
P 6050 1650
F 0 "#PWR09" H 6050 1650 30  0001 C CNN
F 1 "GND" H 6050 1580 30  0001 C CNN
F 2 "" H 6050 1650 60  0001 C CNN
F 3 "" H 6050 1650 60  0001 C CNN
	1    6050 1650
	1    0    0    -1  
$EndComp
Text Notes 5400 700  0    60   ~ 0
Battery voltage reading
Text Notes 6850 750  0    60   ~ 0
Switches, trims & keybs deboucing (x22)
$Comp
L esp32-WiFi-kit-32-rescue:GND-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR011
U 1 1 54992E9C
P 8100 1500
F 0 "#PWR011" H 8100 1500 30  0001 C CNN
F 1 "GND" H 8100 1430 30  0001 C CNN
F 2 "" H 8100 1500 60  0001 C CNN
F 3 "" H 8100 1500 60  0001 C CNN
	1    8100 1500
	1    0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:C-Device-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue C5
U 1 1 54992E49
P 8100 1200
F 0 "C5" V 7850 1100 50  0000 L CNN
F 1 "100n" V 7950 1100 50  0000 L CNN
F 2 "" H 8100 1200 60  0001 C CNN
F 3 "" H 8100 1200 60  0001 C CNN
	1    8100 1200
	1    0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:R-Device-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue R5
U 1 1 54992E3A
P 7850 1000
F 0 "R5" V 7930 1000 50  0000 C CNN
F 1 "200" V 7850 1000 50  0000 C CNN
F 2 "" H 7850 1000 60  0001 C CNN
F 3 "" H 7850 1000 60  0001 C CNN
	1    7850 1000
	0    -1   -1   0   
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:GND-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR010
U 1 1 54992E1E
P 6900 1500
F 0 "#PWR010" H 6900 1500 30  0001 C CNN
F 1 "GND" H 6900 1430 30  0001 C CNN
F 2 "" H 6900 1500 60  0001 C CNN
F 3 "" H 6900 1500 60  0001 C CNN
	1    6900 1500
	1    0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:SW_PUSH-MEGA2560-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue SW_xx1
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
Text Label 6250 4050 2    42   ~ 0
Key_►
Text Label 6250 3850 2    42   ~ 0
Key_▼
$Comp
L esp32-WiFi-kit-32-rescue:GND-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR015
U 1 1 549881D0
P 10600 6000
F 0 "#PWR015" H 10600 6000 30  0001 C CNN
F 1 "GND" H 10600 5930 30  0001 C CNN
F 2 "" H 10600 6000 60  0001 C CNN
F 3 "" H 10600 6000 60  0001 C CNN
	1    10600 6000
	1    0    0    -1  
$EndComp
Text Label 1500 4050 0    42   ~ 0
SW_AilDR
Text Label 1800 4350 2    42   ~ 0
SW_ThCut
Text Label 1500 4250 0    42   ~ 0
SW_RudDR
Text Label 6250 3750 2    42   ~ 0
Key_ESC
Text Label 6250 3650 2    42   ~ 0
Key_MENU
Text Label 1800 4150 2    42   ~ 0
SW_EleDR
Text Label 6250 3950 2    42   ~ 0
Key_▲
Text Label 6250 4150 2    42   ~ 0
Key_◄
Text Label 1500 4650 0    42   ~ 0
SW_ID2
Text Label 1500 4550 0    42   ~ 0
SW_ID1
Text Label 6350 4850 2    42   ~ 0
Trim_LV-►
Text Label 6350 4750 2    42   ~ 0
Trim_LV◄-
Text Label 6350 5050 2    42   ~ 0
Trim_RV-►
Text Label 6350 4950 2    42   ~ 0
Trim_RV◄-
Text Label 6350 5150 2    42   ~ 0
Trim_RH◄-
Text Label 6350 5250 2    42   ~ 0
Trim_RH-►
Text Label 6350 4550 2    42   ~ 0
Trim_LH◄-
Text Label 6350 4650 2    42   ~ 0
Trim_LH-►
Text Label 1500 5250 0    42   ~ 0
Stick_LV
Text Label 1500 5350 0    42   ~ 0
Stick_RH
Text Label 1500 4750 0    42   ~ 0
U_batt
Text Label 1500 4950 0    42   ~ 0
Pot_1
Text Label 1500 4850 0    42   ~ 0
Pot_2
Text Label 1500 5150 0    42   ~ 0
Stick_RV
Text Label 1500 5050 0    42   ~ 0
Stick_LH
Text Label 1500 4450 0    42   ~ 0
AUDIO
Text Label 3400 4450 0    42   ~ 0
Telemetry_TX
Text Label 3400 4350 0    42   ~ 0
Telemetry_RX
Text Label 6250 4250 2    42   ~ 0
RF_Activated
Text Label 3400 4750 0    42   ~ 0
PPM_OUT
$Comp
L esp32-WiFi-kit-32-rescue:CONN_1-MEGA2560-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue +1
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
L esp32-WiFi-kit-32-rescue:VCC-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue 3.3V01
U 1 1 54989ABB
P 1600 3750
F 0 "3.3V01" H 1600 3850 60  0001 C CNN
F 1 "VCC" H 1600 3900 50  0000 C CNN
F 2 "" H 1600 3750 60  0001 C CNN
F 3 "" H 1600 3750 60  0001 C CNN
	1    1600 3750
	0    -1   -1   0   
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:+BATT-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue U04
U 1 1 549E8260
P 5450 950
F 0 "U04" H 5450 900 60  0001 C CNN
F 1 "+BATT" H 5450 1050 50  0000 C BNN
F 2 "" H 5450 950 60  0001 C CNN
F 3 "" H 5450 950 60  0001 C CNN
	1    5450 950 
	1    0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:CONN_1-MEGA2560-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue -1
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
Wire Wire Line
	8100 1400 8100 1500
Wire Wire Line
	6900 1500 6900 1000
Wire Wire Line
	3350 4450 3800 4450
Wire Wire Line
	5850 4250 6250 4250
Wire Wire Line
	1450 4350 1800 4350
Wire Wire Line
	1450 4250 1800 4250
Wire Wire Line
	3350 4750 3800 4750
Wire Wire Line
	3350 4350 3800 4350
Connection ~ 10600 5500
Wire Wire Line
	6450 1050 6050 1050
Wire Wire Line
	6900 1000 7000 1000
Wire Wire Line
	5450 950  5450 1050
$Comp
L esp32-WiFi-kit-32-rescue:+BATT-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue U05
U 1 1 54DF6192
P 10500 3850
F 0 "U05" H 10500 3800 60  0001 C CNN
F 1 "+BATT" V 10450 3900 50  0000 C BNN
F 2 "" H 10500 3850 60  0001 C CNN
F 3 "" H 10500 3850 60  0001 C CNN
	1    10500 3850
	0    1    1    0   
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:GND-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue GND01
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
L esp32-WiFi-kit-32-rescue:POT-MEGA2560-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue Pxx1
U 1 1 55731CC9
P 1050 1350
F 0 "Pxx1" H 1050 1250 50  0000 C CNN
F 1 "POT" H 1050 1350 50  0000 C CNN
F 2 "~" H 1050 1350 60  0000 C CNN
F 3 "~" H 1050 1350 60  0000 C CNN
	1    1050 1350
	0    1    1    0   
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:GND-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR02
U 1 1 55731CE2
P 1400 1750
F 0 "#PWR02" H 1400 1750 50  0001 C CNN
F 1 "GND" H 1400 1680 50  0001 C CNN
F 2 "" H 1400 1750 60  0001 C CNN
F 3 "" H 1400 1750 60  0001 C CNN
	1    1400 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1050 1000 1050 1100
$Comp
L esp32-WiFi-kit-32-rescue:C-Device-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue C1
U 1 1 55732064
P 1400 1550
F 0 "C1" V 1350 1400 50  0000 L CNN
F 1 "100n" V 1550 1400 50  0000 L CNN
F 2 "" H 1400 1550 60  0001 C CNN
F 3 "" H 1400 1550 60  0001 C CNN
	1    1400 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 1750 1050 1750
Text Label 1850 1350 2    42   ~ 0
Stick_xx
Wire Wire Line
	1200 1350 1400 1350
Text Notes 900  850  0    60   ~ 0
Stick anti-jitter filter
Wire Wire Line
	10600 5500 11100 5500
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
	6050 1450 6050 1650
Wire Wire Line
	6050 1150 6050 1050
Wire Wire Line
	6050 1050 5950 1050
Connection ~ 6050 1050
Wire Wire Line
	5450 1050 5650 1050
Wire Wire Line
	1400 1400 1400 1350
Connection ~ 1400 1350
Wire Wire Line
	1400 1350 1850 1350
Wire Wire Line
	1400 1750 1400 1700
Wire Wire Line
	1600 3750 1800 3750
Wire Wire Line
	1800 5050 1450 5050
Wire Wire Line
	1800 4950 1450 4950
Wire Wire Line
	1800 5150 1450 5150
Wire Wire Line
	1800 5350 1450 5350
Wire Wire Line
	1800 5250 1450 5250
Wire Wire Line
	1800 4850 1450 4850
Wire Wire Line
	1800 4750 1450 4750
$Comp
L esp32-WiFi-kit-32-rescue:MCP23017_SP-Interface_Expansion-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue U3
U 1 1 5CECE7B2
P 5150 4450
F 0 "U3" V 5100 4150 50  0000 C CNN
F 1 "MCP23017_SP" V 5100 4550 50  0000 C CNN
F 2 "Package_DIP:DIP-28_W7.62mm" H 5350 3450 50  0001 L CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf" H 5350 3350 50  0001 L CNN
	1    5150 4450
	1    0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:GND-MEGA2560-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR07
U 1 1 5D013A16
P 5150 5700
F 0 "#PWR07" H 5150 5700 30  0001 C CNN
F 1 "GND" H 5150 5630 30  0001 C CNN
F 2 "" H 5150 5700 60  0000 C CNN
F 3 "" H 5150 5700 60  0000 C CNN
	1    5150 5700
	1    0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:VCC-MEGA2560-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR06
U 1 1 5D013C96
P 5150 3150
F 0 "#PWR06" H 5150 3250 30  0001 C CNN
F 1 "VCC" H 5200 3250 30  0000 C CNN
F 2 "" H 5150 3150 60  0000 C CNN
F 3 "" H 5150 3150 60  0000 C CNN
	1    5150 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 5500 5150 5500
Wire Wire Line
	5850 4550 6350 4550
Wire Wire Line
	5850 4650 6350 4650
Wire Wire Line
	5850 4750 6350 4750
Wire Wire Line
	5850 4850 6350 4850
Wire Wire Line
	5850 4950 6350 4950
Wire Wire Line
	5850 5050 6350 5050
Wire Wire Line
	5850 5150 6350 5150
Wire Wire Line
	5850 5250 6350 5250
Wire Wire Line
	1450 4050 1800 4050
$Comp
L esp32-WiFi-kit-32-rescue:GND-MEGA2560-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR01
U 1 1 5D5494FA
P 1200 3850
F 0 "#PWR01" H 1200 3850 30  0001 C CNN
F 1 "GND" H 1200 3780 30  0001 C CNN
F 2 "" H 1200 3850 60  0000 C CNN
F 3 "" H 1200 3850 60  0000 C CNN
	1    1200 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 3650 1200 3650
Wire Wire Line
	1200 3650 1200 3850
Wire Wire Line
	4050 3750 4450 3750
Wire Wire Line
	1450 4650 1800 4650
Text Label 3400 5350 0    42   ~ 0
I2C_SDA
Text Label 3400 5250 0    42   ~ 0
I2C_SCL
Wire Wire Line
	1450 4150 1800 4150
Wire Wire Line
	4350 4550 4450 4550
Wire Wire Line
	3350 4650 4350 4650
Text Label 3400 5150 0    42   ~ 0
SD_N_CS
Wire Wire Line
	3800 5150 3350 5150
Wire Wire Line
	1450 4450 1800 4450
Text Label 3400 4850 0    42   ~ 0
SD_MOSI
Wire Wire Line
	3350 4850 3800 4850
Text Label 3400 4950 0    42   ~ 0
SD_SCK
Wire Wire Line
	3350 4950 3800 4950
Text Label 3400 5050 0    42   ~ 0
SD_MISO
Wire Wire Line
	5150 5550 5150 5700
Wire Wire Line
	4050 3750 4050 5250
Wire Wire Line
	4150 3650 4450 3650
Text Label 3400 4650 0    42   ~ 0
DISPL_RESET
Wire Wire Line
	5850 3650 6250 3650
Wire Wire Line
	5850 3750 6250 3750
Wire Wire Line
	5850 3850 6250 3850
Wire Wire Line
	5850 3950 6250 3950
Wire Wire Line
	5850 4050 6250 4050
Wire Wire Line
	5850 4150 6250 4150
Wire Wire Line
	4450 5250 4450 5500
Connection ~ 4450 5150
Connection ~ 4450 5250
Wire Wire Line
	4450 5050 4450 5150
Wire Wire Line
	4450 5150 4450 5250
$Comp
L Device:C 100n1
U 1 1 5CBA5F4B
P 5450 3300
F 0 "100n1" V 5198 3300 50  0000 C CNN
F 1 "C?" V 5289 3300 50  0000 C CNN
F 2 "" H 5488 3150 50  0001 C CNN
F 3 "~" H 5450 3300 50  0001 C CNN
	1    5450 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	5150 3350 5150 3300
Wire Wire Line
	5300 3300 5150 3300
Connection ~ 5150 3300
Wire Wire Line
	5150 3300 5150 3150
$Comp
L power:GND #PWR08
U 1 1 5CBC8A57
P 5600 3300
F 0 "#PWR08" H 5600 3050 50  0001 C CNN
F 1 "GND" V 5605 3172 50  0000 R CNN
F 2 "" H 5600 3300 50  0001 C CNN
F 3 "" H 5600 3300 50  0001 C CNN
	1    5600 3300
	0    -1   -1   0   
$EndComp
Connection ~ 1400 1750
Wire Wire Line
	1050 1500 1050 1600
$Comp
L Regulator_Linear:AZ1117-3.3 U2
U 1 1 5CBEE637
P 4350 1100
F 0 "U2" H 4350 1342 50  0000 C CNN
F 1 "AZ1117-3.3" H 4350 1251 50  0000 C CNN
F 2 "" H 4350 1350 50  0001 C CIN
F 3 "https://www.diodes.com/assets/Datasheets/AZ1117.pdf" H 4350 1100 50  0001 C CNN
	1    4350 1100
	1    0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:VCC-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR04
U 1 1 5CBEE779
P 4050 1100
F 0 "#PWR04" H 4050 950 50  0001 C CNN
F 1 "VCC-power" V 4068 1227 50  0000 L CNN
F 2 "" H 4050 1100 50  0001 C CNN
F 3 "" H 4050 1100 50  0001 C CNN
	1    4050 1100
	0    -1   -1   0   
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:GND-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR05
U 1 1 5CBEEBD3
P 4350 1700
F 0 "#PWR05" H 4350 1450 50  0001 C CNN
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
Text Label 1050 1000 0    50   ~ 0
Vref
$Comp
L Device:C C3
U 1 1 5CBFFF64
P 4800 1400
F 0 "C3" H 4650 1500 50  0000 L CNN
F 1 "100n" H 4500 1350 50  0000 L CNN
F 2 "" H 4838 1250 50  0001 C CNN
F 3 "~" H 4800 1400 50  0001 C CNN
	1    4800 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C4
U 1 1 5CC000D6
P 5100 1400
F 0 "C4" H 5218 1446 50  0000 L CNN
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
Connection ~ 1050 1600
Wire Wire Line
	1050 1600 1050 1750
Text Notes 4200 700  0    60   ~ 0
Stick Vref
$Comp
L Device:R R2
U 1 1 5CD04313
P 2500 1450
F 0 "R2" H 2570 1496 50  0000 L CNN
F 1 "2k2" H 2570 1405 50  0000 L CNN
F 2 "" V 2430 1450 50  0001 C CNN
F 3 "~" H 2500 1450 50  0001 C CNN
	1    2500 1450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5CD0446A
P 2500 1050
F 0 "R1" H 2570 1096 50  0000 L CNN
F 1 "1K" H 2570 1005 50  0000 L CNN
F 2 "" V 2430 1050 50  0001 C CNN
F 3 "~" H 2500 1050 50  0001 C CNN
	1    2500 1050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5CD045A9
P 2500 1600
F 0 "#PWR03" H 2500 1350 50  0001 C CNN
F 1 "GND" H 2505 1427 50  0000 C CNN
F 2 "" H 2500 1600 50  0001 C CNN
F 3 "" H 2500 1600 50  0001 C CNN
	1    2500 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 1300 2500 1200
Wire Wire Line
	2100 900  2500 900 
Text Label 2100 900  0    50   ~ 0
AUDIO
$Comp
L Device:C C2
U 1 1 5CD6D080
P 2850 1450
F 0 "C2" H 2965 1496 50  0000 L CNN
F 1 "100n" H 2965 1405 50  0000 L CNN
F 2 "" H 2888 1300 50  0001 C CNN
F 3 "~" H 2850 1450 50  0001 C CNN
	1    2850 1450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 1300 2500 1300
Connection ~ 2500 1300
Wire Wire Line
	2850 1600 2500 1600
Connection ~ 2500 1600
Wire Wire Line
	2850 1300 3250 1300
Connection ~ 2850 1300
Text Label 2950 1300 0    50   ~ 0
AU_AMPL
Text Notes 2400 800  0    60   ~ 0
Audio filter
Text Notes 5600 5800 0    50   ~ 0
Power managment has not been \nimplemented in firmware.\nSD card SPI needs external pull ups (not drawn).
$Comp
L esp32-WiFi-kit-32-rescue:GND-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR012
U 1 1 5CF84FA3
P 9750 6000
F 0 "#PWR012" H 9750 6000 30  0001 C CNN
F 1 "GND" H 9750 5930 30  0001 C CNN
F 2 "" H 9750 6000 60  0001 C CNN
F 3 "" H 9750 6000 60  0001 C CNN
	1    9750 6000
	1    0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:R-Device-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue R7
U 1 1 5CF84FA9
P 9750 5800
F 0 "R7" V 9830 5800 50  0000 C CNN
F 1 "100K" V 9750 5800 50  0000 C CNN
F 2 "" H 9750 5800 60  0001 C CNN
F 3 "" H 9750 5800 60  0001 C CNN
	1    9750 5800
	-1   0    0    1   
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:R-Device-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue R6
U 1 1 5CF84FB0
P 9500 5550
F 0 "R6" V 9580 5550 50  0000 C CNN
F 1 "5.1K" V 9500 5550 50  0000 C CNN
F 2 "" H 9500 5550 60  0001 C CNN
F 3 "" H 9500 5550 60  0001 C CNN
	1    9500 5550
	0    -1   -1   0   
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:GND-power-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue #PWR013
U 1 1 5CF84FC5
P 10050 6000
F 0 "#PWR013" H 10050 6000 30  0001 C CNN
F 1 "GND" H 10050 5930 30  0001 C CNN
F 2 "" H 10050 6000 60  0001 C CNN
F 3 "" H 10050 6000 60  0001 C CNN
	1    10050 6000
	1    0    0    -1  
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:R-Device-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue R9
U 1 1 5CF84FCB
P 10050 5100
F 0 "R9" V 10130 5100 50  0000 C CNN
F 1 "5.1K" V 10050 5100 50  0000 C CNN
F 2 "" H 10050 5100 60  0001 C CNN
F 3 "" H 10050 5100 60  0001 C CNN
	1    10050 5100
	-1   0    0    1   
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:MOSFET_P-MEGA2560-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue Q2
U 1 1 5CF84FD2
P 10050 4650
F 0 "Q2" V 10000 4900 60  0000 R CNN
F 1 "NDT2955" V 10250 4750 40  0000 R CNN
F 2 "" H 10050 4650 60  0001 C CNN
F 3 "" H 10050 4650 60  0001 C CNN
	1    10050 4650
	0    1    -1   0   
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:R-Device-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue R8
U 1 1 5CF84FD9
P 9800 4850
F 0 "R8" V 9880 4850 50  0000 C CNN
F 1 "100K" V 9800 4850 50  0000 C CNN
F 2 "" H 9800 4850 60  0001 C CNN
F 3 "" H 9800 4850 60  0001 C CNN
	1    9800 4850
	0    1    -1   0   
$EndComp
$Comp
L esp32-WiFi-kit-32-rescue:MOSFET_N-MEGA2560-esp-wroom-32-rescue-esp32-WiFi-kit-32-rescue Q1
U 1 1 5CF84FE0
P 9950 5550
F 0 "Q1" H 10200 5600 50  0000 R CNN
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
Connection ~ 9550 4550
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
$Comp
L esp32-WiFi-kit-32:WiFi-kit-32 U1
U 1 1 5CFD4342
P 2850 4000
F 0 "U1" H 2575 4715 50  0000 C CNN
F 1 "WiFi-kit-32" H 2575 4624 50  0000 C CNN
F 2 "" H 2850 4000 50  0001 C CNN
F 3 "" H 2850 4000 50  0001 C CNN
	1    2850 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 4650 4350 4550
Wire Wire Line
	3350 5250 4050 5250
Wire Wire Line
	3350 5350 4150 5350
Wire Wire Line
	3350 4250 3800 4250
Text Label 3400 4250 0    42   ~ 0
DISPL_SCL
Wire Wire Line
	3350 4550 3800 4550
Text Label 3400 4550 0    42   ~ 0
DISPL_SDA
Wire Wire Line
	4150 3650 4150 5350
Wire Wire Line
	3350 5050 3800 5050
Wire Wire Line
	7450 3850 8550 3850
Connection ~ 9550 3850
Wire Wire Line
	9550 3850 10500 3850
Wire Wire Line
	9550 3850 9550 4550
Text Label 6250 4350 2    42   ~ 0
RF_POWER
Wire Wire Line
	5850 4350 6250 4350
$Comp
L Device:R_US R12
U 1 1 5D0041BE
P 1000 4550
F 0 "R12" V 795 4550 50  0000 C CNN
F 1 "1K" V 886 4550 50  0000 C CNN
F 2 "" V 1040 4540 50  0001 C CNN
F 3 "~" H 1000 4550 50  0001 C CNN
	1    1000 4550
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR016
U 1 1 5D004342
P 800 4500
F 0 "#PWR016" H 800 4350 50  0001 C CNN
F 1 "VCC" H 817 4673 50  0000 C CNN
F 2 "" H 800 4500 50  0001 C CNN
F 3 "" H 800 4500 50  0001 C CNN
	1    800  4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	800  4500 800  4550
Wire Wire Line
	800  4550 850  4550
Wire Wire Line
	1150 4550 1800 4550
Text Notes 550  4800 0    50   ~ 0
On-module LED \npuls dow with 10K.
$EndSCHEMATC
