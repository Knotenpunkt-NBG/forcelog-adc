EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
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
L conn:CONN_01X01 P?
U 1 1 5E7A2949
P 3000 6900
AR Path="/5E7A2949" Ref="P?"  Part="1" 
AR Path="/5E766EDC/5E7A2949" Ref="P15"  Part="1" 
F 0 "P15" H 3078 6941 50  0000 L CNN
F 1 "SHIELD" H 3078 6850 50  0000 L CNN
F 2 "custom:Shield_TE2118726-2" H 3000 6900 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/te-connectivity-amp-connectors/2118726-2/A126157CT-ND/7381917" H 3000 6900 50  0001 C CNN
	1    3000 6900
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2955
P 3000 7100
AR Path="/5E7A2955" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2955" Ref="#PWR080"  Part="1" 
F 0 "#PWR080" H 3000 6850 50  0001 C CNN
F 1 "GNDA" H 3005 6927 50  0000 C CNN
F 2 "" H 3000 7100 50  0001 C CNN
F 3 "" H 3000 7100 50  0001 C CNN
	1    3000 7100
	1    0    0    -1  
$EndComp
$Comp
L conn:CONN_01X01 P?
U 1 1 5E7A2BB9
P 3350 6900
AR Path="/5E7A2BB9" Ref="P?"  Part="1" 
AR Path="/5E766EDC/5E7A2BB9" Ref="P16"  Part="1" 
F 0 "P16" H 3428 6941 50  0000 L CNN
F 1 "SHIELD" H 3428 6850 50  0000 L CNN
F 2 "custom:Shield_TE2118726-2" H 3350 6900 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/te-connectivity-amp-connectors/2118726-2/A126157CT-ND/7381917" H 3350 6900 50  0001 C CNN
	1    3350 6900
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2BBF
P 3350 7100
AR Path="/5E7A2BBF" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2BBF" Ref="#PWR087"  Part="1" 
F 0 "#PWR087" H 3350 6850 50  0001 C CNN
F 1 "GNDA" H 3355 6927 50  0000 C CNN
F 2 "" H 3350 7100 50  0001 C CNN
F 3 "" H 3350 7100 50  0001 C CNN
	1    3350 7100
	1    0    0    -1  
$EndComp
Wire Notes Line
	800  650  10500 650 
Wire Notes Line
	10500 650  10500 6200
Wire Notes Line
	10500 6200 4700 6200
Wire Notes Line
	4700 6200 4700 7050
Wire Notes Line
	4700 7050 800  7050
Wire Notes Line
	800  7050 800  650 
Text Label 3050 4000 2    50   ~ 0
EX-
Text Label 3050 4100 2    50   ~ 0
EX+
$Comp
L power:GNDA #PWR?
U 1 1 5E7A294F
P 2600 3700
AR Path="/5E7A294F" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A294F" Ref="#PWR074"  Part="1" 
F 0 "#PWR074" H 2600 3450 50  0001 C CNN
F 1 "GNDA" H 2605 3527 50  0000 C CNN
F 2 "" H 2600 3700 50  0001 C CNN
F 3 "" H 2600 3700 50  0001 C CNN
	1    2600 3700
	1    0    0    1   
$EndComp
Wire Wire Line
	2750 3800 2600 3800
Wire Wire Line
	2750 4200 2750 3800
Wire Wire Line
	2900 4200 2900 3900
$Comp
L conn:CONN_01X01 P?
U 1 1 5E80A5BE
P 2750 4400
AR Path="/5E80A5BE" Ref="P?"  Part="1" 
AR Path="/5E766EDC/5E80A5BE" Ref="P13"  Part="1" 
F 0 "P13" H 2828 4441 50  0000 L CNN
F 1 "TP_A+" H 2828 4350 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 2750 4400 50  0001 C CNN
F 3 "" H 2750 4400 50  0000 C CNN
	1    2750 4400
	0    1    1    0   
$EndComp
$Comp
L conn:CONN_01X01 P?
U 1 1 5E7A2937
P 2900 4400
AR Path="/5E7A2937" Ref="P?"  Part="1" 
AR Path="/5E766EDC/5E7A2937" Ref="P14"  Part="1" 
F 0 "P14" H 2978 4441 50  0000 L CNN
F 1 "TP_A-" H 2978 4350 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 2900 4400 50  0001 C CNN
F 3 "" H 2900 4400 50  0000 C CNN
	1    2900 4400
	0    1    1    0   
$EndComp
Wire Wire Line
	3050 4100 2600 4100
Wire Wire Line
	2600 4000 3050 4000
Wire Wire Line
	2600 3900 2900 3900
$Comp
L conn:CONN_01X05 P?
U 1 1 5E7A2916
P 2400 3900
AR Path="/5E7A2916" Ref="P?"  Part="1" 
AR Path="/5E766EDC/5E7A2916" Ref="P12"  Part="1" 
F 0 "P12" H 2319 4315 50  0000 C CNN
F 1 "CELL" H 2319 4224 50  0000 C CNN
F 2 "Connector_JST:JST_XH_S5B-XH-A_1x05_P2.50mm_Horizontal" H 2400 3900 50  0001 C CNN
F 3 "https://www.reichelt.de/jst-stiftleiste-90-1x5-polig-xh-jst-xh5p-st90-p185082.html?&trstct=pos_0&nbc=1" H 2400 3900 50  0001 C CNN
	1    2400 3900
	-1   0    0    1   
$EndComp
$Comp
L jumper:Jumper_2_Bridged JP?
U 1 1 5E7A2973
P 2700 2000
AR Path="/5E7A2973" Ref="JP?"  Part="1" 
AR Path="/5E766EDC/5E7A2973" Ref="JP9"  Part="1" 
F 0 "JP9" H 2700 2195 50  0000 C CNN
F 1 "Jumper_2_Bridged" H 2700 2104 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Bridged2Bar_Pad1.0x1.5mm" H 2700 2000 50  0001 C CNN
F 3 "~" H 2700 2000 50  0001 C CNN
	1    2700 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E7A296D
P 2500 2000
AR Path="/5E7A296D" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A296D" Ref="#PWR073"  Part="1" 
F 0 "#PWR073" H 2500 1750 50  0001 C CNN
F 1 "GND" H 2505 1827 50  0000 C CNN
F 2 "" H 2500 2000 50  0000 C CNN
F 3 "" H 2500 2000 50  0000 C CNN
	1    2500 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2967
P 2900 2000
AR Path="/5E7A2967" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2967" Ref="#PWR075"  Part="1" 
F 0 "#PWR075" H 2900 1750 50  0001 C CNN
F 1 "GNDA" H 2905 1827 50  0000 C CNN
F 2 "" H 2900 2000 50  0001 C CNN
F 3 "" H 2900 2000 50  0001 C CNN
	1    2900 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2961
P 2900 3000
AR Path="/5E7A2961" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2961" Ref="#PWR0100"  Part="1" 
F 0 "#PWR0100" H 2900 2750 50  0001 C CNN
F 1 "GNDA" H 2905 2827 50  0000 C CNN
F 2 "" H 2900 3000 50  0001 C CNN
F 3 "" H 2900 3000 50  0001 C CNN
	1    2900 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A295B
P 2450 3000
AR Path="/5E7A295B" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A295B" Ref="#PWR098"  Part="1" 
F 0 "#PWR098" H 2450 2750 50  0001 C CNN
F 1 "GNDA" H 2455 2827 50  0000 C CNN
F 2 "" H 2450 3000 50  0001 C CNN
F 3 "" H 2450 3000 50  0001 C CNN
	1    2450 3000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5E7A292E
P 2450 2700
AR Path="/5E7A292E" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A292E" Ref="#PWR097"  Part="1" 
F 0 "#PWR097" H 2450 2550 50  0001 C CNN
F 1 "+5V" H 2450 2850 50  0000 C CNN
F 2 "" H 2450 2700 50  0000 C CNN
F 3 "" H 2450 2700 50  0000 C CNN
	1    2450 2700
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5E7A2928
P 2900 2700
AR Path="/5E7A2928" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2928" Ref="#PWR099"  Part="1" 
F 0 "#PWR099" H 2900 2550 50  0001 C CNN
F 1 "+5V" H 2900 2850 50  0000 C CNN
F 2 "" H 2900 2700 50  0000 C CNN
F 3 "" H 2900 2700 50  0000 C CNN
	1    2900 2700
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E7A2922
P 2450 2850
AR Path="/5E7A2922" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2922" Ref="C25"  Part="1" 
F 0 "C25" H 2565 2896 50  0000 L CNN
F 1 "100n" H 2565 2805 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2488 2700 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 2450 2850 50  0001 C CNN
	1    2450 2850
	1    0    0    -1  
$EndComp
$Comp
L device:CP C?
U 1 1 5E7A291C
P 2900 2850
AR Path="/5E7A291C" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A291C" Ref="C26"  Part="1" 
F 0 "C26" H 3018 2896 50  0000 L CNN
F 1 "10µ" H 3018 2805 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.7" H 2938 2700 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-elko-10-f-50-v-2000-h-eln-rv2-50v100mu-p246741.html?&trstct=pol_2&nbc=1" H 2900 2850 50  0001 C CNN
	1    2900 2850
	1    0    0    -1  
$EndComp
$Comp
L modules:HX711 U9
U 1 1 5E789763
P 4550 3500
F 0 "U9" H 4300 3550 50  0000 C CNN
F 1 "HX711" H 4750 3550 50  0000 C CNN
F 2 "buck:hx711" H 4450 3500 50  0001 C CNN
F 3 "https://www.banggood.com/3pcs-AD-Weighing-Sensor-Module-Dual-channel-24-bit-AD-Conversion-HX711-Shieding-p-1355706.html?rmmds=search&cur_warehouse=CN" H 4450 3500 50  0001 C CNN
	1    4550 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 3900 4050 3900
Connection ~ 2900 3900
Wire Wire Line
	2750 3800 4050 3800
Connection ~ 2750 3800
Text Label 4050 3600 2    50   ~ 0
EX+
Text Label 4050 3700 2    50   ~ 0
EX-
NoConn ~ 4050 4000
NoConn ~ 4050 4100
$Comp
L conn:CONN_01X01 P?
U 1 1 5E7901C7
P 7000 2300
AR Path="/5E7901C7" Ref="P?"  Part="1" 
AR Path="/5E766EDC/5E7901C7" Ref="P21"  Part="1" 
F 0 "P21" H 7078 2341 50  0000 L CNN
F 1 "MOD_RATE" H 7078 2250 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 7000 2300 50  0001 C CNN
F 3 "" H 7000 2300 50  0000 C CNN
	1    7000 2300
	1    0    0    -1  
$EndComp
$Comp
L device:R R8
U 1 1 5E7920CC
P 6800 2050
F 0 "R8" H 6870 2096 50  0000 L CNN
F 1 "10k" H 6870 2005 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 6730 2050 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-10-kohm-250-mw-1-rnd-1206-1-10k-p183394.html?&trstct=pol_1&nbc=1" H 6800 2050 50  0001 C CNN
	1    6800 2050
	1    0    0    -1  
$EndComp
$Comp
L device:R R9
U 1 1 5E7927EA
P 7250 2800
F 0 "R9" V 7043 2800 50  0000 C CNN
F 1 "10k" V 7134 2800 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7180 2800 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-100-ohm-250-mw-1-rnd-1206-1-100-p183349.html?&trstct=pol_1&nbc=1" H 7250 2800 50  0001 C CNN
	1    7250 2800
	-1   0    0    1   
$EndComp
Wire Wire Line
	6800 2200 6800 2300
Connection ~ 6800 2300
Wire Wire Line
	6800 2300 6800 2450
$Comp
L power:+5V #PWR?
U 1 1 5E793FC4
P 4550 3300
AR Path="/5E793FC4" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E793FC4" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 4550 3150 50  0001 C CNN
F 1 "+5V" H 4550 3450 50  0000 C CNN
F 2 "" H 4550 3300 50  0000 C CNN
F 3 "" H 4550 3300 50  0000 C CNN
	1    4550 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E795539
P 4550 4400
AR Path="/5E795539" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E795539" Ref="#PWR0103"  Part="1" 
F 0 "#PWR0103" H 4550 4150 50  0001 C CNN
F 1 "GNDA" H 4555 4227 50  0000 C CNN
F 2 "" H 4550 4400 50  0001 C CNN
F 3 "" H 4550 4400 50  0001 C CNN
	1    4550 4400
	1    0    0    -1  
$EndComp
Text HLabel 7550 2650 2    50   Input ~ 0
RATE
Wire Wire Line
	7250 2650 7100 2650
$Comp
L power:GNDA #PWR?
U 1 1 5E799483
P 6800 2850
AR Path="/5E799483" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E799483" Ref="#PWR0104"  Part="1" 
F 0 "#PWR0104" H 6800 2600 50  0001 C CNN
F 1 "GNDA" H 6805 2677 50  0000 C CNN
F 2 "" H 6800 2850 50  0001 C CNN
F 3 "" H 6800 2850 50  0001 C CNN
	1    6800 2850
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5E79C29F
P 6800 1900
AR Path="/5E79C29F" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E79C29F" Ref="#PWR0105"  Part="1" 
F 0 "#PWR0105" H 6800 1750 50  0001 C CNN
F 1 "+5V" H 6800 2050 50  0000 C CNN
F 2 "" H 6800 1900 50  0000 C CNN
F 3 "" H 6800 1900 50  0000 C CNN
	1    6800 1900
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2929
P 2000 3000
AR Path="/5E7A2929" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2929" Ref="#PWR0106"  Part="1" 
F 0 "#PWR0106" H 2000 2750 50  0001 C CNN
F 1 "GNDA" H 2005 2827 50  0000 C CNN
F 2 "" H 2000 3000 50  0001 C CNN
F 3 "" H 2000 3000 50  0001 C CNN
	1    2000 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2933
P 1550 3000
AR Path="/5E7A2933" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2933" Ref="#PWR0107"  Part="1" 
F 0 "#PWR0107" H 1550 2750 50  0001 C CNN
F 1 "GNDA" H 1555 2827 50  0000 C CNN
F 2 "" H 1550 3000 50  0001 C CNN
F 3 "" H 1550 3000 50  0001 C CNN
	1    1550 3000
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5E7A293D
P 1550 2700
AR Path="/5E7A293D" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A293D" Ref="#PWR0108"  Part="1" 
F 0 "#PWR0108" H 1550 2550 50  0001 C CNN
F 1 "+5V" H 1550 2850 50  0000 C CNN
F 2 "" H 1550 2700 50  0000 C CNN
F 3 "" H 1550 2700 50  0000 C CNN
	1    1550 2700
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5E7A2947
P 2000 2700
AR Path="/5E7A2947" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2947" Ref="#PWR0109"  Part="1" 
F 0 "#PWR0109" H 2000 2550 50  0001 C CNN
F 1 "+5V" H 2000 2850 50  0000 C CNN
F 2 "" H 2000 2700 50  0000 C CNN
F 3 "" H 2000 2700 50  0000 C CNN
	1    2000 2700
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E7A2951
P 1550 2850
AR Path="/5E7A2951" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2951" Ref="C15"  Part="1" 
F 0 "C15" H 1665 2896 50  0000 L CNN
F 1 "100n" H 1665 2805 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 1588 2700 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 1550 2850 50  0001 C CNN
	1    1550 2850
	1    0    0    -1  
$EndComp
$Comp
L device:CP C?
U 1 1 5E80A5BF
P 2000 2850
AR Path="/5E80A5BF" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E80A5BF" Ref="C16"  Part="1" 
F 0 "C16" H 2118 2896 50  0000 L CNN
F 1 "10µ" H 2118 2805 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.7" H 2038 2700 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-elko-10-f-50-v-2000-h-eln-rv2-50v100mu-p246741.html?&trstct=pol_2&nbc=1" H 2000 2850 50  0001 C CNN
	1    2000 2850
	1    0    0    -1  
$EndComp
Text HLabel 5050 3750 2    50   Input ~ 0
SCK
Text HLabel 5050 3900 2    50   Input ~ 0
DT
Text HLabel 3450 2950 1    50   Input ~ 0
GND
Text HLabel 3650 2950 1    50   Input ~ 0
+5V
$Comp
L power:+5V #PWR?
U 1 1 5E7D20AD
P 3650 2950
AR Path="/5E7D20AD" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7D20AD" Ref="#PWR0110"  Part="1" 
F 0 "#PWR0110" H 3650 2800 50  0001 C CNN
F 1 "+5V" H 3650 3100 50  0000 C CNN
F 2 "" H 3650 2950 50  0000 C CNN
F 3 "" H 3650 2950 50  0000 C CNN
	1    3650 2950
	-1   0    0    1   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7D271B
P 3450 2950
AR Path="/5E7D271B" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7D271B" Ref="#PWR0111"  Part="1" 
F 0 "#PWR0111" H 3450 2700 50  0001 C CNN
F 1 "GNDA" H 3455 2777 50  0000 C CNN
F 2 "" H 3450 2950 50  0001 C CNN
F 3 "" H 3450 2950 50  0001 C CNN
	1    3450 2950
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E81FDB2
P 3300 4500
AR Path="/5E81FDB2" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E81FDB2" Ref="C17"  Part="1" 
F 0 "C17" H 3415 4546 50  0000 L CNN
F 1 "100n" H 3415 4455 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 3338 4350 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 3300 4500 50  0001 C CNN
	1    3300 4500
	1    0    0    -1  
$EndComp
$Comp
L device:CP C?
U 1 1 5E81FDBC
P 3750 4500
AR Path="/5E81FDBC" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E81FDBC" Ref="C18"  Part="1" 
F 0 "C18" H 3868 4546 50  0000 L CNN
F 1 "10µ" H 3868 4455 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.7" H 3788 4350 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-elko-10-f-50-v-2000-h-eln-rv2-50v100mu-p246741.html?&trstct=pol_2&nbc=1" H 3750 4500 50  0001 C CNN
	1    3750 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 4350 3750 4350
Wire Wire Line
	3300 4650 3750 4650
Text Label 3600 4350 2    50   ~ 0
EX+
Text Label 3600 4650 2    50   ~ 0
EX-
$Comp
L device:Q_NMOS_GSD Q?
U 1 1 5E85D98E
P 6900 2650
AR Path="/5E85D98E" Ref="Q?"  Part="1" 
AR Path="/5E766EDC/5E85D98E" Ref="Q1"  Part="1" 
F 0 "Q1" H 7091 2696 50  0000 L CNN
F 1 "2N7002" H 7091 2605 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 7100 2750 50  0001 C CNN
F 3 "" H 6900 2650 50  0000 C CNN
	1    6900 2650
	-1   0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E79CC44
P 7250 2950
AR Path="/5E79CC44" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E79CC44" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7250 2700 50  0001 C CNN
F 1 "GNDA" H 7255 2777 50  0000 C CNN
F 2 "" H 7250 2950 50  0001 C CNN
F 3 "" H 7250 2950 50  0001 C CNN
	1    7250 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 2650 7550 2650
Connection ~ 7250 2650
$EndSCHEMATC
