EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 9
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L modules:TP4056withprotect U3
U 1 1 5FD638AC
P 4950 1750
F 0 "U3" H 5200 1800 50  0000 C CNN
F 1 "TP4056withprotect" H 4950 1250 50  0000 C CNN
F 2 "modules:TP4056_module_prot" H 4950 1700 50  0001 C CNN
F 3 "" H 4950 1700 50  0001 C CNN
F 4 "N/A" H 4950 1750 50  0001 C CNN "Digikey"
F 5 "N/A" H 4950 1750 50  0001 C CNN "LCSC"
F 6 "false" H 4950 1750 50  0001 C CNN "Populate"
	1    4950 1750
	1    0    0    -1  
$EndComp
$Comp
L batman:NT1713C U4
U 1 1 5FD6492C
P 5800 3450
F 0 "U4" H 6178 3438 50  0000 L CNN
F 1 "NT1713C" H 6178 3347 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6_Handsoldering" H 5800 3900 50  0001 C CNN
F 3 "" H 5800 3900 50  0001 C CNN
F 4 "C596362" H 5800 3450 50  0001 C CNN "LCSC"
F 5 "N/A" H 5800 3450 50  0001 C CNN "Digikey"
	1    5800 3450
	1    0    0    -1  
$EndComp
$Comp
L regulators:LP38501 U2
U 1 1 5FD653DD
P 3700 6050
F 0 "U2" H 3700 6425 50  0000 C CNN
F 1 "LP38501" H 3700 6334 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-263-5_TabPin6" H 3700 6300 50  0001 C CNN
F 3 "" H 3700 6300 50  0001 C CNN
F 4 "C779671" H 3700 6050 50  0001 C CNN "LCSC"
F 5 "LP38501TSX-ADJ/NOPBCT-ND" H 3700 6050 50  0001 C CNN "Digikey"
	1    3700 6050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR013
U 1 1 5FD689FA
P 5500 5900
F 0 "#PWR013" H 5500 5750 50  0001 C CNN
F 1 "+3.3V" H 5515 6073 50  0000 C CNN
F 2 "" H 5500 5900 50  0001 C CNN
F 3 "" H 5500 5900 50  0001 C CNN
	1    5500 5900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5FD69161
P 3700 6600
F 0 "#PWR08" H 3700 6350 50  0001 C CNN
F 1 "GND" H 3705 6427 50  0000 C CNN
F 2 "" H 3700 6600 50  0001 C CNN
F 3 "" H 3700 6600 50  0001 C CNN
	1    3700 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 6450 3600 6600
Wire Wire Line
	3600 6600 3700 6600
Wire Wire Line
	3700 6600 3800 6600
Wire Wire Line
	3800 6600 3800 6450
Connection ~ 3700 6600
$Comp
L power:GND #PWR010
U 1 1 5FD6C476
P 4950 2400
F 0 "#PWR010" H 4950 2150 50  0001 C CNN
F 1 "GND" H 4955 2227 50  0000 C CNN
F 2 "" H 4950 2400 50  0001 C CNN
F 3 "" H 4950 2400 50  0001 C CNN
	1    4950 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 6050 3300 6050
$Comp
L Device:Q_PMOS_DGS Q2
U 1 1 5FD6E909
P 6650 2750
F 0 "Q2" V 6900 2750 50  0000 C CNN
F 1 "IRF7410TRPBF" H 6855 2705 50  0001 L CNN
F 2 "transistor:SO8_3.8x4.8_SSSGDDDD" H 6850 2850 50  0001 C CNN
F 3 "https://www.infineon.com/dgdl/irf7410pbf.pdf?fileId=5546d462533600a4015355fa75d71bb0" H 6650 2750 50  0001 C CNN
F 4 "IRF7410TRPBF" V 6650 2750 50  0001 C CNN "Digikey"
F 5 "C148144" V 6650 2750 50  0001 C CNN "LCSC"
	1    6650 2750
	0    -1   1    0   
$EndComp
$Comp
L Device:R R10
U 1 1 5FE076D7
P 6400 2400
F 0 "R10" H 6470 2446 50  0000 L CNN
F 1 "10k" H 6470 2355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 6330 2400 50  0001 C CNN
F 3 "~" H 6400 2400 50  0001 C CNN
F 4 "C132649" H 6400 2400 50  0001 C CNN "LCSC"
F 5 "541-10.0KFCT-ND" H 6400 2400 50  0001 C CNN "Digikey"
	1    6400 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D1
U 1 1 5FE0951C
P 7100 2700
F 0 "D1" V 7100 2620 50  0000 R CNN
F 1 "PMEG3030EP,115" V 7055 2620 50  0001 R CNN
F 2 "Diode_SMD:D_SOD-128" H 7100 2700 50  0001 C CNN
F 3 "~" H 7100 2700 50  0001 C CNN
F 4 "1727-5324-1-ND" H 7100 2700 50  0001 C CNN "Digikey"
F 5 "C255587" H 7100 2700 50  0001 C CNN "LCSC"
	1    7100 2700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7100 2850 6950 2850
Wire Wire Line
	7100 2550 7100 2250
$Comp
L power:GND #PWR020
U 1 1 5FE0D846
P 9400 1750
F 0 "#PWR020" H 9400 1500 50  0001 C CNN
F 1 "GND" H 9405 1577 50  0000 C CNN
F 2 "" H 9400 1750 50  0001 C CNN
F 3 "" H 9400 1750 50  0001 C CNN
	1    9400 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 2550 6650 2250
Connection ~ 6650 2250
Wire Wire Line
	6650 2250 7100 2250
Connection ~ 6400 2250
Wire Wire Line
	6400 2250 6650 2250
$Comp
L power:GND #PWR016
U 1 1 5FE0F9D1
P 6400 2550
F 0 "#PWR016" H 6400 2300 50  0001 C CNN
F 1 "GND" H 6405 2377 50  0000 C CNN
F 2 "" H 6400 2550 50  0001 C CNN
F 3 "" H 6400 2550 50  0001 C CNN
	1    6400 2550
	1    0    0    -1  
$EndComp
$Comp
L batman:TP4056 U1
U 1 1 5FE127E5
P 2400 3350
F 0 "U1" H 2400 3915 50  0000 C CNN
F 1 "TP4056" H 2400 3824 50  0000 C CNN
F 2 "Package_SO:SOIC-8-1EP_3.9x4.9mm_P1.27mm_EP2.29x3mm_ThermalVias" H 2400 3800 50  0001 C CNN
F 3 "" H 2400 3800 50  0001 C CNN
F 4 "C191323" H 2400 3350 50  0001 C CNN "LCSC"
F 5 "N/A" H 2400 3350 50  0001 C CNN "Digikey"
	1    2400 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5FE14998
P 1950 2500
F 0 "R1" H 2020 2591 50  0000 L CNN
F 1 "400m" H 2020 2500 50  0000 L CNN
F 2 "Resistor_SMD:R_2512_6332Metric_Pad1.40x3.35mm_HandSolder" V 1880 2500 50  0001 C CNN
F 3 "~" H 1950 2500 50  0001 C CNN
F 4 "1W" H 2020 2409 50  0000 L CNN "Power"
F 5 "C165323" H 1950 2500 50  0001 C CNN "LCSC"
F 6 "P19396CT-ND" H 1950 2500 50  0001 C CNN "Digikey"
	1    1950 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5FE15884
P 1400 3200
F 0 "C1" H 1515 3246 50  0000 L CNN
F 1 "10u" H 1515 3155 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 1438 3050 50  0001 C CNN
F 3 "~" H 1400 3200 50  0001 C CNN
F 4 "1276-1075-1-ND" H 1400 3200 50  0001 C CNN "Digikey"
F 5 "C9807" H 1400 3200 50  0001 C CNN "LCSC"
	1    1400 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 3050 1950 3050
Wire Wire Line
	1950 3200 1950 3050
Connection ~ 1950 3050
Wire Wire Line
	1950 2650 1950 2700
$Comp
L Device:C C4
U 1 1 5FE17259
P 3050 3000
F 0 "C4" H 3165 3046 50  0000 L CNN
F 1 "10u" H 3165 2955 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 3088 2850 50  0001 C CNN
F 3 "~" H 3050 3000 50  0001 C CNN
F 4 "1276-1075-1-ND" H 3050 3000 50  0001 C CNN "Digikey"
F 5 "C9807" H 3050 3000 50  0001 C CNN "LCSC"
	1    3050 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5FE18AEA
P 2400 4000
F 0 "#PWR04" H 2400 3750 50  0001 C CNN
F 1 "GND" H 2405 3827 50  0000 C CNN
F 2 "" H 2400 4000 50  0001 C CNN
F 3 "" H 2400 4000 50  0001 C CNN
	1    2400 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5FE19456
P 3050 3150
F 0 "#PWR06" H 3050 2900 50  0001 C CNN
F 1 "GND" H 3055 2977 50  0000 C CNN
F 2 "" H 3050 3150 50  0001 C CNN
F 3 "" H 3050 3150 50  0001 C CNN
	1    3050 3150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5FE197BC
P 1400 3350
F 0 "#PWR01" H 1400 3100 50  0001 C CNN
F 1 "GND" H 1405 3177 50  0000 C CNN
F 2 "" H 1400 3350 50  0001 C CNN
F 3 "" H 1400 3350 50  0001 C CNN
	1    1400 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5FE19FA4
P 2950 3750
F 0 "R2" H 3020 3796 50  0000 L CNN
F 1 "1k2" H 3020 3705 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 2880 3750 50  0001 C CNN
F 3 "~" H 2950 3750 50  0001 C CNN
F 4 "C185265" H 2950 3750 50  0001 C CNN "LCSC"
F 5 "CR1206-FX-1201ELFCT-ND" H 2950 3750 50  0001 C CNN "Digikey"
	1    2950 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5FE1A5BC
P 2950 3900
F 0 "#PWR05" H 2950 3650 50  0001 C CNN
F 1 "GND" H 2955 3727 50  0000 C CNN
F 2 "" H 2950 3900 50  0001 C CNN
F 3 "" H 2950 3900 50  0001 C CNN
	1    2950 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 3600 2850 3600
$Comp
L Device:R R4
U 1 1 5FE1AE7F
P 3500 3050
F 0 "R4" H 3570 3096 50  0000 L CNN
F 1 "5k6" H 3570 3005 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 3430 3050 50  0001 C CNN
F 3 "~" H 3500 3050 50  0001 C CNN
F 4 "C137248" H 3500 3050 50  0001 C CNN "LCSC"
F 5 "541-5.60FFCT-ND" H 3500 3050 50  0001 C CNN "Digikey"
	1    3500 3050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 5FE1B320
P 3500 3750
F 0 "R5" H 3570 3796 50  0000 L CNN
F 1 "82k" H 3570 3705 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 3430 3750 50  0001 C CNN
F 3 "~" H 3500 3750 50  0001 C CNN
F 4 "C137229" H 3500 3750 50  0001 C CNN "LCSC"
F 5 "311-82.0KFRCT-ND" H 3500 3750 50  0001 C CNN "Digikey"
	1    3500 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 3400 3300 3400
Wire Wire Line
	3500 3400 3500 3600
Wire Wire Line
	3500 3200 3500 3400
Connection ~ 3500 3400
Wire Wire Line
	3500 2700 1950 2700
Wire Wire Line
	3500 2700 3500 2900
Connection ~ 1950 2700
Wire Wire Line
	1950 2700 1950 3050
$Comp
L power:GND #PWR07
U 1 1 5FE1C959
P 3500 3900
F 0 "#PWR07" H 3500 3650 50  0001 C CNN
F 1 "GND" H 3505 3727 50  0000 C CNN
F 2 "" H 3500 3900 50  0001 C CNN
F 3 "" H 3500 3900 50  0001 C CNN
	1    3500 3900
	1    0    0    -1  
$EndComp
Text Label 1950 3450 2    50   ~ 0
~LED_CHRG
Text Label 1950 3600 2    50   ~ 0
~LED_STBY
$Comp
L Device:R R9
U 1 1 5FE2F3BE
P 6050 4150
F 0 "R9" H 6120 4196 50  0000 L CNN
F 1 "1k" H 6120 4105 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 5980 4150 50  0001 C CNN
F 3 "~" H 6050 4150 50  0001 C CNN
F 4 "C131398" H 6050 4150 50  0001 C CNN "LCSC"
F 5 "311-1.00KFRCT-ND" H 6050 4150 50  0001 C CNN "Digikey"
	1    6050 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 4400 6050 4400
Wire Wire Line
	6050 4400 6050 4300
Wire Wire Line
	6050 4000 6050 3900
Wire Wire Line
	5800 4100 5800 3900
Wire Wire Line
	5400 4100 5550 4100
Wire Wire Line
	5550 4100 5550 3900
$Comp
L Device:C C6
U 1 1 5FE32901
P 5100 3350
F 0 "C6" H 5215 3396 50  0000 L CNN
F 1 "100n" H 5215 3305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 5138 3200 50  0001 C CNN
F 3 "~" H 5100 3350 50  0001 C CNN
F 4 "399-C1206C104K5RAC7800CT-ND" H 5100 3350 50  0001 C CNN "Digikey"
F 5 "C730479" H 5100 3350 50  0001 C CNN "LCSC"
	1    5100 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 5FE32F15
P 4850 3200
F 0 "R6" V 4643 3200 50  0000 C CNN
F 1 "470" V 4734 3200 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 4780 3200 50  0001 C CNN
F 3 "~" H 4850 3200 50  0001 C CNN
F 4 "C114945" H 4850 3200 50  0001 C CNN "LCSC"
F 5 "A106077CT-ND" H 4850 3200 50  0001 C CNN "Digikey"
	1    4850 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	5100 3500 5350 3500
Wire Wire Line
	5100 3200 5350 3200
Wire Wire Line
	5000 3200 5100 3200
Connection ~ 5100 3200
Wire Wire Line
	6050 4400 6350 4400
Connection ~ 6050 4400
Wire Wire Line
	4700 4400 5200 4400
$Comp
L power:GND #PWR015
U 1 1 5FE6158B
P 6350 4400
F 0 "#PWR015" H 6350 4150 50  0001 C CNN
F 1 "GND" H 6355 4227 50  0000 C CNN
F 2 "" H 6350 4400 50  0001 C CNN
F 3 "" H 6350 4400 50  0001 C CNN
	1    6350 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 3500 4700 3500
Wire Wire Line
	4700 3200 4700 3500
Wire Wire Line
	4700 4400 4700 3950
Wire Wire Line
	4700 3600 4650 3600
$Comp
L power:GND #PWR019
U 1 1 5FEB6D80
P 10200 2650
F 0 "#PWR019" H 10200 2400 50  0001 C CNN
F 1 "GND" V 10205 2522 50  0000 R CNN
F 2 "" H 10200 2650 50  0001 C CNN
F 3 "" H 10200 2650 50  0001 C CNN
	1    10200 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	7800 2850 7600 2850
Connection ~ 7100 2850
$Comp
L Device:C C3
U 1 1 5FEDA6D8
P 2500 6150
F 0 "C3" H 2615 6241 50  0000 L CNN
F 1 "22u" H 2615 6150 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 2538 6000 50  0001 C CNN
F 3 "~" H 2500 6150 50  0001 C CNN
F 4 "X5R/X7R" H 2615 6059 50  0000 L CNN "Coeff"
F 5 "1276-2728-1-ND" H 2500 6150 50  0001 C CNN "Digikey"
F 6 "C90146" H 2500 6150 50  0001 C CNN "LCSC"
	1    2500 6150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5FEDD850
P 5500 6550
F 0 "R8" H 5570 6596 50  0000 L CNN
F 1 "5k6" H 5570 6505 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 5430 6550 50  0001 C CNN
F 3 "~" H 5500 6550 50  0001 C CNN
F 4 "C137248" H 5500 6550 50  0001 C CNN "LCSC"
F 5 "541-5.60FFCT-ND" H 5500 6550 50  0001 C CNN "Digikey"
	1    5500 6550
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 5FEDF9C0
P 5500 6150
F 0 "R7" H 5570 6196 50  0000 L CNN
F 1 "24k" H 5570 6105 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 5430 6150 50  0001 C CNN
F 3 "~" H 5500 6150 50  0001 C CNN
F 4 "C163384" H 5500 6150 50  0001 C CNN "LCSC"
F 5 "311-24.0KFRCT-ND" H 5500 6150 50  0001 C CNN "Digikey"
	1    5500 6150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5FEE34C2
P 5500 6700
F 0 "#PWR014" H 5500 6450 50  0001 C CNN
F 1 "GND" H 5505 6527 50  0000 C CNN
F 2 "" H 5500 6700 50  0001 C CNN
F 3 "" H 5500 6700 50  0001 C CNN
	1    5500 6700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 6450 4200 6050
Wire Wire Line
	4200 6050 4100 6050
$Comp
L power:GND #PWR02
U 1 1 5FEE76A0
P 2200 6400
F 0 "#PWR02" H 2200 6150 50  0001 C CNN
F 1 "GND" H 2205 6227 50  0000 C CNN
F 2 "" H 2200 6400 50  0001 C CNN
F 3 "" H 2200 6400 50  0001 C CNN
	1    2200 6400
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP1
U 1 1 5FEEE478
P 3300 3600
F 0 "JP1" H 3350 3500 50  0000 R CNN
F 1 "Jumper_2_Open" V 3255 3512 50  0001 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 3300 3600 50  0001 C CNN
F 3 "~" H 3300 3600 50  0001 C CNN
	1    3300 3600
	0    -1   -1   0   
$EndComp
Connection ~ 3300 3400
Wire Wire Line
	3300 3400 3500 3400
Wire Wire Line
	3300 3800 3300 3900
Wire Wire Line
	3300 3900 3500 3900
Connection ~ 3500 3900
$Comp
L power:GND #PWR012
U 1 1 5FF21084
P 5450 2050
F 0 "#PWR012" H 5450 1800 50  0001 C CNN
F 1 "GND" H 5455 1877 50  0000 C CNN
F 2 "" H 5450 2050 50  0001 C CNN
F 3 "" H 5450 2050 50  0001 C CNN
	1    5450 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 1900 5550 2850
Connection ~ 5550 2850
Wire Wire Line
	5550 2850 6450 2850
Wire Wire Line
	5550 1900 5450 1900
Connection ~ 4700 3600
$Comp
L Device:C C2
U 1 1 5FF52824
P 1900 6150
F 0 "C2" H 2015 6241 50  0000 L CNN
F 1 "100n" H 2015 6150 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 1938 6000 50  0001 C CNN
F 3 "~" H 1900 6150 50  0001 C CNN
F 4 "X5R/X7R" H 2015 6059 50  0000 L CNN "Coeff"
F 5 "399-C1206C104K5RAC7800CT-ND" H 1900 6150 50  0001 C CNN "Digikey"
F 6 "C730479" H 1900 6150 50  0001 C CNN "LCSC"
	1    1900 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 6300 1900 6400
Wire Wire Line
	1900 6400 2200 6400
Wire Wire Line
	2200 6400 2500 6400
Wire Wire Line
	2500 6400 2500 6300
Connection ~ 2200 6400
$Comp
L Device:C C5
U 1 1 5FF61A07
P 4350 6050
F 0 "C5" H 4465 6141 50  0000 L CNN
F 1 "22u" H 4465 6050 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 4388 5900 50  0001 C CNN
F 3 "~" H 4350 6050 50  0001 C CNN
F 4 "X5R/X7R" H 4465 5959 50  0000 L CNN "Coeff"
F 5 "1276-2728-1-ND" H 4350 6050 50  0001 C CNN "Digikey"
F 6 "C90146" H 4350 6050 50  0001 C CNN "LCSC"
	1    4350 6050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C7
U 1 1 5FF65B84
P 4950 6050
F 0 "C7" H 5065 6141 50  0000 L CNN
F 1 "100n" H 5065 6050 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 4988 5900 50  0001 C CNN
F 3 "~" H 4950 6050 50  0001 C CNN
F 4 "X5R/X7R" H 5065 5959 50  0000 L CNN "Coeff"
F 5 "399-C1206C104K5RAC7800CT-ND" H 4950 6050 50  0001 C CNN "Digikey"
F 6 "C730479" H 4950 6050 50  0001 C CNN "LCSC"
	1    4950 6050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 5900 4350 5900
Connection ~ 4350 5900
Wire Wire Line
	4350 5900 4950 5900
Connection ~ 4950 5900
Wire Wire Line
	4950 5900 5500 5900
Wire Wire Line
	5500 5900 5500 6000
Connection ~ 5500 5900
Wire Wire Line
	5500 6300 5500 6350
Wire Wire Line
	5500 6350 5300 6350
Wire Wire Line
	5300 6350 5300 6450
Wire Wire Line
	5300 6450 4200 6450
Connection ~ 5500 6350
Wire Wire Line
	5500 6350 5500 6400
$Comp
L power:GND #PWR09
U 1 1 5FF79C8E
P 4350 6200
F 0 "#PWR09" H 4350 5950 50  0001 C CNN
F 1 "GND" H 4355 6027 50  0000 C CNN
F 2 "" H 4350 6200 50  0001 C CNN
F 3 "" H 4350 6200 50  0001 C CNN
	1    4350 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5FF7A0BA
P 4950 6200
F 0 "#PWR011" H 4950 5950 50  0001 C CNN
F 1 "GND" H 4955 6027 50  0000 C CNN
F 2 "" H 4950 6200 50  0001 C CNN
F 3 "" H 4950 6200 50  0001 C CNN
	1    4950 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 1550 6100 1550
Wire Wire Line
	6100 1550 6100 2250
Wire Wire Line
	6100 2250 6400 2250
Text Label 2950 1900 0    50   ~ 0
~LED_STBY
Text Label 2950 2250 0    50   ~ 0
~LED_CHRG
Wire Wire Line
	4950 1550 4650 1550
Wire Wire Line
	1950 1550 1950 2350
Connection ~ 4950 1550
$Comp
L Device:R R3
U 1 1 5FFD6A7D
P 3050 6050
F 0 "R3" V 2950 6050 50  0000 C CNN
F 1 "10k" V 3050 6050 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 2980 6050 50  0001 C CNN
F 3 "~" H 3050 6050 50  0001 C CNN
F 4 "C132649" H 3050 6050 50  0001 C CNN "LCSC"
F 5 "541-10.0KFCT-ND" H 3050 6050 50  0001 C CNN "Digikey"
	1    3050 6050
	0    1    1    0   
$EndComp
Text HLabel 10000 1050 2    50   Input ~ 0
GND
Text HLabel 10000 900  2    50   Input ~ 0
3V3
$Comp
L power:+3.3V #PWR022
U 1 1 608527C1
P 10000 900
F 0 "#PWR022" H 10000 750 50  0001 C CNN
F 1 "+3.3V" V 10015 1028 50  0000 L CNN
F 2 "" H 10000 900 50  0001 C CNN
F 3 "" H 10000 900 50  0001 C CNN
	1    10000 900 
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR023
U 1 1 6085567F
P 10000 1050
F 0 "#PWR023" H 10000 800 50  0001 C CNN
F 1 "GND" V 10005 922 50  0000 R CNN
F 2 "" H 10000 1050 50  0001 C CNN
F 3 "" H 10000 1050 50  0001 C CNN
	1    10000 1050
	0    1    1    0   
$EndComp
Text Label 9750 1800 3    50   ~ 0
DC_IN
Text Label 4350 1350 2    50   ~ 0
DC_IN
Text Label 3900 3400 0    50   ~ 0
TEMP
Text Label 10200 2450 2    50   ~ 0
BAT+
Text Label 10200 2550 2    50   ~ 0
BAT-
Text Label 10200 2750 2    50   ~ 0
TEMP
Text Label 4300 3250 3    50   ~ 0
BAT+
Text Label 4650 3600 2    50   ~ 0
BAT-
$Comp
L Device:Q_Dual_NMOS_S1G1S2G2D2D2D1D1 Q1
U 1 1 61168484
P 5800 4300
F 0 "Q1" V 6133 4300 50  0000 C CNN
F 1 "DMG9926USD" H 6088 4255 50  0001 L CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 6000 4300 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds31757.pdf" H 6000 4300 50  0001 C CNN
F 4 "DMG9926USD-13DICT-ND" H 5800 4300 50  0001 C CNN "Digikey"
F 5 "C154896" H 5800 4300 50  0001 C CNN "LCSC"
	1    5800 4300
	0    -1   1    0   
$EndComp
$Comp
L Device:Q_Dual_NMOS_S1G1S2G2D2D2D1D1 Q1
U 2 1 611694C8
P 5400 4300
F 0 "Q1" V 5733 4300 50  0000 C CNN
F 1 "DMG9926USD" H 5688 4255 50  0001 L CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 5600 4300 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds31757.pdf" H 5600 4300 50  0001 C CNN
F 4 "C154896" H 5400 4300 50  0001 C CNN "LCSC"
F 5 "DMG9926USD-13DICT-ND" H 5400 4300 50  0001 C CNN "Digikey"
	2    5400 4300
	0    1    1    0   
$EndComp
Wire Wire Line
	2300 3900 2400 3900
Wire Wire Line
	2400 4000 2400 3900
Connection ~ 2400 3900
Wire Wire Line
	2400 3900 2500 3900
Wire Wire Line
	2900 6050 2900 5900
Wire Wire Line
	2900 5900 3300 5900
Wire Wire Line
	2500 6000 2500 5900
Wire Wire Line
	2500 5900 2900 5900
Connection ~ 2900 5900
Wire Wire Line
	1900 6000 1900 5900
Wire Wire Line
	1900 5900 2500 5900
Connection ~ 2500 5900
Connection ~ 1900 5900
Text HLabel 10000 750  2    50   Input ~ 0
V+
Wire Wire Line
	5100 3500 5100 3600
Connection ~ 5100 3500
$Comp
L Connector:Conn_01x02_Male J13
U 1 1 615A0A11
P 3500 2050
F 0 "J13" H 3608 2231 50  0000 C CNN
F 1 "LED" H 3608 2140 50  0000 C CNN
F 2 "TestPoint:TestPoint_2Pads_Pitch2.54mm_Drill0.8mm" H 3500 2050 50  0001 C CNN
F 3 "~" H 3500 2050 50  0001 C CNN
F 4 "" H 3500 2050 50  0001 C CNN "LCSC"
	1    3500 2050
	1    0    0    -1  
$EndComp
Text Label 3700 2050 0    50   ~ 0
~LED_CHRG
Text Label 3700 2150 0    50   ~ 0
~LED_STBY
Wire Wire Line
	5600 4500 5600 4400
Connection ~ 5600 4500
Connection ~ 5600 4400
$Comp
L power_custom:V+ #PWR043
U 1 1 606123E1
P 10000 750
F 0 "#PWR043" H 10000 600 50  0001 C CNN
F 1 "V+" V 10015 877 50  0000 L CNN
F 2 "" H 10000 750 50  0001 C CNN
F 3 "" H 10000 750 50  0001 C CNN
	1    10000 750 
	0    -1   -1   0   
$EndComp
$Comp
L power_custom:V+ #PWR040
U 1 1 60737CD0
P 1900 5900
F 0 "#PWR040" H 1900 5750 50  0001 C CNN
F 1 "V+" H 1915 6073 50  0000 C CNN
F 2 "" H 1900 5900 50  0001 C CNN
F 3 "" H 1900 5900 50  0001 C CNN
	1    1900 5900
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D7
U 1 1 6074B9C7
P 2800 1900
F 0 "D7" V 2839 1782 50  0000 R CNN
F 1 "LED" V 2748 1782 50  0000 R CNN
F 2 "LED_SMD:LED_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2800 1900 50  0001 C CNN
F 3 "~" H 2800 1900 50  0001 C CNN
F 4 "732-5032-1-ND" H 2800 1900 50  0001 C CNN "Digikey"
F 5 "C125085" H 2800 1900 50  0001 C CNN "LCSC"
	1    2800 1900
	-1   0    0    1   
$EndComp
$Comp
L Device:LED D8
U 1 1 6074D4E3
P 2800 2250
F 0 "D8" V 2839 2132 50  0000 R CNN
F 1 "LED" V 2748 2132 50  0000 R CNN
F 2 "LED_SMD:LED_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2800 2250 50  0001 C CNN
F 3 "~" H 2800 2250 50  0001 C CNN
F 4 "732-5034-1-ND" H 2800 2250 50  0001 C CNN "Digikey"
F 5 "C125087" H 2800 2250 50  0001 C CNN "LCSC"
	1    2800 2250
	-1   0    0    1   
$EndComp
$Comp
L power_custom:V+ #PWR042
U 1 1 60759AE7
P 8200 2850
F 0 "#PWR042" H 8200 2700 50  0001 C CNN
F 1 "V+" H 8215 3023 50  0000 C CNN
F 2 "" H 8200 2850 50  0001 C CNN
F 3 "" H 8200 2850 50  0001 C CNN
	1    8200 2850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R41
U 1 1 6075C3D3
P 2500 2250
F 0 "R41" H 2570 2296 50  0000 L CNN
F 1 "1k" H 2570 2205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 2430 2250 50  0001 C CNN
F 3 "~" H 2500 2250 50  0001 C CNN
F 4 "C131398" H 2500 2250 50  0001 C CNN "LCSC"
F 5 "311-1.00KFRCT-ND" H 2500 2250 50  0001 C CNN "Digikey"
	1    2500 2250
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R40
U 1 1 6075C7CC
P 2500 1900
F 0 "R40" H 2570 1946 50  0000 L CNN
F 1 "1k" H 2570 1855 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 2430 1900 50  0001 C CNN
F 3 "~" H 2500 1900 50  0001 C CNN
F 4 "C131398" H 2500 1900 50  0001 C CNN "LCSC"
F 5 "311-1.00KFRCT-ND" H 2500 1900 50  0001 C CNN "Digikey"
	1    2500 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2350 2250 2350 2100
Wire Wire Line
	2300 2100 2350 2100
Connection ~ 2350 2100
Wire Wire Line
	2350 2100 2350 1900
$Comp
L Connector:Screw_Terminal_01x02 J1
U 1 1 6076AE2C
P 9200 1650
F 0 "J1" H 9280 1642 50  0000 L CNN
F 1 "Screw_Terminal_01x02" H 9280 1551 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 9200 1650 50  0001 C CNN
F 3 "~" H 9200 1650 50  0001 C CNN
F 4 "277-16667-ND" H 9200 1650 50  0001 C CNN "Digikey"
F 5 "C8404" H 9200 1650 50  0001 C CNN "LCSC"
	1    9200 1650
	-1   0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_3_Open JP2
U 1 1 6077B8DF
P 9750 1650
F 0 "JP2" H 9750 1874 50  0000 C CNN
F 1 "Jumper_3_Open" H 9750 1783 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 9750 1650 50  0001 C CNN
F 3 "~" H 9750 1650 50  0001 C CNN
	1    9750 1650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9500 1650 9400 1650
Text HLabel 10000 1650 2    50   Input ~ 0
+5V
$Comp
L Device:C C21
U 1 1 608541F9
P 3800 3550
F 0 "C21" H 3915 3596 50  0000 L CNN
F 1 "100n" H 3915 3505 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.33x1.80mm_HandSolder" H 3838 3400 50  0001 C CNN
F 3 "~" H 3800 3550 50  0001 C CNN
F 4 "399-C1206C104K5RAC7800CT-ND" H 3800 3550 50  0001 C CNN "Digikey"
F 5 "C730479" H 3800 3550 50  0001 C CNN "LCSC"
	1    3800 3550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR041
U 1 1 608548C0
P 3800 3700
F 0 "#PWR041" H 3800 3450 50  0001 C CNN
F 1 "GND" H 3805 3527 50  0000 C CNN
F 2 "" H 3800 3700 50  0001 C CNN
F 3 "" H 3800 3700 50  0001 C CNN
	1    3800 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 3400 3800 3400
Text Notes 9700 2850 0    50   ~ 0
THERMISTOR:NXRT15XH103FA1B040
Text Label 2300 2100 2    50   ~ 0
DC_IN
Wire Wire Line
	4700 3600 5100 3600
Text Label 4450 2050 2    50   ~ 0
BAT-
Wire Wire Line
	4350 1350 4350 1550
Connection ~ 4350 1550
Wire Wire Line
	4350 1550 1950 1550
Wire Wire Line
	3050 2850 4150 2850
Wire Wire Line
	4700 3200 4700 2850
Connection ~ 4700 3200
Connection ~ 4700 2850
Wire Wire Line
	4700 2850 5550 2850
Wire Wire Line
	2850 3050 2850 2850
Wire Wire Line
	2850 2850 3050 2850
Connection ~ 3050 2850
Wire Wire Line
	3800 3400 3900 3400
Connection ~ 3800 3400
$Comp
L Connector:Conn_01x04_Male J2
U 1 1 60A77CF1
P 10400 2550
F 0 "J2" H 10372 2524 50  0000 R CNN
F 1 "Conn_01x04_Male" H 10372 2433 50  0000 R CNN
F 2 "Connector_JST:JST_XH_S4B-XH-A-1_1x04_P2.50mm_Horizontal" H 10400 2550 50  0001 C CNN
F 3 "~" H 10400 2550 50  0001 C CNN
F 4 "455-4227-ND" H 10400 2550 50  0001 C CNN "Digikey"
F 5 "C163037" H 10400 2550 50  0001 C CNN "LCSC"
	1    10400 2550
	-1   0    0    -1  
$EndComp
$Comp
L Device:Q_PMOS_DGS Q6
U 1 1 601ED01B
P 8000 2950
F 0 "Q6" V 8250 2950 50  0000 C CNN
F 1 "IRF7410TRPBF" H 8205 2905 50  0001 L CNN
F 2 "transistor:SO8_3.8x4.8_SSSGDDDD" H 8200 3050 50  0001 C CNN
F 3 "https://www.infineon.com/dgdl/irf7410pbf.pdf?fileId=5546d462533600a4015355fa75d71bb0" H 8000 2950 50  0001 C CNN
F 4 "IRF7410TRPBF" V 8000 2950 50  0001 C CNN "Digikey"
F 5 "C148144" V 8000 2950 50  0001 C CNN "LCSC"
	1    8000 2950
	0    1    -1   0   
$EndComp
$Comp
L Device:R R66
U 1 1 601EEC7C
P 7600 3000
F 0 "R66" H 7670 3046 50  0000 L CNN
F 1 "10k" H 7670 2955 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7530 3000 50  0001 C CNN
F 3 "~" H 7600 3000 50  0001 C CNN
F 4 "C132649" H 7600 3000 50  0001 C CNN "LCSC"
F 5 "541-10.0KFCT-ND" H 7600 3000 50  0001 C CNN "Digikey"
	1    7600 3000
	1    0    0    -1  
$EndComp
Connection ~ 7600 2850
Wire Wire Line
	7600 2850 7100 2850
Wire Wire Line
	7600 3150 8000 3150
$Comp
L Switch:SW_DIP_x01 SW2
U 1 1 601F41AD
P 8000 3450
F 0 "SW2" V 8046 3320 50  0000 R CNN
F 1 "SW_DIP_x01" V 7955 3320 50  0000 R CNN
F 2 "Button_Switch_THT:SW_DIP_SPSTx01_Slide_9.78x4.72mm_W7.62mm_P2.54mm" H 8000 3450 50  0001 C CNN
F 3 "~" H 8000 3450 50  0001 C CNN
F 4 "C229812" H 8000 3450 50  0001 C CNN "LCSC"
F 5 "2223-DS01-254-L-01BE-ND" H 8000 3450 50  0001 C CNN "Digikey"
	1    8000 3450
	0    -1   -1   0   
$EndComp
Connection ~ 8000 3150
$Comp
L power:GND #PWR0139
U 1 1 601FA462
P 8000 3750
F 0 "#PWR0139" H 8000 3500 50  0001 C CNN
F 1 "GND" H 8005 3577 50  0000 C CNN
F 2 "" H 8000 3750 50  0001 C CNN
F 3 "" H 8000 3750 50  0001 C CNN
	1    8000 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R65
U 1 1 601FE3E6
P 7400 3350
F 0 "R65" H 7470 3396 50  0000 L CNN
F 1 "1k" H 7470 3305 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7330 3350 50  0001 C CNN
F 3 "~" H 7400 3350 50  0001 C CNN
F 4 "C131398" H 7400 3350 50  0001 C CNN "LCSC"
F 5 "311-1.00KFRCT-ND" H 7400 3350 50  0001 C CNN "Digikey"
	1    7400 3350
	0    1    1    0   
$EndComp
$Comp
L Device:LED D11
U 1 1 60206B62
P 7100 3350
F 0 "D11" H 7093 3095 50  0000 C CNN
F 1 "LED" H 7093 3186 50  0000 C CNN
F 2 "LED_SMD:LED_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7100 3350 50  0001 C CNN
F 3 "~" H 7100 3350 50  0001 C CNN
F 4 "732-5032-1-ND" H 7100 3350 50  0001 C CNN "Digikey"
F 5 "C125085" H 7100 3350 50  0001 C CNN "LCSC"
	1    7100 3350
	-1   0    0    1   
$EndComp
Wire Wire Line
	7550 3350 7600 3350
Wire Wire Line
	7600 3350 7600 3150
Connection ~ 7600 3150
Wire Wire Line
	6950 3350 6950 2850
Connection ~ 6950 2850
Wire Wire Line
	6950 2850 6850 2850
$Comp
L Connector:TestPoint TP3
U 1 1 60460998
P 4700 2850
F 0 "TP3" H 4758 2968 50  0000 L CNN
F 1 "BAT+" H 4758 2877 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 4900 2850 50  0001 C CNN
F 3 "~" H 4900 2850 50  0001 C CNN
	1    4700 2850
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint_2Pole TP2
U 1 1 604701EC
P 4300 3050
F 0 "TP2" V 4254 3108 50  0000 L CNN
F 1 "IBAT" V 4345 3108 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4300 3050 50  0001 C CNN
F 3 "~" H 4300 3050 50  0001 C CNN
	1    4300 3050
	0    1    1    0   
$EndComp
Connection ~ 4300 2850
Wire Wire Line
	4300 2850 4700 2850
Wire Wire Line
	4450 1900 4150 1900
Wire Wire Line
	4150 1900 4150 2850
Connection ~ 4150 2850
Wire Wire Line
	4150 2850 4300 2850
$Comp
L Connector:TestPoint TP5
U 1 1 60498C4B
P 4650 1550
F 0 "TP5" H 4708 1668 50  0000 L CNN
F 1 "DC" H 4708 1577 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 4850 1550 50  0001 C CNN
F 3 "~" H 4850 1550 50  0001 C CNN
	1    4650 1550
	1    0    0    -1  
$EndComp
Connection ~ 4650 1550
Wire Wire Line
	4650 1550 4350 1550
Text Label 10000 650  2    50   ~ 0
DC_IN
Text HLabel 10000 650  2    50   Input ~ 0
DC_IN
$Comp
L Connector:TestPoint TP1
U 1 1 5FFECFD2
P 5400 4100
F 0 "TP1" H 5458 4218 50  0000 L CNN
F 1 "DIS" H 5458 4127 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 5600 4100 50  0001 C CNN
F 3 "~" H 5600 4100 50  0001 C CNN
	1    5400 4100
	0    -1   -1   0   
$EndComp
Connection ~ 5400 4100
$Comp
L Connector:TestPoint TP4
U 1 1 5FFF01C3
P 5800 4100
F 0 "TP4" H 5858 4218 50  0000 L CNN
F 1 "CHRG" H 5858 4127 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 6000 4100 50  0001 C CNN
F 3 "~" H 6000 4100 50  0001 C CNN
	1    5800 4100
	0    -1   -1   0   
$EndComp
Connection ~ 5800 4100
$Comp
L Connector:TestPoint TP6
U 1 1 6002B2F3
P 4700 3950
F 0 "TP6" H 4758 4068 50  0000 L CNN
F 1 "BAT-" H 4758 3977 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 4900 3950 50  0001 C CNN
F 3 "~" H 4900 3950 50  0001 C CNN
	1    4700 3950
	0    -1   -1   0   
$EndComp
Connection ~ 4700 3950
Wire Wire Line
	4700 3950 4700 3600
$EndSCHEMATC
