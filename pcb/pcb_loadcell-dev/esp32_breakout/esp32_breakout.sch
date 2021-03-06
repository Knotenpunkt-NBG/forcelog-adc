EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L rf-modules:ESP32-WROOM-32 U1
U 1 1 5E6FAE82
P 3600 3600
F 0 "U1" H 3600 5181 50  0000 C CNN
F 1 "ESP32-WROOM-32" H 3600 5090 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 3600 2100 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/espressif-systems/ESP32-WROOM-32-16MB/1904-1020-1-ND/9381729" H 3300 3650 50  0001 C CNN
	1    3600 3600
	1    0    0    -1  
$EndComp
$Comp
L device:C C2
U 1 1 5E70D46D
P 4500 1350
F 0 "C2" H 4615 1396 50  0000 L CNN
F 1 "100n" H 4615 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4538 1200 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 4500 1350 50  0001 C CNN
	1    4500 1350
	1    0    0    -1  
$EndComp
$Comp
L device:CP C3
U 1 1 5E70DA10
P 4900 1350
F 0 "C3" H 5018 1396 50  0000 L CNN
F 1 "10µ" H 5018 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4938 1200 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-10-f-16v-125-c-kem-x7r1206-10u-p207163.html?&trstct=pol_0&nbc=1" H 4900 1350 50  0001 C CNN
	1    4900 1350
	1    0    0    -1  
$EndComp
$Comp
L device:C C1
U 1 1 5E70DEF9
P 4050 1350
F 0 "C1" H 4165 1396 50  0000 L CNN
F 1 "100n" H 4165 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4088 1200 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 4050 1350 50  0001 C CNN
	1    4050 1350
	1    0    0    -1  
$EndComp
$Comp
L device:CP C4
U 1 1 5E70E242
P 5300 1350
F 0 "C4" H 5418 1396 50  0000 L CNN
F 1 "10µ" H 5418 1305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5338 1200 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-10-f-16v-125-c-kem-x7r1206-10u-p207163.html?&trstct=pol_0&nbc=1" H 5300 1350 50  0001 C CNN
	1    5300 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 1500 4900 1500
Connection ~ 4500 1500
Wire Wire Line
	4500 1500 4050 1500
Connection ~ 4900 1500
Wire Wire Line
	4050 1200 4500 1200
Connection ~ 4500 1200
Wire Wire Line
	4500 1200 4700 1200
Connection ~ 4900 1200
Wire Wire Line
	4900 1200 5300 1200
$Comp
L power:+3.3V #PWR03
U 1 1 5E70EFC1
P 4700 1200
F 0 "#PWR03" H 4700 1050 50  0001 C CNN
F 1 "+3.3V" H 4715 1373 50  0000 C CNN
F 2 "" H 4700 1200 50  0001 C CNN
F 3 "" H 4700 1200 50  0001 C CNN
	1    4700 1200
	1    0    0    -1  
$EndComp
Connection ~ 4700 1200
Wire Wire Line
	4700 1200 4900 1200
$Comp
L power:GND #PWR04
U 1 1 5E70F4C2
P 5300 2550
F 0 "#PWR04" H 5300 2300 50  0001 C CNN
F 1 "GND" H 5305 2377 50  0000 C CNN
F 2 "" H 5300 2550 50  0001 C CNN
F 3 "" H 5300 2550 50  0001 C CNN
	1    5300 2550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR01
U 1 1 5E70F72B
P 3600 2200
F 0 "#PWR01" H 3600 2050 50  0001 C CNN
F 1 "+3.3V" H 3615 2373 50  0000 C CNN
F 2 "" H 3600 2200 50  0001 C CNN
F 3 "" H 3600 2200 50  0001 C CNN
	1    3600 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5E70FBCD
P 3600 5000
F 0 "#PWR02" H 3600 4750 50  0001 C CNN
F 1 "GND" H 3605 4827 50  0000 C CNN
F 2 "" H 3600 5000 50  0001 C CNN
F 3 "" H 3600 5000 50  0001 C CNN
	1    3600 5000
	1    0    0    -1  
$EndComp
$Comp
L conn:CONN_01X10 P3
U 1 1 5E70D73A
P 7250 5050
F 0 "P3" H 7328 5091 50  0000 L CNN
F 1 "CONN_01X10" H 7328 5000 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 7250 5050 50  0001 C CNN
F 3 "" H 7250 5050 50  0000 C CNN
	1    7250 5050
	0    1    1    0   
$EndComp
$Comp
L conn:CONN_01X14 P1
U 1 1 5E70E2DE
P 6150 3550
F 0 "P1" H 6228 3591 50  0000 L CNN
F 1 "CONN_01X14" H 6228 3500 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x14_P2.54mm_Vertical" H 6150 3550 50  0001 C CNN
F 3 "" H 6150 3550 50  0000 C CNN
	1    6150 3550
	-1   0    0    -1  
$EndComp
$Comp
L conn:CONN_01X14 P2
U 1 1 5E70EAAD
P 8150 3550
F 0 "P2" H 8228 3591 50  0000 L CNN
F 1 "CONN_01X14" H 8228 3500 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x14_P2.54mm_Vertical" H 8150 3550 50  0001 C CNN
F 3 "" H 8150 3550 50  0000 C CNN
	1    8150 3550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5E711144
P 6350 2900
F 0 "#PWR0101" H 6350 2650 50  0001 C CNN
F 1 "GND" H 6355 2727 50  0000 C CNN
F 2 "" H 6350 2900 50  0001 C CNN
F 3 "" H 6350 2900 50  0001 C CNN
	1    6350 2900
	0    -1   1    0   
$EndComp
$Comp
L power:+3.3V #PWR0102
U 1 1 5E71302A
P 6350 3000
F 0 "#PWR0102" H 6350 2850 50  0001 C CNN
F 1 "+3.3V" H 6365 3173 50  0000 C CNN
F 2 "" H 6350 3000 50  0001 C CNN
F 3 "" H 6350 3000 50  0001 C CNN
	1    6350 3000
	0    1    -1   0   
$EndComp
Text Label 6350 3100 0    50   ~ 0
P1_3
Text Label 6350 3200 0    50   ~ 0
P1_4
Text Label 6350 3300 0    50   ~ 0
P1_5
Text Label 6350 3400 0    50   ~ 0
P1_6
Text Label 6350 3500 0    50   ~ 0
P1_7
Text Label 6350 3600 0    50   ~ 0
P1_8
Text Label 6350 3700 0    50   ~ 0
P1_9
Text Label 6350 3800 0    50   ~ 0
P1_10
Text Label 6350 3900 0    50   ~ 0
P1_11
Text Label 6350 4000 0    50   ~ 0
P1_12
Text Label 6350 4100 0    50   ~ 0
P1_13
Text Label 6350 4200 0    50   ~ 0
P1_14
Text Label 7950 2900 2    50   ~ 0
P2_1
Text Label 7950 3000 2    50   ~ 0
P2_2
Text Label 7950 3100 2    50   ~ 0
P2_3
Text Label 7950 3200 2    50   ~ 0
P2_4
Text Label 7950 3300 2    50   ~ 0
P2_5
Text Label 7950 3400 2    50   ~ 0
P2_6
Text Label 7950 3500 2    50   ~ 0
P2_7
Text Label 7950 3700 2    50   ~ 0
P2_9
Text Label 7950 3800 2    50   ~ 0
P2_10
Text Label 7950 3900 2    50   ~ 0
P2_11
Text Label 7950 4000 2    50   ~ 0
P2_12
Text Label 7950 4100 2    50   ~ 0
P2_13
Text Label 7950 4200 2    50   ~ 0
P2_14
Text Label 7700 4850 1    50   ~ 0
P3_1
Text Label 7600 4850 1    50   ~ 0
P3_2
Text Label 7500 4850 1    50   ~ 0
P3_3
Text Label 7400 4850 1    50   ~ 0
P3_4
Text Label 7300 4850 1    50   ~ 0
P3_5
Text Label 7200 4850 1    50   ~ 0
P3_6
Text Label 7100 4850 1    50   ~ 0
P3_7
Text Label 7000 4850 1    50   ~ 0
P3_8
Text Label 6900 4850 1    50   ~ 0
P3_9
Text Label 6800 4850 1    50   ~ 0
P3_10
Text Label 3000 2400 2    50   ~ 0
P1_3
Text Label 3000 2600 2    50   ~ 0
P1_4
Text Label 3000 2700 2    50   ~ 0
P1_5
Text Label 4200 4600 0    50   ~ 0
P1_6
Text Label 4200 4700 0    50   ~ 0
P1_7
Text Label 4200 4400 0    50   ~ 0
P1_8
Text Label 4200 4500 0    50   ~ 0
P1_9
Text Label 4200 4100 0    50   ~ 0
P1_10
Text Label 4200 4200 0    50   ~ 0
P1_11
Text Label 4200 4300 0    50   ~ 0
P1_12
Text Label 4200 3200 0    50   ~ 0
P1_13
Text Label 4200 3000 0    50   ~ 0
P1_14
Text Label 5300 2550 0    50   ~ 0
P3_1
Text Label 4200 3100 0    50   ~ 0
P3_2
Text Label 3000 3800 2    50   ~ 0
P3_3
Text Label 3000 3900 2    50   ~ 0
P3_4
Text Label 3000 4100 2    50   ~ 0
P3_5
Text Label 3000 4000 2    50   ~ 0
P3_6
Text Label 3000 3600 2    50   ~ 0
P3_7
Text Label 3000 3700 2    50   ~ 0
P3_8
Text Label 4200 3300 0    50   ~ 0
P3_9
Text Label 4200 2600 0    50   ~ 0
P3_10
Wire Wire Line
	4500 1500 4700 1500
Text Label 4200 2400 0    50   ~ 0
P2_1
Text Label 4200 2800 0    50   ~ 0
P2_2
Text Label 4200 3400 0    50   ~ 0
P2_3
Text Label 4200 3500 0    50   ~ 0
P2_4
Text Label 4200 2900 0    50   ~ 0
P2_5
Text Label 4200 3600 0    50   ~ 0
P2_6
Text Label 4200 3700 0    50   ~ 0
P2_7
Text Label 4200 3800 0    50   ~ 0
P2_9
Text Label 4200 2700 0    50   ~ 0
P2_10
Text Label 4200 2500 0    50   ~ 0
P2_11
Text Label 4200 3900 0    50   ~ 0
P2_12
Text Label 4200 4000 0    50   ~ 0
P2_13
Text Label 5300 2550 2    50   ~ 0
P2_14
NoConn ~ 7950 3600
$Comp
L power:GND #PWR0103
U 1 1 5E795859
P 4700 1500
F 0 "#PWR0103" H 4700 1250 50  0001 C CNN
F 1 "GND" H 4705 1327 50  0000 C CNN
F 2 "" H 4700 1500 50  0001 C CNN
F 3 "" H 4700 1500 50  0001 C CNN
	1    4700 1500
	1    0    0    -1  
$EndComp
Connection ~ 4700 1500
Wire Wire Line
	4700 1500 4900 1500
$EndSCHEMATC
