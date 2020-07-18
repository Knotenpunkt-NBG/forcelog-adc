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
L conn:CONN_01X05 P?
U 1 1 5E7A2916
P 1200 4800
AR Path="/5E7A2916" Ref="P?"  Part="1" 
AR Path="/5E766EDC/5E7A2916" Ref="P12"  Part="1" 
F 0 "P12" H 1119 5215 50  0000 C CNN
F 1 "CELL" H 1119 5124 50  0000 C CNN
F 2 "Connector_JST:JST_XH_S5B-XH-A_1x05_P2.50mm_Horizontal" H 1200 4800 50  0001 C CNN
F 3 "https://www.reichelt.de/jst-stiftleiste-90-1x5-polig-xh-jst-xh5p-st90-p185082.html?&trstct=pos_0&nbc=1" H 1200 4800 50  0001 C CNN
	1    1200 4800
	-1   0    0    1   
$EndComp
$Comp
L device:CP C?
U 1 1 5E7A291C
P 6650 3250
AR Path="/5E7A291C" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A291C" Ref="C26"  Part="1" 
F 0 "C26" H 6768 3296 50  0000 L CNN
F 1 "10µ" H 6768 3205 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.7" H 6688 3100 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-elko-10-f-50-v-2000-h-eln-rv2-50v100mu-p246741.html?&trstct=pol_2&nbc=1" H 6650 3250 50  0001 C CNN
	1    6650 3250
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E7A2922
P 6200 3250
AR Path="/5E7A2922" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2922" Ref="C25"  Part="1" 
F 0 "C25" H 6315 3296 50  0000 L CNN
F 1 "100n" H 6315 3205 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 6238 3100 50  0001 C CNN
F 3 "" H 6200 3250 50  0000 C CNN
	1    6200 3250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5E7A2928
P 6650 3100
AR Path="/5E7A2928" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2928" Ref="#PWR099"  Part="1" 
F 0 "#PWR099" H 6650 2950 50  0001 C CNN
F 1 "+5V" H 6650 3250 50  0000 C CNN
F 2 "" H 6650 3100 50  0000 C CNN
F 3 "" H 6650 3100 50  0000 C CNN
	1    6650 3100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5E7A292E
P 6200 3100
AR Path="/5E7A292E" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A292E" Ref="#PWR097"  Part="1" 
F 0 "#PWR097" H 6200 2950 50  0001 C CNN
F 1 "+5V" H 6200 3250 50  0000 C CNN
F 2 "" H 6200 3100 50  0000 C CNN
F 3 "" H 6200 3100 50  0000 C CNN
	1    6200 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1400 4800 1700 4800
Wire Wire Line
	1400 4900 1850 4900
Wire Wire Line
	1850 5000 1400 5000
$Comp
L conn:CONN_01X01 P?
U 1 1 5E7A2937
P 1700 5300
AR Path="/5E7A2937" Ref="P?"  Part="1" 
AR Path="/5E766EDC/5E7A2937" Ref="P14"  Part="1" 
F 0 "P14" H 1778 5341 50  0000 L CNN
F 1 "TP_A-" H 1778 5250 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 1700 5300 50  0001 C CNN
F 3 "" H 1700 5300 50  0000 C CNN
	1    1700 5300
	0    1    1    0   
$EndComp
$Comp
L conn:CONN_01X01 P?
U 1 1 5E7A293D
P 1550 5300
AR Path="/5E7A293D" Ref="P?"  Part="1" 
AR Path="/5E766EDC/5E7A293D" Ref="P13"  Part="1" 
F 0 "P13" H 1628 5341 50  0000 L CNN
F 1 "TP_A+" H 1628 5250 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 1550 5300 50  0001 C CNN
F 3 "" H 1550 5300 50  0000 C CNN
	1    1550 5300
	0    1    1    0   
$EndComp
Wire Wire Line
	1700 5100 1700 4800
Connection ~ 1700 4800
Wire Wire Line
	1550 5100 1550 4700
Connection ~ 1550 4700
Wire Wire Line
	1550 4700 1400 4700
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
U 1 1 5E7A294F
P 1400 4600
AR Path="/5E7A294F" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A294F" Ref="#PWR074"  Part="1" 
F 0 "#PWR074" H 1400 4350 50  0001 C CNN
F 1 "GNDA" H 1405 4427 50  0000 C CNN
F 2 "" H 1400 4600 50  0001 C CNN
F 3 "" H 1400 4600 50  0001 C CNN
	1    1400 4600
	1    0    0    1   
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
L power:GNDA #PWR?
U 1 1 5E7A295B
P 6200 3400
AR Path="/5E7A295B" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A295B" Ref="#PWR098"  Part="1" 
F 0 "#PWR098" H 6200 3150 50  0001 C CNN
F 1 "GNDA" H 6205 3227 50  0000 C CNN
F 2 "" H 6200 3400 50  0001 C CNN
F 3 "" H 6200 3400 50  0001 C CNN
	1    6200 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2961
P 6650 3400
AR Path="/5E7A2961" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2961" Ref="#PWR0100"  Part="1" 
F 0 "#PWR0100" H 6650 3150 50  0001 C CNN
F 1 "GNDA" H 6655 3227 50  0000 C CNN
F 2 "" H 6650 3400 50  0001 C CNN
F 3 "" H 6650 3400 50  0001 C CNN
	1    6650 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2967
P 1800 3500
AR Path="/5E7A2967" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2967" Ref="#PWR075"  Part="1" 
F 0 "#PWR075" H 1800 3250 50  0001 C CNN
F 1 "GNDA" H 1805 3327 50  0000 C CNN
F 2 "" H 1800 3500 50  0001 C CNN
F 3 "" H 1800 3500 50  0001 C CNN
	1    1800 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E7A296D
P 1400 3500
AR Path="/5E7A296D" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A296D" Ref="#PWR073"  Part="1" 
F 0 "#PWR073" H 1400 3250 50  0001 C CNN
F 1 "GND" H 1405 3327 50  0000 C CNN
F 2 "" H 1400 3500 50  0000 C CNN
F 3 "" H 1400 3500 50  0000 C CNN
	1    1400 3500
	1    0    0    -1  
$EndComp
$Comp
L jumper:Jumper_2_Bridged JP?
U 1 1 5E7A2973
P 1600 3500
AR Path="/5E7A2973" Ref="JP?"  Part="1" 
AR Path="/5E766EDC/5E7A2973" Ref="JP9"  Part="1" 
F 0 "JP9" H 1600 3695 50  0000 C CNN
F 1 "Jumper_2_Bridged" H 1600 3604 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Bridged2Bar_Pad1.0x1.5mm" H 1600 3500 50  0001 C CNN
F 3 "~" H 1600 3500 50  0001 C CNN
	1    1600 3500
	1    0    0    -1  
$EndComp
$Comp
L device:CP C?
U 1 1 5E7A2979
P 10150 4350
AR Path="/5E7A2979" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2979" Ref="C28"  Part="1" 
F 0 "C28" H 10268 4396 50  0000 L CNN
F 1 "10µ" H 10268 4305 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.7" H 10188 4200 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-elko-10-f-50-v-2000-h-eln-rv2-50v100mu-p246741.html?&trstct=pol_2&nbc=1" H 10150 4350 50  0001 C CNN
	1    10150 4350
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E7A297F
P 1350 6000
AR Path="/5E7A297F" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A297F" Ref="C27"  Part="1" 
F 0 "C27" H 1465 6046 50  0000 L CNN
F 1 "100n" H 1465 5955 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 1388 5850 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 1350 6000 50  0001 C CNN
	1    1350 6000
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2985
P 1350 6150
AR Path="/5E7A2985" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2985" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 1350 5900 50  0001 C CNN
F 1 "GNDA" H 1355 5977 50  0000 C CNN
F 2 "" H 1350 6150 50  0001 C CNN
F 3 "" H 1350 6150 50  0001 C CNN
	1    1350 6150
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A298B
P 10150 4500
AR Path="/5E7A298B" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A298B" Ref="#PWR0110"  Part="1" 
F 0 "#PWR0110" H 10150 4250 50  0001 C CNN
F 1 "GNDA" H 10155 4327 50  0000 C CNN
F 2 "" H 10150 4500 50  0001 C CNN
F 3 "" H 10150 4500 50  0001 C CNN
	1    10150 4500
	1    0    0    -1  
$EndComp
$Comp
L transistors:IRF7343PBF Q?
U 1 1 5E7A2991
P 2950 2250
AR Path="/5E7A2991" Ref="Q?"  Part="1" 
AR Path="/5E766EDC/5E7A2991" Ref="Q1"  Part="1" 
F 0 "Q1" H 3238 2250 50  0000 L CNN
F 1 "IRF7343PBF" H 3238 2205 50  0001 L CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 3238 2205 50  0001 L CNN
F 3 "https://www.digikey.com/product-detail/en/infineon-technologies/IRF7343TRPBF/IRF7343PBFCT-ND/812557" H 3238 2114 50  0001 L CNN
	1    2950 2250
	1    0    0    -1  
$EndComp
$Comp
L transistors:IRF7343PBF Q?
U 2 1 5E7A2997
P 2950 1650
AR Path="/5E7A2997" Ref="Q?"  Part="2" 
AR Path="/5E766EDC/5E7A2997" Ref="Q1"  Part="2" 
F 0 "Q1" H 3238 1650 50  0000 L CNN
F 1 "IRF7343PBF" H 3238 1605 50  0001 L CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 3238 1605 50  0001 L CNN
F 3 "https://www.digikey.com/product-detail/en/infineon-technologies/IRF7343TRPBF/IRF7343PBFCT-ND/812557" H 3238 1514 50  0001 L CNN
	2    2950 1650
	1    0    0    1   
$EndComp
$Comp
L transistors:IRF7343PBF Q?
U 1 1 5E7A299D
P 4050 2250
AR Path="/5E7A299D" Ref="Q?"  Part="1" 
AR Path="/5E766EDC/5E7A299D" Ref="Q2"  Part="1" 
F 0 "Q2" H 4338 2250 50  0000 L CNN
F 1 "IRF7343PBF" H 4338 2205 50  0001 L CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 4338 2205 50  0001 L CNN
F 3 "https://www.digikey.com/product-detail/en/infineon-technologies/IRF7343TRPBF/IRF7343PBFCT-ND/812557" H 4338 2114 50  0001 L CNN
	1    4050 2250
	-1   0    0    -1  
$EndComp
$Comp
L transistors:IRF7343PBF Q?
U 2 1 5E7A29A3
P 4050 1650
AR Path="/5E7A29A3" Ref="Q?"  Part="2" 
AR Path="/5E766EDC/5E7A29A3" Ref="Q2"  Part="2" 
F 0 "Q2" H 4337 1650 50  0000 L CNN
F 1 "IRF7343PBF" H 4338 1605 50  0001 L CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 4338 1605 50  0001 L CNN
F 3 "https://www.digikey.com/product-detail/en/infineon-technologies/IRF7343TRPBF/IRF7343PBFCT-ND/812557" H 4338 1514 50  0001 L CNN
	2    4050 1650
	-1   0    0    1   
$EndComp
Wire Wire Line
	3950 1850 3950 1950
Wire Wire Line
	3850 1850 3850 1950
Wire Wire Line
	3950 1950 3850 1950
Connection ~ 3950 1950
Wire Wire Line
	3950 1950 3950 2050
Connection ~ 3850 1950
Wire Wire Line
	3850 1950 3850 2050
Wire Wire Line
	3150 1850 3150 1950
Wire Wire Line
	3050 1850 3050 1950
Wire Wire Line
	3050 1950 3150 1950
Connection ~ 3050 1950
Wire Wire Line
	3050 1950 3050 2050
Connection ~ 3150 1950
Wire Wire Line
	3150 1950 3150 2050
Wire Wire Line
	3150 1950 3400 1950
Wire Wire Line
	3850 1950 3600 1950
Text Label 3400 1950 2    50   ~ 0
EX+
Text Label 3600 1950 0    50   ~ 0
EX-
$Comp
L device:C C?
U 1 1 5E7A29BB
P 4400 1750
AR Path="/5E7A29BB" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A29BB" Ref="C21"  Part="1" 
F 0 "C21" H 4515 1796 50  0000 L CNN
F 1 "1n" H 4515 1705 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4438 1600 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-1-0nf-50v-125-c-kem-x7r1206-1-0n-p207139.html?&trstct=pol_4&nbc=1" H 4400 1750 50  0001 C CNN
	1    4400 1750
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E7A29C1
P 2600 1750
AR Path="/5E7A29C1" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A29C1" Ref="C15"  Part="1" 
F 0 "C15" H 2715 1796 50  0000 L CNN
F 1 "1n" H 2715 1705 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2638 1600 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-1-0nf-50v-125-c-kem-x7r1206-1-0n-p207139.html?&trstct=pol_4&nbc=1" H 2600 1750 50  0001 C CNN
	1    2600 1750
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E7A29C7
P 2600 2450
AR Path="/5E7A29C7" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A29C7" Ref="C16"  Part="1" 
F 0 "C16" H 2715 2496 50  0000 L CNN
F 1 "1n" H 2715 2405 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2638 2300 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-1-0nf-50v-125-c-kem-x7r1206-1-0n-p207139.html?&trstct=pol_4&nbc=1" H 2600 2450 50  0001 C CNN
	1    2600 2450
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E7A29CD
P 4400 2450
AR Path="/5E7A29CD" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A29CD" Ref="C22"  Part="1" 
F 0 "C22" H 4515 2496 50  0000 L CNN
F 1 "1n" H 4515 2405 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4438 2300 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-1-0nf-50v-125-c-kem-x7r1206-1-0n-p207139.html?&trstct=pol_4&nbc=1" H 4400 2450 50  0001 C CNN
	1    4400 2450
	1    0    0    -1  
$EndComp
$Comp
L device:D D?
U 1 1 5E7A29D3
P 2250 1250
AR Path="/5E7A29D3" Ref="D?"  Part="1" 
AR Path="/5E766EDC/5E7A29D3" Ref="D1"  Part="1" 
F 0 "D1" H 2250 1035 50  0000 C CNN
F 1 "BAT54" H 2250 1126 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-323_HandSoldering" H 2250 1250 50  0001 C CNN
F 3 "https://www.reichelt.de/schottkydiode-30-v-0-2-a-sod-323f-bat-54j-nxp-p219408.html?&trstct=pos_9&nbc=1" H 2250 1250 50  0001 C CNN
	1    2250 1250
	-1   0    0    1   
$EndComp
$Comp
L device:R R?
U 1 1 5E7A29D9
P 2250 1600
AR Path="/5E7A29D9" Ref="R?"  Part="1" 
AR Path="/5E766EDC/5E7A29D9" Ref="R18"  Part="1" 
F 0 "R18" V 2043 1600 50  0000 C CNN
F 1 "10k" V 2134 1600 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 2180 1600 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-10-kohm-250-mw-1-rnd-1206-1-10k-p183394.html?&trstct=pol_1&nbc=1" H 2250 1600 50  0001 C CNN
	1    2250 1600
	0    1    1    0   
$EndComp
Wire Wire Line
	2750 1600 2600 1600
Connection ~ 2600 1600
Wire Wire Line
	2400 1600 2600 1600
Wire Wire Line
	2400 1250 2400 1600
Connection ~ 2400 1600
Wire Wire Line
	2100 1250 2100 1600
$Comp
L device:D D?
U 1 1 5E7A29EB
P 2250 1950
AR Path="/5E7A29EB" Ref="D?"  Part="1" 
AR Path="/5E766EDC/5E7A29EB" Ref="D2"  Part="1" 
F 0 "D2" H 2250 2165 50  0000 C CNN
F 1 "BAT54" H 2250 2074 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-323_HandSoldering" H 2250 1950 50  0001 C CNN
F 3 "https://www.reichelt.de/schottkydiode-30-v-0-2-a-sod-323f-bat-54j-nxp-p219408.html?&trstct=pos_9&nbc=1" H 2250 1950 50  0001 C CNN
	1    2250 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 1950 2400 2300
Wire Wire Line
	2400 2300 2600 2300
Wire Wire Line
	2600 2300 2750 2300
Connection ~ 2600 2300
Wire Wire Line
	2100 1950 2100 2300
$Comp
L device:R R?
U 1 1 5E7A29F7
P 4850 2300
AR Path="/5E7A29F7" Ref="R?"  Part="1" 
AR Path="/5E766EDC/5E7A29F7" Ref="R28"  Part="1" 
F 0 "R28" V 4643 2300 50  0000 C CNN
F 1 "10k" V 4734 2300 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4780 2300 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-10-kohm-250-mw-1-rnd-1206-1-10k-p183394.html?&trstct=pol_1&nbc=1" H 4850 2300 50  0001 C CNN
	1    4850 2300
	0    1    1    0   
$EndComp
$Comp
L device:R R?
U 1 1 5E7A29FD
P 4850 1600
AR Path="/5E7A29FD" Ref="R?"  Part="1" 
AR Path="/5E766EDC/5E7A29FD" Ref="R27"  Part="1" 
F 0 "R27" V 4643 1600 50  0000 C CNN
F 1 "10k" V 4734 1600 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4780 1600 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-10-kohm-250-mw-1-rnd-1206-1-10k-p183394.html?&trstct=pol_1&nbc=1" H 4850 1600 50  0001 C CNN
	1    4850 1600
	0    1    1    0   
$EndComp
$Comp
L device:D D?
U 1 1 5E7A2A03
P 4850 1950
AR Path="/5E7A2A03" Ref="D?"  Part="1" 
AR Path="/5E766EDC/5E7A2A03" Ref="D4"  Part="1" 
F 0 "D4" H 4850 1735 50  0000 C CNN
F 1 "BAT54" H 4850 1826 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-323_HandSoldering" H 4850 1950 50  0001 C CNN
F 3 "https://www.reichelt.de/schottkydiode-30-v-0-2-a-sod-323f-bat-54j-nxp-p219408.html?&trstct=pos_9&nbc=1" H 4850 1950 50  0001 C CNN
	1    4850 1950
	-1   0    0    1   
$EndComp
$Comp
L device:D D?
U 1 1 5E7A2A09
P 4850 1250
AR Path="/5E7A2A09" Ref="D?"  Part="1" 
AR Path="/5E766EDC/5E7A2A09" Ref="D3"  Part="1" 
F 0 "D3" H 4850 1465 50  0000 C CNN
F 1 "BAT54" H 4850 1374 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-323_HandSoldering" H 4850 1250 50  0001 C CNN
F 3 "https://www.reichelt.de/schottkydiode-30-v-0-2-a-sod-323f-bat-54j-nxp-p219408.html?&trstct=pos_9&nbc=1" H 4850 1250 50  0001 C CNN
	1    4850 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 1600 4400 1600
Wire Wire Line
	4400 1600 4700 1600
Connection ~ 4400 1600
Wire Wire Line
	4250 2300 4400 2300
Wire Wire Line
	4400 2300 4700 2300
Connection ~ 4400 2300
Wire Wire Line
	4700 2300 4700 1950
Connection ~ 4700 2300
Wire Wire Line
	5000 1950 5000 2300
Wire Wire Line
	4700 1250 4700 1600
Connection ~ 4700 1600
Wire Wire Line
	5000 1250 5000 1600
$Comp
L 74xx:74HC14 U?
U 1 1 5E7A2A1B
P 1550 1750
AR Path="/5E7A2A1B" Ref="U?"  Part="1" 
AR Path="/5E766EDC/5E7A2A1B" Ref="U9"  Part="1" 
F 0 "U9" H 1550 2067 50  0000 C CNN
F 1 "74HCT14" H 1550 1976 50  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 1550 1750 50  0001 C CNN
F 3 "https://www.reichelt.de/inverter-hex-4-5-5-5-v-so-14-74hct-14d-nxp-p219163.html?&trstct=pos_1&nbc=1" H 1550 1750 50  0001 C CNN
	1    1550 1750
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74HC14 U?
U 2 1 5E7A2A21
P 5600 2000
AR Path="/5E7A2A21" Ref="U?"  Part="2" 
AR Path="/5E766EDC/5E7A2A21" Ref="U9"  Part="2" 
F 0 "U9" H 5600 2317 50  0000 C CNN
F 1 "74HCT14" H 5600 2226 50  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 5600 2000 50  0001 C CNN
F 3 "https://www.reichelt.de/inverter-hex-4-5-5-5-v-so-14-74hct-14d-nxp-p219163.html?&trstct=pos_1&nbc=1" H 5600 2000 50  0001 C CNN
	2    5600 2000
	-1   0    0    -1  
$EndComp
$Comp
L 74xx:74HC14 U?
U 3 1 5E7A2A27
P 6200 2000
AR Path="/5E7A2A27" Ref="U?"  Part="3" 
AR Path="/5E766EDC/5E7A2A27" Ref="U9"  Part="3" 
F 0 "U9" H 6200 1683 50  0000 C CNN
F 1 "74HCT14" H 6200 1774 50  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 6200 2000 50  0001 C CNN
F 3 "https://www.reichelt.de/inverter-hex-4-5-5-5-v-so-14-74hct-14d-nxp-p219163.html?&trstct=pos_1&nbc=1" H 6200 2000 50  0001 C CNN
	3    6200 2000
	-1   0    0    1   
$EndComp
$Comp
L 74xx:74HC14 U?
U 4 1 5E7A2A2D
P 7700 1200
AR Path="/5E7A2A2D" Ref="U?"  Part="4" 
AR Path="/5E766EDC/5E7A2A2D" Ref="U9"  Part="4" 
F 0 "U9" H 7700 1517 50  0000 C CNN
F 1 "74HCT14" H 7700 1426 50  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 7700 1200 50  0001 C CNN
F 3 "https://www.reichelt.de/inverter-hex-4-5-5-5-v-so-14-74hct-14d-nxp-p219163.html?&trstct=pos_1&nbc=1" H 7700 1200 50  0001 C CNN
	4    7700 1200
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74HC14 U?
U 5 1 5E7A2A33
P 7650 1750
AR Path="/5E7A2A33" Ref="U?"  Part="5" 
AR Path="/5E766EDC/5E7A2A33" Ref="U9"  Part="5" 
F 0 "U9" H 7650 2067 50  0000 C CNN
F 1 "74HCT14" H 7650 1976 50  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 7650 1750 50  0001 C CNN
F 3 "https://www.reichelt.de/inverter-hex-4-5-5-5-v-so-14-74hct-14d-nxp-p219163.html?&trstct=pos_1&nbc=1" H 7650 1750 50  0001 C CNN
	5    7650 1750
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74HC14 U?
U 6 1 5E7A2A39
P 7650 2300
AR Path="/5E7A2A39" Ref="U?"  Part="6" 
AR Path="/5E766EDC/5E7A2A39" Ref="U9"  Part="6" 
F 0 "U9" H 7650 2617 50  0000 C CNN
F 1 "74HCT14" H 7650 2526 50  0000 C CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 7650 2300 50  0001 C CNN
F 3 "https://www.reichelt.de/inverter-hex-4-5-5-5-v-so-14-74hct-14d-nxp-p219163.html?&trstct=pos_1&nbc=1" H 7650 2300 50  0001 C CNN
	6    7650 2300
	1    0    0    -1  
$EndComp
$Comp
L 74xx:74HC14 U?
U 7 1 5E7A2A3F
P 8500 1700
AR Path="/5E7A2A3F" Ref="U?"  Part="7" 
AR Path="/5E766EDC/5E7A2A3F" Ref="U9"  Part="7" 
F 0 "U9" H 8730 1746 50  0000 L CNN
F 1 "74HCT14" H 8730 1655 50  0000 L CNN
F 2 "Package_SO:SOIC-14_3.9x8.7mm_P1.27mm" H 8500 1700 50  0001 C CNN
F 3 "https://www.reichelt.de/inverter-hex-4-5-5-5-v-so-14-74hct-14d-nxp-p219163.html?&trstct=pos_1&nbc=1" H 8500 1700 50  0001 C CNN
	7    8500 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2A45
P 2600 1900
AR Path="/5E7A2A45" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2A45" Ref="#PWR076"  Part="1" 
F 0 "#PWR076" H 2600 1650 50  0001 C CNN
F 1 "GNDA" H 2605 1727 50  0000 C CNN
F 2 "" H 2600 1900 50  0001 C CNN
F 3 "" H 2600 1900 50  0001 C CNN
	1    2600 1900
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2A4B
P 2600 2600
AR Path="/5E7A2A4B" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2A4B" Ref="#PWR077"  Part="1" 
F 0 "#PWR077" H 2600 2350 50  0001 C CNN
F 1 "GNDA" H 2605 2427 50  0000 C CNN
F 2 "" H 2600 2600 50  0001 C CNN
F 3 "" H 2600 2600 50  0001 C CNN
	1    2600 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2A51
P 3250 2750
AR Path="/5E7A2A51" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2A51" Ref="#PWR082"  Part="1" 
F 0 "#PWR082" H 3250 2500 50  0001 C CNN
F 1 "GNDA" H 3255 2577 50  0000 C CNN
F 2 "" H 3250 2750 50  0001 C CNN
F 3 "" H 3250 2750 50  0001 C CNN
	1    3250 2750
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2A57
P 4400 2600
AR Path="/5E7A2A57" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2A57" Ref="#PWR090"  Part="1" 
F 0 "#PWR090" H 4400 2350 50  0001 C CNN
F 1 "GNDA" H 4405 2427 50  0000 C CNN
F 2 "" H 4400 2600 50  0001 C CNN
F 3 "" H 4400 2600 50  0001 C CNN
	1    4400 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2A5D
P 4400 1900
AR Path="/5E7A2A5D" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2A5D" Ref="#PWR089"  Part="1" 
F 0 "#PWR089" H 4400 1650 50  0001 C CNN
F 1 "GNDA" H 4405 1727 50  0000 C CNN
F 2 "" H 4400 1900 50  0001 C CNN
F 3 "" H 4400 1900 50  0001 C CNN
	1    4400 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 1600 1850 1600
Wire Wire Line
	1850 1600 1850 1750
Connection ~ 2100 1600
Wire Wire Line
	2100 2300 1850 2300
Wire Wire Line
	1850 2300 1850 1750
Connection ~ 1850 1750
Wire Wire Line
	5300 2000 5300 1600
Wire Wire Line
	5300 1600 5000 1600
Connection ~ 5000 1600
Wire Wire Line
	5300 2000 5300 2300
Wire Wire Line
	5300 2300 5000 2300
Connection ~ 5300 2000
Connection ~ 5000 2300
Wire Wire Line
	1250 3000 5300 3000
Wire Wire Line
	5300 3000 5300 2300
Wire Wire Line
	1250 1750 1250 3000
Connection ~ 5300 2300
$Comp
L linear:LT1492 U?
U 1 1 5E7A2A75
P 3450 4050
AR Path="/5E7A2A75" Ref="U?"  Part="1" 
AR Path="/5E766EDC/5E7A2A75" Ref="U10"  Part="1" 
F 0 "U10" H 3550 4400 50  0000 L CNN
F 1 "LT1492" H 3450 4300 50  0000 L CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 3450 4050 50  0001 C CNN
F 3 "http://cds.linear.com/docs/en/datasheet/14923f.pdf" H 3450 4050 50  0001 C CNN
	1    3450 4050
	1    0    0    -1  
$EndComp
$Comp
L linear:LT1492 U?
U 2 1 5E7A2A7B
P 3450 5550
AR Path="/5E7A2A7B" Ref="U?"  Part="2" 
AR Path="/5E766EDC/5E7A2A7B" Ref="U10"  Part="2" 
F 0 "U10" H 3550 5200 50  0000 C CNN
F 1 "LT1492" H 3550 5300 50  0000 C CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 3450 5550 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/linear-technology-analog-devices/LT1492CS8-PBF/LT1492CS8-PBF-ND/889428" H 3450 5550 50  0001 C CNN
	2    3450 5550
	1    0    0    1   
$EndComp
$Comp
L power:+5VA #PWR?
U 1 1 5E7A2A81
P 3350 3750
AR Path="/5E7A2A81" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2A81" Ref="#PWR083"  Part="1" 
F 0 "#PWR083" H 3350 3600 50  0001 C CNN
F 1 "+5VA" H 3365 3923 50  0000 C CNN
F 2 "" H 3350 3750 50  0001 C CNN
F 3 "" H 3350 3750 50  0001 C CNN
	1    3350 3750
	1    0    0    -1  
$EndComp
$Comp
L power:+5VA #PWR?
U 1 1 5E7A2A87
P 3350 5850
AR Path="/5E7A2A87" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2A87" Ref="#PWR086"  Part="1" 
F 0 "#PWR086" H 3350 5700 50  0001 C CNN
F 1 "+5VA" H 3365 6023 50  0000 C CNN
F 2 "" H 3350 5850 50  0001 C CNN
F 3 "" H 3350 5850 50  0001 C CNN
	1    3350 5850
	-1   0    0    1   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2A8D
P 3350 4350
AR Path="/5E7A2A8D" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2A8D" Ref="#PWR084"  Part="1" 
F 0 "#PWR084" H 3350 4100 50  0001 C CNN
F 1 "GNDA" H 3355 4177 50  0000 C CNN
F 2 "" H 3350 4350 50  0001 C CNN
F 3 "" H 3350 4350 50  0001 C CNN
	1    3350 4350
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2A93
P 3350 5250
AR Path="/5E7A2A93" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2A93" Ref="#PWR085"  Part="1" 
F 0 "#PWR085" H 3350 5000 50  0001 C CNN
F 1 "GNDA" H 3355 5077 50  0000 C CNN
F 2 "" H 3350 5250 50  0001 C CNN
F 3 "" H 3350 5250 50  0001 C CNN
	1    3350 5250
	-1   0    0    1   
$EndComp
$Comp
L device:R R?
U 1 1 5E7A2A99
P 2950 4800
AR Path="/5E7A2A99" Ref="R?"  Part="1" 
AR Path="/5E766EDC/5E7A2A99" Ref="R22"  Part="1" 
F 0 "R22" H 2880 4754 50  0000 R CNN
F 1 "100" H 2880 4845 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 2880 4800 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-100-ohm-250-mw-0-1-pan-era8aeb101v-p238078.html?&trstct=pol_1&nbc=1" H 2950 4800 50  0001 C CNN
F 4 "0.1%" H 3100 4800 50  0000 C CNN "Tolerance"
	1    2950 4800
	-1   0    0    1   
$EndComp
$Comp
L device:C C?
U 1 1 5E7A2A9F
P 4200 4200
AR Path="/5E7A2A9F" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2A9F" Ref="C19"  Part="1" 
F 0 "C19" H 4315 4246 50  0000 L CNN
F 1 "47n" H 4315 4155 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4238 4050 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-47nf-100v-125-c-kem-x7r1206-47n-p207148.html?&trstct=pol_4&nbc=1" H 4200 4200 50  0001 C CNN
	1    4200 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 4150 2950 4150
Wire Wire Line
	2950 4150 2950 4650
Wire Wire Line
	2950 4950 2950 5450
Wire Wire Line
	2950 5450 3150 5450
Wire Wire Line
	4200 4350 4200 4650
Wire Wire Line
	4200 4650 2950 4650
Connection ~ 2950 4650
$Comp
L device:C C?
U 1 1 5E7A2AAC
P 4200 5400
AR Path="/5E7A2AAC" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2AAC" Ref="C20"  Part="1" 
F 0 "C20" H 4315 5446 50  0000 L CNN
F 1 "47n" H 4315 5355 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4238 5250 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-47nf-100v-125-c-kem-x7r1206-47n-p207148.html?&trstct=pol_4&nbc=1" H 4200 5400 50  0001 C CNN
	1    4200 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 5250 4200 4950
Wire Wire Line
	4200 4950 2950 4950
Connection ~ 2950 4950
Wire Wire Line
	4200 4050 3750 4050
Wire Wire Line
	4200 5550 3750 5550
$Comp
L device:C C?
U 1 1 5E7A2AB7
P 2600 4100
AR Path="/5E7A2AB7" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2AB7" Ref="C17"  Part="1" 
F 0 "C17" H 2715 4146 50  0000 L CNN
F 1 "82n" H 2715 4055 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2638 3950 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-82nf-50v-125-c-kem-x7r1206-82n-p207150.html?&trstct=pol_0&nbc=1" H 2600 4100 50  0001 C CNN
	1    2600 4100
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E7A2ABD
P 2600 5800
AR Path="/5E7A2ABD" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2ABD" Ref="C18"  Part="1" 
F 0 "C18" H 2715 5846 50  0000 L CNN
F 1 "82n" H 2715 5755 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2638 5650 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-82nf-50v-125-c-kem-x7r1206-82n-p207150.html?&trstct=pol_0&nbc=1" H 2600 5800 50  0001 C CNN
	1    2600 5800
	1    0    0    -1  
$EndComp
$Comp
L device:R R?
U 1 1 5E7A2AC3
P 2350 5650
AR Path="/5E7A2AC3" Ref="R?"  Part="1" 
AR Path="/5E766EDC/5E7A2AC3" Ref="R21"  Part="1" 
F 0 "R21" V 2557 5650 50  0000 C CNN
F 1 "100" V 2466 5650 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 2280 5650 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-100-ohm-250-mw-0-1-pan-era8aeb101v-p238078.html?&trstct=pol_1&nbc=1" H 2350 5650 50  0001 C CNN
F 4 "0.1%" V 2250 5650 50  0000 C CNN "Tolerance"
	1    2350 5650
	0    -1   -1   0   
$EndComp
$Comp
L device:R R?
U 1 1 5E7A2AC9
P 2350 3950
AR Path="/5E7A2AC9" Ref="R?"  Part="1" 
AR Path="/5E766EDC/5E7A2AC9" Ref="R20"  Part="1" 
F 0 "R20" V 2557 3950 50  0000 C CNN
F 1 "100" V 2466 3950 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 2280 3950 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-100-ohm-250-mw-0-1-pan-era8aeb101v-p238078.html?&trstct=pol_1&nbc=1" H 2350 3950 50  0001 C CNN
F 4 "0.1%" V 2250 3950 50  0000 C CNN "Tolerance"
	1    2350 3950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2500 3950 2600 3950
Wire Wire Line
	2600 3950 3150 3950
Connection ~ 2600 3950
Wire Wire Line
	2500 5650 2600 5650
Wire Wire Line
	2600 5650 3150 5650
Connection ~ 2600 5650
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2AD5
P 2600 5950
AR Path="/5E7A2AD5" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2AD5" Ref="#PWR079"  Part="1" 
F 0 "#PWR079" H 2600 5700 50  0001 C CNN
F 1 "GNDA" H 2605 5777 50  0000 C CNN
F 2 "" H 2600 5950 50  0001 C CNN
F 3 "" H 2600 5950 50  0001 C CNN
	1    2600 5950
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2ADB
P 2600 4250
AR Path="/5E7A2ADB" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2ADB" Ref="#PWR078"  Part="1" 
F 0 "#PWR078" H 2600 4000 50  0001 C CNN
F 1 "GNDA" H 2605 4077 50  0000 C CNN
F 2 "" H 2600 4250 50  0001 C CNN
F 3 "" H 2600 4250 50  0001 C CNN
	1    2600 4250
	1    0    0    -1  
$EndComp
Text Label 1850 5000 2    50   ~ 0
EX+
Text Label 1850 4900 2    50   ~ 0
EX-
Wire Wire Line
	2200 3950 2200 4700
Wire Wire Line
	1550 4700 2200 4700
Wire Wire Line
	2200 5650 2200 4800
Wire Wire Line
	1700 4800 2200 4800
$Comp
L device:R R?
U 1 1 5E7A2AE7
P 4550 4050
AR Path="/5E7A2AE7" Ref="R?"  Part="1" 
AR Path="/5E766EDC/5E7A2AE7" Ref="R23"  Part="1" 
F 0 "R23" V 4757 4050 50  0000 C CNN
F 1 "10" V 4666 4050 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4480 4050 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/bourns-inc/CRT1206-BY-10R0ELF/CRT1206-BY-10R0ELFCT-ND/1775050" H 4550 4050 50  0001 C CNN
F 4 "0.1%" V 4550 4250 50  0000 C CNN "Tolerance"
	1    4550 4050
	0    -1   -1   0   
$EndComp
$Comp
L device:R R?
U 1 1 5E7A2AED
P 4550 5550
AR Path="/5E7A2AED" Ref="R?"  Part="1" 
AR Path="/5E766EDC/5E7A2AED" Ref="R26"  Part="1" 
F 0 "R26" V 4757 5550 50  0000 C CNN
F 1 "10" V 4666 5550 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4480 5550 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/bourns-inc/CRT1206-BY-10R0ELF/CRT1206-BY-10R0ELFCT-ND/1775050" H 4550 5550 50  0001 C CNN
F 4 "0.1%" V 4550 5750 50  0000 C CNN "Tolerance"
	1    4550 5550
	0    -1   -1   0   
$EndComp
$Comp
L device:R R?
U 1 1 5E7A2AF3
P 4550 4950
AR Path="/5E7A2AF3" Ref="R?"  Part="1" 
AR Path="/5E766EDC/5E7A2AF3" Ref="R25"  Part="1" 
F 0 "R25" V 4757 4950 50  0000 C CNN
F 1 "1k" V 4666 4950 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4480 4950 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-1-0-kohm-250-mw-0-1-pan-era8aeb102v-p238079.html?&trstct=pol_1&nbc=1" H 4550 4950 50  0001 C CNN
F 4 "0.1%" V 4550 5150 50  0000 C CNN "Tolerance"
	1    4550 4950
	0    -1   -1   0   
$EndComp
$Comp
L device:R R?
U 1 1 5E7A2AF9
P 4550 4650
AR Path="/5E7A2AF9" Ref="R?"  Part="1" 
AR Path="/5E766EDC/5E7A2AF9" Ref="R24"  Part="1" 
F 0 "R24" V 4757 4650 50  0000 C CNN
F 1 "1k" V 4666 4650 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 4480 4650 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-1-0-kohm-250-mw-0-1-pan-era8aeb102v-p238079.html?&trstct=pol_1&nbc=1" H 4550 4650 50  0001 C CNN
F 4 "0.1%" V 4550 4850 50  0000 C CNN "Tolerance"
	1    4550 4650
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4400 4050 4200 4050
Connection ~ 4200 4050
Wire Wire Line
	4400 4650 4200 4650
Connection ~ 4200 4650
Wire Wire Line
	4400 4950 4200 4950
Connection ~ 4200 4950
Wire Wire Line
	4400 5550 4200 5550
Connection ~ 4200 5550
Wire Wire Line
	4700 4650 4700 4050
Wire Wire Line
	4700 4950 4700 5550
$Comp
L device:C C?
U 1 1 5E7A2B09
P 5050 4200
AR Path="/5E7A2B09" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2B09" Ref="C23"  Part="1" 
F 0 "C23" H 5165 4246 50  0000 L CNN
F 1 "1µ" H 5165 4155 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5088 4050 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-1-0-f-25v-85-c-kem-y5v1206-1-0u-p207164.html?&trstct=pol_0&nbc=1" H 5050 4200 50  0001 C CNN
	1    5050 4200
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E7A2B0F
P 5050 5700
AR Path="/5E7A2B0F" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2B0F" Ref="C24"  Part="1" 
F 0 "C24" H 5165 5746 50  0000 L CNN
F 1 "1µ" H 5165 5655 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5088 5550 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-1-0-f-25v-85-c-kem-y5v1206-1-0u-p207164.html?&trstct=pol_0&nbc=1" H 5050 5700 50  0001 C CNN
	1    5050 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 4050 4700 4050
Connection ~ 4700 4050
Wire Wire Line
	5050 5550 4700 5550
Connection ~ 4700 5550
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2B19
P 5050 5850
AR Path="/5E7A2B19" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B19" Ref="#PWR092"  Part="1" 
F 0 "#PWR092" H 5050 5600 50  0001 C CNN
F 1 "GNDA" H 5055 5677 50  0000 C CNN
F 2 "" H 5050 5850 50  0001 C CNN
F 3 "" H 5050 5850 50  0001 C CNN
	1    5050 5850
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2B1F
P 5050 4350
AR Path="/5E7A2B1F" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B1F" Ref="#PWR091"  Part="1" 
F 0 "#PWR091" H 5050 4100 50  0001 C CNN
F 1 "GNDA" H 5055 4177 50  0000 C CNN
F 2 "" H 5050 4350 50  0001 C CNN
F 3 "" H 5050 4350 50  0001 C CNN
	1    5050 4350
	1    0    0    -1  
$EndComp
$Comp
L LTC2440CGN:LTC2440CGN U?
U 1 1 5E7A2B2A
P 5850 4500
AR Path="/5E7A2B2A" Ref="U?"  Part="1" 
AR Path="/5E766EDC/5E7A2B2A" Ref="U11"  Part="1" 
F 0 "U11" H 6650 4865 50  0000 C CNN
F 1 "LTC2440CGN" H 6650 4774 50  0000 C CNN
F 2 "Package_SO:QSOP-16_3.9x4.9mm_P0.635mm" H 5850 4500 50  0001 L BNN
F 3 "https://www.reichelt.de/index.html?ACTION=446&LA=446&nbc=1&q=ltc2440" H 5850 4500 50  0001 L BNN
F 4 "Linear Technology" H 5850 4500 50  0001 L BNN "Field4"
F 5 "LTC2440 - 24-Bit High Speed Differential Delta Sigma ADC with Selectable Speed/Resolution" H 5850 4500 50  0001 L BNN "Field5"
F 6 "LTC2440CGN" H 5850 4500 50  0001 L BNN "Field6"
F 7 "SSOP-16 Linear Technology" H 5850 4500 50  0001 L BNN "Field7"
F 8 "Unavailable" H 5850 4500 50  0001 L BNN "Field8"
	1    5850 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 4900 5550 4900
Wire Wire Line
	5550 4900 5550 4050
Wire Wire Line
	5550 4050 5050 4050
Connection ~ 5050 4050
Wire Wire Line
	5850 5000 5550 5000
Wire Wire Line
	5550 5000 5550 5550
Wire Wire Line
	5550 5550 5050 5550
Connection ~ 5050 5550
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2B38
P 5850 5200
AR Path="/5E7A2B38" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B38" Ref="#PWR096"  Part="1" 
F 0 "#PWR096" H 5850 4950 50  0001 C CNN
F 1 "GNDA" H 5855 5027 50  0000 C CNN
F 2 "" H 5850 5200 50  0001 C CNN
F 3 "" H 5850 5200 50  0001 C CNN
	1    5850 5200
	0    1    1    0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2B3E
P 5850 4500
AR Path="/5E7A2B3E" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B3E" Ref="#PWR093"  Part="1" 
F 0 "#PWR093" H 5850 4250 50  0001 C CNN
F 1 "GNDA" H 5855 4327 50  0000 C CNN
F 2 "" H 5850 4500 50  0001 C CNN
F 3 "" H 5850 4500 50  0001 C CNN
	1    5850 4500
	0    1    1    0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2B44
P 7450 4500
AR Path="/5E7A2B44" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B44" Ref="#PWR0106"  Part="1" 
F 0 "#PWR0106" H 7450 4250 50  0001 C CNN
F 1 "GNDA" H 7455 4327 50  0000 C CNN
F 2 "" H 7450 4500 50  0001 C CNN
F 3 "" H 7450 4500 50  0001 C CNN
	1    7450 4500
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2B4A
P 7450 5200
AR Path="/5E7A2B4A" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B4A" Ref="#PWR0108"  Part="1" 
F 0 "#PWR0108" H 7450 4950 50  0001 C CNN
F 1 "GNDA" H 7455 5027 50  0000 C CNN
F 2 "" H 7450 5200 50  0001 C CNN
F 3 "" H 7450 5200 50  0001 C CNN
	1    7450 5200
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2B50
P 7450 4700
AR Path="/5E7A2B50" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B50" Ref="#PWR0107"  Part="1" 
F 0 "#PWR0107" H 7450 4450 50  0001 C CNN
F 1 "GNDA" H 7455 4527 50  0000 C CNN
F 2 "" H 7450 4700 50  0001 C CNN
F 3 "" H 7450 4700 50  0001 C CNN
	1    7450 4700
	0    -1   -1   0   
$EndComp
$Comp
L power:+5VA #PWR?
U 1 1 5E7A2B57
P 3050 1450
AR Path="/5E7A2B57" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B57" Ref="#PWR081"  Part="1" 
F 0 "#PWR081" H 3050 1300 50  0001 C CNN
F 1 "+5VA" H 3065 1623 50  0000 C CNN
F 2 "" H 3050 1450 50  0001 C CNN
F 3 "" H 3050 1450 50  0001 C CNN
	1    3050 1450
	1    0    0    -1  
$EndComp
$Comp
L power:+5VA #PWR?
U 1 1 5E7A2B5D
P 3950 1450
AR Path="/5E7A2B5D" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B5D" Ref="#PWR088"  Part="1" 
F 0 "#PWR088" H 3950 1300 50  0001 C CNN
F 1 "+5VA" H 3965 1623 50  0000 C CNN
F 2 "" H 3950 1450 50  0001 C CNN
F 3 "" H 3950 1450 50  0001 C CNN
	1    3950 1450
	1    0    0    -1  
$EndComp
$Comp
L power:+5VA #PWR?
U 1 1 5E7A2B63
P 1350 5850
AR Path="/5E7A2B63" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B63" Ref="#PWR0101"  Part="1" 
F 0 "#PWR0101" H 1350 5700 50  0001 C CNN
F 1 "+5VA" H 1365 6023 50  0000 C CNN
F 2 "" H 1350 5850 50  0001 C CNN
F 3 "" H 1350 5850 50  0001 C CNN
	1    1350 5850
	1    0    0    -1  
$EndComp
$Comp
L power:+5VA #PWR?
U 1 1 5E7A2B69
P 10150 4200
AR Path="/5E7A2B69" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B69" Ref="#PWR0109"  Part="1" 
F 0 "#PWR0109" H 10150 4050 50  0001 C CNN
F 1 "+5VA" H 10165 4373 50  0000 C CNN
F 2 "" H 10150 4200 50  0001 C CNN
F 3 "" H 10150 4200 50  0001 C CNN
	1    10150 4200
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2B6F
P 7400 1200
AR Path="/5E7A2B6F" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B6F" Ref="#PWR0105"  Part="1" 
F 0 "#PWR0105" H 7400 950 50  0001 C CNN
F 1 "GNDA" H 7405 1027 50  0000 C CNN
F 2 "" H 7400 1200 50  0001 C CNN
F 3 "" H 7400 1200 50  0001 C CNN
	1    7400 1200
	0    1    1    0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2B75
P 7350 1750
AR Path="/5E7A2B75" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B75" Ref="#PWR0103"  Part="1" 
F 0 "#PWR0103" H 7350 1500 50  0001 C CNN
F 1 "GNDA" H 7355 1577 50  0000 C CNN
F 2 "" H 7350 1750 50  0001 C CNN
F 3 "" H 7350 1750 50  0001 C CNN
	1    7350 1750
	0    1    1    0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2B7B
P 7350 2300
AR Path="/5E7A2B7B" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B7B" Ref="#PWR0104"  Part="1" 
F 0 "#PWR0104" H 7350 2050 50  0001 C CNN
F 1 "GNDA" H 7355 2127 50  0000 C CNN
F 2 "" H 7350 2300 50  0001 C CNN
F 3 "" H 7350 2300 50  0001 C CNN
	1    7350 2300
	0    1    1    0   
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2B81
P 8500 2200
AR Path="/5E7A2B81" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B81" Ref="#PWR0115"  Part="1" 
F 0 "#PWR0115" H 8500 1950 50  0001 C CNN
F 1 "GNDA" H 8505 2027 50  0000 C CNN
F 2 "" H 8500 2200 50  0001 C CNN
F 3 "" H 8500 2200 50  0001 C CNN
	1    8500 2200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5E7A2B87
P 8500 1200
AR Path="/5E7A2B87" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B87" Ref="#PWR0114"  Part="1" 
F 0 "#PWR0114" H 8500 1050 50  0001 C CNN
F 1 "+5V" H 8500 1350 50  0000 C CNN
F 2 "" H 8500 1200 50  0000 C CNN
F 3 "" H 8500 1200 50  0000 C CNN
	1    8500 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+5VA #PWR?
U 1 1 5E7A2B8D
P 5850 4700
AR Path="/5E7A2B8D" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B8D" Ref="#PWR095"  Part="1" 
F 0 "#PWR095" H 5850 4550 50  0001 C CNN
F 1 "+5VA" V 5865 4827 50  0000 L CNN
F 2 "" H 5850 4700 50  0001 C CNN
F 3 "" H 5850 4700 50  0001 C CNN
	1    5850 4700
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5E7A2B93
P 5850 4600
AR Path="/5E7A2B93" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2B93" Ref="#PWR094"  Part="1" 
F 0 "#PWR094" H 5850 4450 50  0001 C CNN
F 1 "+5V" H 5850 4750 50  0000 C CNN
F 2 "" H 5850 4600 50  0000 C CNN
F 3 "" H 5850 4600 50  0000 C CNN
	1    5850 4600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3050 2450 3250 2450
Wire Wire Line
	5850 4800 5650 4800
Wire Wire Line
	5650 4800 5650 3100
Wire Wire Line
	5650 3100 3450 3100
Wire Wire Line
	3450 3100 3450 2450
Connection ~ 3450 2450
Wire Wire Line
	3450 2450 3950 2450
$Comp
L jumper:SolderJumper_2_Bridged JP?
U 1 1 5E7A2BA0
P 3250 2600
AR Path="/5E7A2BA0" Ref="JP?"  Part="1" 
AR Path="/5E766EDC/5E7A2BA0" Ref="JP10"  Part="1" 
F 0 "JP10" V 3250 2668 50  0000 L CNN
F 1 "SolderJumper_2_Bridged" V 3295 2668 50  0001 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Bridged2Bar_Pad1.0x1.5mm" H 3250 2600 50  0001 C CNN
F 3 "~" H 3250 2600 50  0001 C CNN
	1    3250 2600
	0    1    1    0   
$EndComp
Connection ~ 3250 2450
Wire Wire Line
	3250 2450 3450 2450
$Comp
L device:R R?
U 1 1 5E7A2BAD
P 7900 5250
AR Path="/5E7A2BAD" Ref="R?"  Part="1" 
AR Path="/5E766EDC/5E7A2BAD" Ref="R29"  Part="1" 
F 0 "R29" H 7970 5296 50  0000 L CNN
F 1 "10k" H 7970 5205 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7830 5250 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-10-kohm-250-mw-1-rnd-1206-1-10k-p183394.html?&trstct=pol_1&nbc=1" H 7900 5250 50  0001 C CNN
	1    7900 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2BB3
P 7900 5400
AR Path="/5E7A2BB3" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2BB3" Ref="#PWR0111"  Part="1" 
F 0 "#PWR0111" H 7900 5150 50  0001 C CNN
F 1 "GNDA" H 7905 5227 50  0000 C CNN
F 2 "" H 7900 5400 50  0001 C CNN
F 3 "" H 7900 5400 50  0001 C CNN
	1    7900 5400
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
Wire Wire Line
	7450 5100 7900 5100
$Comp
L device:C C?
U 1 1 5E7A2BC6
P 9300 1700
AR Path="/5E7A2BC6" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2BC6" Ref="C30"  Part="1" 
F 0 "C30" H 9415 1746 50  0000 L CNN
F 1 "100n" H 9415 1655 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9338 1550 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 9300 1700 50  0001 C CNN
	1    9300 1700
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 5E7A2BCC
P 9300 1550
AR Path="/5E7A2BCC" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2BCC" Ref="#PWR0118"  Part="1" 
F 0 "#PWR0118" H 9300 1400 50  0001 C CNN
F 1 "+5V" H 9300 1700 50  0000 C CNN
F 2 "" H 9300 1550 50  0000 C CNN
F 3 "" H 9300 1550 50  0000 C CNN
	1    9300 1550
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2BD2
P 9300 1850
AR Path="/5E7A2BD2" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2BD2" Ref="#PWR0119"  Part="1" 
F 0 "#PWR0119" H 9300 1600 50  0001 C CNN
F 1 "GNDA" H 9305 1677 50  0000 C CNN
F 2 "" H 9300 1850 50  0001 C CNN
F 3 "" H 9300 1850 50  0001 C CNN
	1    9300 1850
	1    0    0    -1  
$EndComp
$Comp
L custom_lor:lt1461 U?
U 1 1 5E7A2BD8
P 9150 4300
AR Path="/5E7A2BD8" Ref="U?"  Part="1" 
AR Path="/5E766EDC/5E7A2BD8" Ref="U12"  Part="1" 
F 0 "U12" H 9125 4665 50  0000 C CNN
F 1 "lt1461" H 9125 4574 50  0000 C CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 9150 4300 50  0001 C CNN
F 3 "https://www.digikey.com/product-detail/en/linear-technology-analog-devices/LT1461CCS8-3-PBF/LT1461CCS8-3-PBF-ND/889264" H 9150 4300 50  0001 C CNN
	1    9150 4300
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E7A2BDE
P 9700 4350
AR Path="/5E7A2BDE" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2BDE" Ref="C31"  Part="1" 
F 0 "C31" H 9815 4396 50  0000 L CNN
F 1 "2µ2" H 9815 4305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9738 4200 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-kerko-1206-2-2-f-10-v-10-mlcc-rnd-1501206b2251-p225544.html?&trstct=pol_0&nbc=1" H 9700 4350 50  0001 C CNN
	1    9700 4350
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E7A2BE4
P 8550 4350
AR Path="/5E7A2BE4" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5E7A2BE4" Ref="C29"  Part="1" 
F 0 "C29" H 8665 4396 50  0000 L CNN
F 1 "1µ" H 8665 4305 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8588 4200 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-1-0-f-25v-85-c-kem-y5v1206-1-0u-p207164.html?&trstct=pol_0&nbc=1" H 8550 4350 50  0001 C CNN
	1    8550 4350
	1    0    0    -1  
$EndComp
$Comp
L power:+5VA #PWR?
U 1 1 5E7A2BEA
P 9700 4200
AR Path="/5E7A2BEA" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2BEA" Ref="#PWR0120"  Part="1" 
F 0 "#PWR0120" H 9700 4050 50  0001 C CNN
F 1 "+5VA" H 9715 4373 50  0000 C CNN
F 2 "" H 9700 4200 50  0001 C CNN
F 3 "" H 9700 4200 50  0001 C CNN
	1    9700 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 4200 9700 4200
Connection ~ 9700 4200
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2BF2
P 9050 4700
AR Path="/5E7A2BF2" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2BF2" Ref="#PWR0117"  Part="1" 
F 0 "#PWR0117" H 9050 4450 50  0001 C CNN
F 1 "GNDA" H 9055 4527 50  0000 C CNN
F 2 "" H 9050 4700 50  0001 C CNN
F 3 "" H 9050 4700 50  0001 C CNN
	1    9050 4700
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2BF8
P 9700 4500
AR Path="/5E7A2BF8" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2BF8" Ref="#PWR0121"  Part="1" 
F 0 "#PWR0121" H 9700 4250 50  0001 C CNN
F 1 "GNDA" H 9705 4327 50  0000 C CNN
F 2 "" H 9700 4500 50  0001 C CNN
F 3 "" H 9700 4500 50  0001 C CNN
	1    9700 4500
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5E7A2BFE
P 8550 4500
AR Path="/5E7A2BFE" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E7A2BFE" Ref="#PWR0116"  Part="1" 
F 0 "#PWR0116" H 8550 4250 50  0001 C CNN
F 1 "GNDA" H 8555 4327 50  0000 C CNN
F 2 "" H 8550 4500 50  0001 C CNN
F 3 "" H 8550 4500 50  0001 C CNN
	1    8550 4500
	1    0    0    -1  
$EndComp
NoConn ~ 9250 4700
Wire Wire Line
	8750 4200 8550 4200
Text HLabel 8550 4200 0    50   Input ~ 0
VBOOST
Text HLabel 7450 4900 2    50   Input ~ 0
MISO
Text HLabel 7450 5000 2    50   Input ~ 0
~CS
Text HLabel 7900 5100 2    50   Input ~ 0
~EXT
Text HLabel 7450 4800 2    50   Input ~ 0
CLK
Text HLabel 7450 4600 2    50   Input ~ 0
BUSY
Text HLabel 5850 5100 0    50   Input ~ 0
MOSI
Text HLabel 6500 2000 2    50   Input ~ 0
HBRIDGE
Text HLabel 8050 3100 1    50   Input ~ 0
5V
$Comp
L power:+5V #PWR?
U 1 1 5E9C7A2E
P 8050 3100
AR Path="/5E9C7A2E" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E9C7A2E" Ref="#PWR0112"  Part="1" 
F 0 "#PWR0112" H 8050 2950 50  0001 C CNN
F 1 "+5V" H 8050 3250 50  0000 C CNN
F 2 "" H 8050 3100 50  0000 C CNN
F 3 "" H 8050 3100 50  0000 C CNN
	1    8050 3100
	-1   0    0    1   
$EndComp
Text HLabel 8250 3100 1    50   Input ~ 0
GND
$Comp
L power:GND #PWR?
U 1 1 5E9EF2D8
P 8250 3100
AR Path="/5E9EF2D8" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5E9EF2D8" Ref="#PWR0113"  Part="1" 
F 0 "#PWR0113" H 8250 2850 50  0001 C CNN
F 1 "GND" H 8255 2927 50  0000 C CNN
F 2 "" H 8250 3100 50  0000 C CNN
F 3 "" H 8250 3100 50  0000 C CNN
	1    8250 3100
	1    0    0    -1  
$EndComp
NoConn ~ 8000 1200
NoConn ~ 7950 1750
NoConn ~ 7950 2300
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
$Comp
L device:C C?
U 1 1 5EE0FAE0
P 3500 1150
AR Path="/5EE0FAE0" Ref="C?"  Part="1" 
AR Path="/5E766EDC/5EE0FAE0" Ref="C32"  Part="1" 
F 0 "C32" H 3615 1196 50  0000 L CNN
F 1 "100n" H 3615 1105 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 3538 1000 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 3500 1150 50  0001 C CNN
	1    3500 1150
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR?
U 1 1 5EE0FAEA
P 3500 1300
AR Path="/5EE0FAEA" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5EE0FAEA" Ref="#PWR0123"  Part="1" 
F 0 "#PWR0123" H 3500 1050 50  0001 C CNN
F 1 "GNDA" H 3505 1127 50  0000 C CNN
F 2 "" H 3500 1300 50  0001 C CNN
F 3 "" H 3500 1300 50  0001 C CNN
	1    3500 1300
	1    0    0    -1  
$EndComp
$Comp
L power:+5VA #PWR?
U 1 1 5EE0FAF4
P 3500 1000
AR Path="/5EE0FAF4" Ref="#PWR?"  Part="1" 
AR Path="/5E766EDC/5EE0FAF4" Ref="#PWR0122"  Part="1" 
F 0 "#PWR0122" H 3500 850 50  0001 C CNN
F 1 "+5VA" H 3515 1173 50  0000 C CNN
F 2 "" H 3500 1000 50  0001 C CNN
F 3 "" H 3500 1000 50  0001 C CNN
	1    3500 1000
	1    0    0    -1  
$EndComp
Connection ~ 2100 2300
Connection ~ 2400 2300
$Comp
L device:R R?
U 1 1 5E7A29E5
P 2250 2300
AR Path="/5E7A29E5" Ref="R?"  Part="1" 
AR Path="/5E766EDC/5E7A29E5" Ref="R19"  Part="1" 
F 0 "R19" V 2043 2300 50  0000 C CNN
F 1 "10k" V 2134 2300 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 2180 2300 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-10-kohm-250-mw-1-rnd-1206-1-10k-p183394.html?&trstct=pol_1&nbc=1" H 2250 2300 50  0001 C CNN
	1    2250 2300
	0    1    1    0   
$EndComp
$EndSCHEMATC
