EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Label 3850 2950 0    50   ~ 0
TXD
Text Label 3850 3150 0    50   ~ 0
RXD
$Comp
L pspice:R R6
U 1 1 5E6E8C96
P 5850 4100
F 0 "R6" H 5782 4054 50  0000 R CNN
F 1 "750" H 5782 4145 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5850 4100 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-750-ohm-250-mw-1-koa-rk73h2bttd71-p238075.html?&trstct=pol_2&nbc=1" H 5850 4100 50  0001 C CNN
	1    5850 4100
	-1   0    0    1   
$EndComp
$Comp
L pspice:R R7
U 1 1 5E6E8C9C
P 5850 4600
F 0 "R7" H 5782 4554 50  0000 R CNN
F 1 "1k5" H 5782 4645 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5850 4600 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-1-5-kohm-250-mw-1-rnd-1206-1-1-5k-p183381.html?&trstct=pol_0&nbc=1" H 5850 4600 50  0001 C CNN
	1    5850 4600
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR057
U 1 1 5E6E8CA2
P 5850 4850
F 0 "#PWR057" H 5850 4600 50  0001 C CNN
F 1 "GND" H 5855 4677 50  0000 C CNN
F 2 "" H 5850 4850 50  0000 C CNN
F 3 "" H 5850 4850 50  0000 C CNN
	1    5850 4850
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R12
U 1 1 5E6E8CA8
P 7550 4050
F 0 "R12" H 7482 4004 50  0000 R CNN
F 1 "470" H 7482 4095 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7550 4050 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-470-ohm-250-mw-1-rnd-1206-1-470-p183358.html?&trstct=pol_0&nbc=1" H 7550 4050 50  0001 C CNN
	1    7550 4050
	-1   0    0    1   
$EndComp
$Comp
L pspice:R R13
U 1 1 5E6E8CAE
P 7550 4700
F 0 "R13" H 7482 4654 50  0000 R CNN
F 1 "910" H 7482 4745 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7550 4700 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-910-ohm-250-mw-1-rnd-1206-1-910-p212738.html?&trstct=pol_0&nbc=1" H 7550 4700 50  0001 C CNN
	1    7550 4700
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR063
U 1 1 5E6E8CB4
P 7550 4950
F 0 "#PWR063" H 7550 4700 50  0001 C CNN
F 1 "GND" H 7555 4777 50  0000 C CNN
F 2 "" H 7550 4950 50  0000 C CNN
F 3 "" H 7550 4950 50  0000 C CNN
	1    7550 4950
	1    0    0    -1  
$EndComp
$Comp
L conn:CONN_01X04 P9
U 1 1 5E6E8CBA
P 5550 2600
F 0 "P9" H 5628 2641 50  0000 L CNN
F 1 "UART" H 5628 2550 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 5550 2600 50  0001 C CNN
F 3 "https://www.reichelt.de/jst-stiftleiste-gerade-1x4-polig-xh-jst-xh4p-st-p185075.html?&trstct=pol_12&nbc=1" H 5550 2600 50  0001 C CNN
	1    5550 2600
	1    0    0    -1  
$EndComp
Text Label 5350 2750 2    50   ~ 0
DTR
Text Label 5350 2550 2    50   ~ 0
TXD
Text Label 5350 2450 2    50   ~ 0
RXD
Text Label 5350 2650 2    50   ~ 0
RTS
$Comp
L rf-modules:ESP32-WROOM-32U U6
U 1 1 5E6E8CC4
P 3250 4050
F 0 "U6" H 2900 5450 50  0000 C CNN
F 1 "ESP32-WROOM-32U" H 3650 5450 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32U" H 3250 2550 50  0001 C CNN
F 3 "https://www.digikey.de/product-detail/en/espressif-systems/ESP32-WROOM-32U-16MB/1904-1028-1-ND/9381737?_ga=2.78679122.772215829.1584722537-1107834322.1578137426" H 2950 4100 50  0001 C CNN
	1    3250 4050
	1    0    0    -1  
$EndComp
Text Label 2650 2850 2    50   ~ 0
RTS
Text Label 3850 2850 0    50   ~ 0
DTR
$Comp
L power:+3.3V #PWR041
U 1 1 5E6E8CCC
P 3250 2650
F 0 "#PWR041" H 3250 2500 50  0001 C CNN
F 1 "+3.3V" H 3265 2823 50  0000 C CNN
F 2 "" H 3250 2650 50  0000 C CNN
F 3 "" H 3250 2650 50  0000 C CNN
	1    3250 2650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR042
U 1 1 5E6E8CD2
P 3250 5450
F 0 "#PWR042" H 3250 5200 50  0001 C CNN
F 1 "GND" H 3255 5277 50  0000 C CNN
F 2 "" H 3250 5450 50  0000 C CNN
F 3 "" H 3250 5450 50  0000 C CNN
	1    3250 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 4850 4600 4850
Wire Wire Line
	5200 4850 5200 4350
Wire Wire Line
	5200 4350 5500 4350
Connection ~ 5850 4350
$Comp
L device:C C10
U 1 1 5E6E8CDD
P 5500 4500
F 0 "C10" H 5615 4546 50  0000 L CNN
F 1 "100n" H 5615 4455 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5538 4350 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 5500 4500 50  0001 C CNN
	1    5500 4500
	1    0    0    -1  
$EndComp
Connection ~ 5500 4350
Wire Wire Line
	5500 4350 5850 4350
$Comp
L power:GND #PWR056
U 1 1 5E6E8CE5
P 5500 4650
F 0 "#PWR056" H 5500 4400 50  0001 C CNN
F 1 "GND" H 5505 4477 50  0000 C CNN
F 2 "" H 5500 4650 50  0000 C CNN
F 3 "" H 5500 4650 50  0000 C CNN
	1    5500 4650
	1    0    0    -1  
$EndComp
Text Label 3850 4750 0    50   ~ 0
PING
$Comp
L conn:CONN_01X01 P8
U 1 1 5E6E8CED
P 4600 4600
F 0 "P8" H 4678 4641 50  0000 L CNN
F 1 "TP_ADC" H 4678 4550 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 4600 4600 50  0001 C CNN
F 3 "" H 4600 4600 50  0000 C CNN
	1    4600 4600
	0    -1   -1   0   
$EndComp
NoConn ~ 2650 4050
NoConn ~ 2650 4150
NoConn ~ 2650 4250
NoConn ~ 2650 4350
NoConn ~ 2650 4450
NoConn ~ 2650 4550
NoConn ~ 2650 3050
NoConn ~ 2650 3150
Wire Wire Line
	4600 4800 4600 4850
Connection ~ 4600 4850
Wire Wire Line
	4600 4850 5200 4850
$Comp
L pspice:R R4
U 1 1 5E6E8D0F
P 4400 3050
F 0 "R4" H 4332 3004 50  0000 R CNN
F 1 "10k" H 4332 3095 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4400 3050 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-10-kohm-250-mw-1-rnd-1206-1-10k-p183394.html?&trstct=pol_1&nbc=1" H 4400 3050 50  0001 C CNN
	1    4400 3050
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR049
U 1 1 5E6E8D15
P 4650 3050
F 0 "#PWR049" H 4650 2800 50  0001 C CNN
F 1 "GND" H 4655 2877 50  0000 C CNN
F 2 "" H 4650 3050 50  0000 C CNN
F 3 "" H 4650 3050 50  0000 C CNN
	1    4650 3050
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR051
U 1 1 5E6E8D21
P 4600 3750
F 0 "#PWR051" H 4600 3500 50  0001 C CNN
F 1 "GND" H 4605 3577 50  0000 C CNN
F 2 "" H 4600 3750 50  0000 C CNN
F 3 "" H 4600 3750 50  0000 C CNN
	1    4600 3750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4100 3750 3850 3750
Text Label 3850 4650 0    50   ~ 0
RED
Text Label 3850 4550 0    50   ~ 0
GREEN
Text Label 3850 4950 0    50   ~ 0
BLUE
$Comp
L pspice:R R3
U 1 1 5E6E8D2B
P 4350 3750
F 0 "R3" H 4282 3704 50  0000 R CNN
F 1 "10k" H 4282 3795 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4350 3750 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-10-kohm-250-mw-1-rnd-1206-1-10k-p183394.html?&trstct=pol_1&nbc=1" H 4350 3750 50  0001 C CNN
F 4 "OPT" H 4350 3750 50  0000 C CNN "Field4"
	1    4350 3750
	0    -1   -1   0   
$EndComp
$Comp
L jumper:Jumper_2_Open JP6
U 1 1 5E6E8D31
P 5850 3650
F 0 "JP6" H 5850 3885 50  0000 C CNN
F 1 "JP_ADC" H 5850 3794 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5850 3650 50  0001 C CNN
F 3 "~" H 5850 3650 50  0001 C CNN
	1    5850 3650
	0    1    1    0   
$EndComp
Text Label 6400 4350 2    50   ~ 0
VBUSY
$Comp
L pspice:R R10
U 1 1 5E6E8D3C
P 7100 4050
F 0 "R10" H 7032 4004 50  0000 R CNN
F 1 "470" H 7032 4095 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7100 4050 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-470-ohm-250-mw-1-rnd-1206-1-470-p183358.html?&trstct=pol_0&nbc=1" H 7100 4050 50  0001 C CNN
	1    7100 4050
	-1   0    0    1   
$EndComp
$Comp
L pspice:R R11
U 1 1 5E6E8D42
P 7100 4650
F 0 "R11" H 7032 4604 50  0000 R CNN
F 1 "910" H 7032 4695 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7100 4650 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-910-ohm-250-mw-1-rnd-1206-1-910-p212738.html?&trstct=pol_0&nbc=1" H 7100 4650 50  0001 C CNN
	1    7100 4650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR059
U 1 1 5E6E8D48
P 7100 4900
F 0 "#PWR059" H 7100 4650 50  0001 C CNN
F 1 "GND" H 7105 4727 50  0000 C CNN
F 2 "" H 7100 4900 50  0000 C CNN
F 3 "" H 7100 4900 50  0000 C CNN
	1    7100 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 4300 7100 4350
Wire Wire Line
	6850 4350 6900 4350
Connection ~ 7100 4350
Wire Wire Line
	7100 4350 7100 4400
$Comp
L jumper:Jumper_2_Open JP7
U 1 1 5E6E8D53
P 6900 4050
F 0 "JP7" V 6900 3962 50  0000 R CNN
F 1 "JP_CLK" V 6945 4148 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6900 4050 50  0001 C CNN
F 3 "~" H 6900 4050 50  0001 C CNN
	1    6900 4050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6900 3850 6900 3800
Wire Wire Line
	6900 3800 7100 3800
Wire Wire Line
	6900 4250 6900 4350
Connection ~ 6900 4350
Wire Wire Line
	6900 4350 7100 4350
$Comp
L pspice:R R8
U 1 1 5E6E8D5E
P 6500 4100
F 0 "R8" H 6432 4054 50  0000 R CNN
F 1 "470" H 6432 4145 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 6500 4100 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-470-ohm-250-mw-1-rnd-1206-1-470-p183358.html?&trstct=pol_0&nbc=1" H 6500 4100 50  0001 C CNN
	1    6500 4100
	-1   0    0    1   
$EndComp
$Comp
L pspice:R R9
U 1 1 5E6E8D64
P 6500 4600
F 0 "R9" H 6432 4554 50  0000 R CNN
F 1 "910" H 6432 4645 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 6500 4600 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-910-ohm-250-mw-1-rnd-1206-1-910-p212738.html?&trstct=pol_0&nbc=1" H 6500 4600 50  0001 C CNN
	1    6500 4600
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR058
U 1 1 5E6E8D6A
P 6500 4850
F 0 "#PWR058" H 6500 4600 50  0001 C CNN
F 1 "GND" H 6505 4677 50  0000 C CNN
F 2 "" H 6500 4850 50  0000 C CNN
F 3 "" H 6500 4850 50  0000 C CNN
	1    6500 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4350 6400 4350
Connection ~ 6500 4350
Text Label 3850 3750 0    50   ~ 0
H~CS
Text Label 3850 3650 0    50   ~ 0
HCLK
Text Label 3850 3450 0    50   ~ 0
HMISO
Text Label 3850 3550 0    50   ~ 0
HMOSI
Text Label 3850 4150 0    50   ~ 0
VMISO
Text Label 3850 4050 0    50   ~ 0
VCLK
$Comp
L device:CP C?
U 1 1 5E6F3BE4
P 8025 1575
AR Path="/5E6F3BE4" Ref="C?"  Part="1" 
AR Path="/5E6C650B/5E6F3BE4" Ref="C13"  Part="1" 
F 0 "C13" H 8143 1621 50  0000 L CNN
F 1 "10µ" H 8143 1530 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.7" H 8063 1425 50  0001 C CNN
F 3 "" H 8025 1575 50  0000 C CNN
	1    8025 1575
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5E6F3BEA
P 8025 1425
AR Path="/5E6F3BEA" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E6F3BEA" Ref="#PWR066"  Part="1" 
F 0 "#PWR066" H 8025 1275 50  0001 C CNN
F 1 "+3.3V" H 8040 1598 50  0000 C CNN
F 2 "" H 8025 1425 50  0000 C CNN
F 3 "" H 8025 1425 50  0000 C CNN
	1    8025 1425
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E6F3BF0
P 8025 1725
AR Path="/5E6F3BF0" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E6F3BF0" Ref="#PWR067"  Part="1" 
F 0 "#PWR067" H 8025 1475 50  0001 C CNN
F 1 "GND" H 8030 1552 50  0000 C CNN
F 2 "" H 8025 1725 50  0000 C CNN
F 3 "" H 8025 1725 50  0000 C CNN
	1    8025 1725
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E6F3BF6
P 8425 1600
AR Path="/5E6F3BF6" Ref="C?"  Part="1" 
AR Path="/5E6C650B/5E6F3BF6" Ref="C14"  Part="1" 
F 0 "C14" H 8540 1646 50  0000 L CNN
F 1 "100n" H 8540 1555 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8463 1450 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 8425 1600 50  0001 C CNN
	1    8425 1600
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5E6F3BFC
P 8425 1450
AR Path="/5E6F3BFC" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E6F3BFC" Ref="#PWR068"  Part="1" 
F 0 "#PWR068" H 8425 1300 50  0001 C CNN
F 1 "+3.3V" H 8440 1623 50  0000 C CNN
F 2 "" H 8425 1450 50  0000 C CNN
F 3 "" H 8425 1450 50  0000 C CNN
	1    8425 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E6F3C02
P 8425 1750
AR Path="/5E6F3C02" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E6F3C02" Ref="#PWR069"  Part="1" 
F 0 "#PWR069" H 8425 1500 50  0001 C CNN
F 1 "GND" H 8430 1577 50  0000 C CNN
F 2 "" H 8425 1750 50  0000 C CNN
F 3 "" H 8425 1750 50  0000 C CNN
	1    8425 1750
	1    0    0    -1  
$EndComp
$Comp
L device:CP C?
U 1 1 5E6F3C08
P 7750 1600
AR Path="/5E6F3C08" Ref="C?"  Part="1" 
AR Path="/5E6C650B/5E6F3C08" Ref="C12"  Part="1" 
F 0 "C12" H 7868 1646 50  0000 L CNN
F 1 "10µ" H 7868 1555 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.7" H 7788 1450 50  0001 C CNN
F 3 "" H 7750 1600 50  0000 C CNN
	1    7750 1600
	1    0    0    -1  
$EndComp
$Comp
L device:C C?
U 1 1 5E6F3C0E
P 7450 1600
AR Path="/5E6F3C0E" Ref="C?"  Part="1" 
AR Path="/5E6C650B/5E6F3C0E" Ref="C11"  Part="1" 
F 0 "C11" H 7565 1646 50  0000 L CNN
F 1 "100n" H 7565 1555 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7488 1450 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 7450 1600 50  0001 C CNN
	1    7450 1600
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5E6F3C14
P 7750 1450
AR Path="/5E6F3C14" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E6F3C14" Ref="#PWR064"  Part="1" 
F 0 "#PWR064" H 7750 1300 50  0001 C CNN
F 1 "+3.3V" H 7765 1623 50  0000 C CNN
F 2 "" H 7750 1450 50  0000 C CNN
F 3 "" H 7750 1450 50  0000 C CNN
	1    7750 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E6F3C1A
P 7750 1750
AR Path="/5E6F3C1A" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E6F3C1A" Ref="#PWR065"  Part="1" 
F 0 "#PWR065" H 7750 1500 50  0001 C CNN
F 1 "GND" H 7755 1577 50  0000 C CNN
F 2 "" H 7750 1750 50  0000 C CNN
F 3 "" H 7750 1750 50  0000 C CNN
	1    7750 1750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E6F3C20
P 7450 1750
AR Path="/5E6F3C20" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E6F3C20" Ref="#PWR062"  Part="1" 
F 0 "#PWR062" H 7450 1500 50  0001 C CNN
F 1 "GND" H 7455 1577 50  0000 C CNN
F 2 "" H 7450 1750 50  0000 C CNN
F 3 "" H 7450 1750 50  0000 C CNN
	1    7450 1750
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5E6F3C26
P 7450 1450
AR Path="/5E6F3C26" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E6F3C26" Ref="#PWR061"  Part="1" 
F 0 "#PWR061" H 7450 1300 50  0001 C CNN
F 1 "+3.3V" H 7465 1623 50  0000 C CNN
F 2 "" H 7450 1450 50  0000 C CNN
F 3 "" H 7450 1450 50  0000 C CNN
	1    7450 1450
	1    0    0    -1  
$EndComp
$Comp
L modules:W25Q128 U?
U 1 1 5E74523E
P 1650 1450
AR Path="/5E74523E" Ref="U?"  Part="1" 
AR Path="/5E6C650B/5E74523E" Ref="U7"  Part="1" 
F 0 "U7" H 1650 1915 50  0000 C CNN
F 1 "W25Q128" H 1650 1824 50  0000 C CNN
F 2 "memory:W25Q128-module" H 1650 1450 50  0001 C CNN
F 3 "https://www.banggood.com/W25Q32-W25Q128-Large-Capacity-FLASH-Storage-Module-Memory-Card-SPI-Interface-BV-FV-STM32-p-1595134.html?rmmds=search&ID=6281655&cur_warehouse=CN" H 1650 1450 50  0001 C CNN
	1    1650 1450
	1    0    0    -1  
$EndComp
Text Label 1300 1250 2    50   ~ 0
H~CS
Text Label 1300 1400 2    50   ~ 0
HMISO
Text Label 1300 1550 2    50   ~ 0
HCLK
Text Label 1300 1700 2    50   ~ 0
HMOSI
$Comp
L device:C C?
U 1 1 5E745248
P 2300 1450
AR Path="/5E745248" Ref="C?"  Part="1" 
AR Path="/5E6C650B/5E745248" Ref="C9"  Part="1" 
F 0 "C9" H 2415 1496 50  0000 L CNN
F 1 "100n" H 2415 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2338 1300 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 2300 1450 50  0001 C CNN
	1    2300 1450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E74524E
P 2300 1600
AR Path="/5E74524E" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E74524E" Ref="#PWR055"  Part="1" 
F 0 "#PWR055" H 2300 1350 50  0001 C CNN
F 1 "GND" H 2305 1427 50  0000 C CNN
F 2 "" H 2300 1600 50  0000 C CNN
F 3 "" H 2300 1600 50  0000 C CNN
	1    2300 1600
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5E745254
P 2300 1300
AR Path="/5E745254" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E745254" Ref="#PWR054"  Part="1" 
F 0 "#PWR054" H 2300 1150 50  0001 C CNN
F 1 "+3.3V" H 2315 1473 50  0000 C CNN
F 2 "" H 2300 1300 50  0000 C CNN
F 3 "" H 2300 1300 50  0000 C CNN
	1    2300 1300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5E74525A
P 2000 1350
AR Path="/5E74525A" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E74525A" Ref="#PWR052"  Part="1" 
F 0 "#PWR052" H 2000 1200 50  0001 C CNN
F 1 "+3.3V" H 2015 1523 50  0000 C CNN
F 2 "" H 2000 1350 50  0000 C CNN
F 3 "" H 2000 1350 50  0000 C CNN
	1    2000 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E745260
P 2000 1550
AR Path="/5E745260" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E745260" Ref="#PWR053"  Part="1" 
F 0 "#PWR053" H 2000 1300 50  0001 C CNN
F 1 "GND" H 2005 1377 50  0000 C CNN
F 2 "" H 2000 1550 50  0000 C CNN
F 3 "" H 2000 1550 50  0000 C CNN
	1    2000 1550
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R17
U 1 1 5E7B19D8
P 9300 3050
F 0 "R17" H 9232 3004 50  0000 R CNN
F 1 "22" H 9232 3095 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9300 3050 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-22-ohm-250-mw-1-rnd-1206-1-22-p183340.html?&trstct=pol_0&nbc=1" H 9300 3050 50  0001 C CNN
	1    9300 3050
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR071
U 1 1 5E7B19DE
P 9800 2000
F 0 "#PWR071" H 9800 1850 50  0001 C CNN
F 1 "+3.3V" H 9815 2173 50  0000 C CNN
F 2 "" H 9800 2000 50  0000 C CNN
F 3 "" H 9800 2000 50  0000 C CNN
	1    9800 2000
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R15
U 1 1 5E7B19E4
P 9300 2350
F 0 "R15" V 9400 2450 50  0000 R CNN
F 1 "120" V 9200 2400 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9300 2350 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-120-ohm-250-mw-1-rnd-1206-1-120-p183350.html?&trstct=pol_1&nbc=1" H 9300 2350 50  0001 C CNN
	1    9300 2350
	0    -1   -1   0   
$EndComp
$Comp
L pspice:R R16
U 1 1 5E7B19EA
P 9300 2700
F 0 "R16" H 9232 2654 50  0000 R CNN
F 1 "91" H 9232 2745 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9300 2700 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-91-ohm-250-mw-1-rnd-1206-1-91-p212729.html?&trstct=pol_0&nbc=1" H 9300 2700 50  0001 C CNN
	1    9300 2700
	0    -1   -1   0   
$EndComp
Text Label 9050 2350 2    50   ~ 0
RED
Text Label 9050 2700 2    50   ~ 0
GREEN
Text Label 9050 3050 2    50   ~ 0
BLUE
$Comp
L conn:CONN_01X04 P11
U 1 1 5E7B19F3
P 9950 2600
F 0 "P11" H 10028 2641 50  0000 L CNN
F 1 "LED_RGB" H 10028 2550 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 9950 2600 50  0001 C CNN
F 3 "https://www.reichelt.de/jst-stiftleiste-gerade-1x4-polig-xh-jst-xh4p-st-p185075.html?&trstct=pol_12&nbc=1" H 9950 2600 50  0001 C CNN
	1    9950 2600
	1    0    0    -1  
$EndComp
Text Label 9050 3850 2    50   ~ 0
PING
$Comp
L pspice:R R14
U 1 1 5E7B1A00
P 9300 3850
F 0 "R14" V 9095 3850 50  0000 C CNN
F 1 "22" V 9186 3850 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9300 3850 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-22-ohm-250-mw-1-rnd-1206-1-22-p183340.html?&trstct=pol_0&nbc=1" H 9300 3850 50  0001 C CNN
	1    9300 3850
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR072
U 1 1 5E7B1A0C
P 10300 2000
F 0 "#PWR072" H 10300 1750 50  0001 C CNN
F 1 "GND" H 10305 1827 50  0000 C CNN
F 2 "" H 10300 2000 50  0000 C CNN
F 3 "" H 10300 2000 50  0000 C CNN
	1    10300 2000
	1    0    0    -1  
$EndComp
$Comp
L jumper:Jumper_3_Bridged12 JP8
U 1 1 5E7B1A12
P 10050 2000
F 0 "JP8" H 10050 2113 50  0000 C CNN
F 1 "JP_RGB" H 10050 2113 50  0001 C CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Open_Pad1.0x1.5mm" H 10050 2000 50  0001 C CNN
F 3 "~" H 10050 2000 50  0001 C CNN
	1    10050 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 2150 9750 2150
Wire Wire Line
	9750 2150 9750 2450
Wire Wire Line
	9550 2350 9650 2350
Wire Wire Line
	9650 2350 9650 2550
Wire Wire Line
	9650 2550 9750 2550
Wire Wire Line
	9550 2700 9550 2650
Wire Wire Line
	9550 2650 9750 2650
Wire Wire Line
	9550 3050 9750 3050
Wire Wire Line
	9750 3050 9750 2750
Wire Wire Line
	9550 3850 9750 3850
Text HLabel 7550 3800 1    50   Input ~ 0
MISO
Text HLabel 7100 3800 1    50   Input ~ 0
CLK
$Comp
L maxim:DS18B20 U8
U 1 1 5E8239E4
P 4950 1200
F 0 "U8" H 5178 1246 50  0000 L CNN
F 1 "DS18B20" H 5178 1155 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_HandSolder" H 4800 1450 50  0001 C CNN
F 3 "https://www.reichelt.de/digital-thermometer-1-wire-0-5-c-to-92-ds-18b20-p58169.html?&trstct=pos_0&nbc=1" H 5178 1109 50  0001 L CNN
	1    4950 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR047
U 1 1 5E82607D
P 4650 1100
F 0 "#PWR047" H 4650 950 50  0001 C CNN
F 1 "+3.3V" H 4665 1273 50  0000 C CNN
F 2 "" H 4650 1100 50  0000 C CNN
F 3 "" H 4650 1100 50  0000 C CNN
	1    4650 1100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR048
U 1 1 5E826F4B
P 4650 1300
F 0 "#PWR048" H 4650 1050 50  0001 C CNN
F 1 "GND" H 4655 1127 50  0000 C CNN
F 2 "" H 4650 1300 50  0000 C CNN
F 3 "" H 4650 1300 50  0000 C CNN
	1    4650 1300
	1    0    0    -1  
$EndComp
$Comp
L device:C C8
U 1 1 5E829631
P 4050 1200
F 0 "C8" H 4165 1246 50  0000 L CNN
F 1 "100n" H 4165 1155 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4088 1050 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 4050 1200 50  0001 C CNN
	1    4050 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR044
U 1 1 5E82C310
P 4050 1050
F 0 "#PWR044" H 4050 900 50  0001 C CNN
F 1 "+3.3V" H 4065 1223 50  0000 C CNN
F 2 "" H 4050 1050 50  0000 C CNN
F 3 "" H 4050 1050 50  0000 C CNN
	1    4050 1050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR045
U 1 1 5E82C571
P 4050 1350
F 0 "#PWR045" H 4050 1100 50  0001 C CNN
F 1 "GND" H 4055 1177 50  0000 C CNN
F 2 "" H 4050 1350 50  0000 C CNN
F 3 "" H 4050 1350 50  0000 C CNN
	1    4050 1350
	1    0    0    -1  
$EndComp
Text Label 4650 1200 2    50   ~ 0
DQ
Text Label 3850 3250 0    50   ~ 0
DQ
Text Label 6850 4350 2    50   ~ 0
VCLK
Text Label 7450 4350 2    50   ~ 0
VMISO
Wire Wire Line
	7450 4350 7550 4350
Wire Wire Line
	7550 4350 7550 4300
Wire Wire Line
	7550 4450 7550 4350
Connection ~ 7550 4350
Text HLabel 6500 3850 1    50   Input ~ 0
BUSY
Text Label 3850 3950 0    50   ~ 0
VBUSY
Wire Wire Line
	4150 3050 3850 3050
$Comp
L power:+3.3V #PWR046
U 1 1 5E6E8D08
P 4450 2350
F 0 "#PWR046" H 4450 2200 50  0001 C CNN
F 1 "+3.3V" H 4465 2523 50  0000 C CNN
F 2 "" H 4450 2350 50  0000 C CNN
F 3 "" H 4450 2350 50  0000 C CNN
	1    4450 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 2850 4450 2850
$Comp
L pspice:R R5
U 1 1 5E6E8CFF
P 4450 2600
F 0 "R5" H 4382 2554 50  0000 R CNN
F 1 "10k" H 4382 2645 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4450 2600 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-10-kohm-250-mw-1-rnd-1206-1-10k-p183394.html?&trstct=pol_1&nbc=1" H 4450 2600 50  0001 C CNN
	1    4450 2600
	-1   0    0    1   
$EndComp
Text HLabel 5850 3450 1    50   Input ~ 0
BAT+
Text HLabel 2850 1450 1    50   Input ~ 0
3V3
Text HLabel 3150 1450 1    50   Input ~ 0
GND
$Comp
L power:GND #PWR037
U 1 1 5E89F4DE
P 3150 1450
F 0 "#PWR037" H 3150 1200 50  0001 C CNN
F 1 "GND" H 3155 1277 50  0000 C CNN
F 2 "" H 3150 1450 50  0000 C CNN
F 3 "" H 3150 1450 50  0000 C CNN
	1    3150 1450
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR035
U 1 1 5E89FACF
P 2850 1450
F 0 "#PWR035" H 2850 1300 50  0001 C CNN
F 1 "+3.3V" H 2865 1623 50  0000 C CNN
F 2 "" H 2850 1450 50  0000 C CNN
F 3 "" H 2850 1450 50  0000 C CNN
	1    2850 1450
	-1   0    0    1   
$EndComp
$Comp
L 74xx_IEEE:74165 U5
U 1 1 5E8C394F
P 4650 6550
F 0 "U5" H 4650 7316 50  0000 C CNN
F 1 "74HC165" H 4650 7225 50  0000 C CNN
F 2 "Package_SO:SOIC-16_3.9x9.9mm_P1.27mm" H 4650 6550 50  0001 C CNN
F 3 "https://www.reichelt.de/shift-register-8-bit-2-6-v-so-16-74hc-165d-nxp-p216626.html?&trstct=pos_2&nbc=1" H 4650 6550 50  0001 C CNN
	1    4650 6550
	1    0    0    -1  
$EndComp
NoConn ~ 5200 7450
NoConn ~ 4100 6550
Text Label 4100 6350 2    50   ~ 0
HCLK
Text Label 5700 7500 3    50   ~ 0
QH
Text Label 3850 5050 0    50   ~ 0
QH
$Comp
L pspice:R R2
U 1 1 5E92BEE5
P 4350 3450
F 0 "R2" H 4282 3404 50  0000 R CNN
F 1 "10k" H 4282 3495 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4350 3450 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-10-kohm-250-mw-1-rnd-1206-1-10k-p183394.html?&trstct=pol_1&nbc=1" H 4350 3450 50  0001 C CNN
F 4 "OPT" H 4350 3450 50  0000 C CNN "Field4"
	1    4350 3450
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR050
U 1 1 5E92C73D
P 4600 3450
F 0 "#PWR050" H 4600 3200 50  0001 C CNN
F 1 "GND" H 4605 3277 50  0000 C CNN
F 2 "" H 4600 3450 50  0000 C CNN
F 3 "" H 4600 3450 50  0000 C CNN
	1    4600 3450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4100 3450 3850 3450
Wire Wire Line
	5500 7350 5200 7350
$Comp
L power:+3.3V #PWR?
U 1 1 5E93DF36
P 4850 6000
AR Path="/5E93DF36" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E93DF36" Ref="#PWR038"  Part="1" 
F 0 "#PWR038" H 4850 5850 50  0001 C CNN
F 1 "+3.3V" H 4865 6173 50  0000 C CNN
F 2 "" H 4850 6000 50  0000 C CNN
F 3 "" H 4850 6000 50  0000 C CNN
	1    4850 6000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E93E493
P 4650 6000
AR Path="/5E93E493" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E93E493" Ref="#PWR036"  Part="1" 
F 0 "#PWR036" H 4650 5750 50  0001 C CNN
F 1 "GND" H 4655 5827 50  0000 C CNN
F 2 "" H 4650 6000 50  0000 C CNN
F 3 "" H 4650 6000 50  0000 C CNN
	1    4650 6000
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5E93F176
P 5900 7350
AR Path="/5E93F176" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E93F176" Ref="#PWR043"  Part="1" 
F 0 "#PWR043" H 5900 7100 50  0001 C CNN
F 1 "GND" H 5905 7177 50  0000 C CNN
F 2 "" H 5900 7350 50  0000 C CNN
F 3 "" H 5900 7350 50  0000 C CNN
	1    5900 7350
	0    -1   -1   0   
$EndComp
Text HLabel 3850 4350 2    50   Input ~ 0
HBRIDGE
Text HLabel 3850 3350 2    50   Input ~ 0
V~CS
Text HLabel 3850 4250 2    50   Input ~ 0
~EXT
$Comp
L jumper:SolderJumper_3_Open JP3
U 1 1 5EA53B87
P 5700 7350
F 0 "JP3" H 5700 7555 50  0000 C CNN
F 1 "JP_ID" H 5700 7464 50  0000 C CNN
F 2 "jumper:SolderJumper-3_P1.3mm_Open_Pad1.0x1.5mm" H 5700 7350 50  0001 C CNN
F 3 "~" H 5700 7350 50  0001 C CNN
	1    5700 7350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EA5A662
P 4100 7400
AR Path="/5EA5A662" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5EA5A662" Ref="#PWR034"  Part="1" 
F 0 "#PWR034" H 4100 7150 50  0001 C CNN
F 1 "GND" H 4105 7227 50  0000 C CNN
F 2 "" H 4100 7400 50  0000 C CNN
F 3 "" H 4100 7400 50  0000 C CNN
	1    4100 7400
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EA5AEF8
P 4100 7250
AR Path="/5EA5AEF8" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5EA5AEF8" Ref="#PWR033"  Part="1" 
F 0 "#PWR033" H 4100 7000 50  0001 C CNN
F 1 "GND" H 4105 7077 50  0000 C CNN
F 2 "" H 4100 7250 50  0000 C CNN
F 3 "" H 4100 7250 50  0000 C CNN
	1    4100 7250
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EA5B0FE
P 4100 7150
AR Path="/5EA5B0FE" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5EA5B0FE" Ref="#PWR032"  Part="1" 
F 0 "#PWR032" H 4100 6900 50  0001 C CNN
F 1 "GND" H 4105 6977 50  0000 C CNN
F 2 "" H 4100 7150 50  0000 C CNN
F 3 "" H 4100 7150 50  0000 C CNN
	1    4100 7150
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EA5B32A
P 4100 7050
AR Path="/5EA5B32A" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5EA5B32A" Ref="#PWR031"  Part="1" 
F 0 "#PWR031" H 4100 6800 50  0001 C CNN
F 1 "GND" H 4105 6877 50  0000 C CNN
F 2 "" H 4100 7050 50  0000 C CNN
F 3 "" H 4100 7050 50  0000 C CNN
	1    4100 7050
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EA5B510
P 4100 6950
AR Path="/5EA5B510" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5EA5B510" Ref="#PWR030"  Part="1" 
F 0 "#PWR030" H 4100 6700 50  0001 C CNN
F 1 "GND" H 4105 6777 50  0000 C CNN
F 2 "" H 4100 6950 50  0000 C CNN
F 3 "" H 4100 6950 50  0000 C CNN
	1    4100 6950
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EA5B6B9
P 4100 6850
AR Path="/5EA5B6B9" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5EA5B6B9" Ref="#PWR029"  Part="1" 
F 0 "#PWR029" H 4100 6600 50  0001 C CNN
F 1 "GND" H 4105 6677 50  0000 C CNN
F 2 "" H 4100 6850 50  0000 C CNN
F 3 "" H 4100 6850 50  0000 C CNN
	1    4100 6850
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EA5B8A5
P 4100 6650
AR Path="/5EA5B8A5" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5EA5B8A5" Ref="#PWR027"  Part="1" 
F 0 "#PWR027" H 4100 6400 50  0001 C CNN
F 1 "GND" H 4105 6477 50  0000 C CNN
F 2 "" H 4100 6650 50  0000 C CNN
F 3 "" H 4100 6650 50  0000 C CNN
	1    4100 6650
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5EA5C04E
P 4100 6750
AR Path="/5EA5C04E" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5EA5C04E" Ref="#PWR028"  Part="1" 
F 0 "#PWR028" H 4100 6600 50  0001 C CNN
F 1 "+3.3V" H 4115 6923 50  0000 C CNN
F 2 "" H 4100 6750 50  0000 C CNN
F 3 "" H 4100 6750 50  0000 C CNN
	1    4100 6750
	0    -1   -1   0   
$EndComp
Text HLabel 3850 4450 2    50   Input ~ 0
VMOSI
$Comp
L device:C C?
U 1 1 5EA81C54
P 5300 6400
AR Path="/5EA81C54" Ref="C?"  Part="1" 
AR Path="/5E6C650B/5EA81C54" Ref="C7"  Part="1" 
F 0 "C7" H 5415 6446 50  0000 L CNN
F 1 "100n" H 5415 6355 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5338 6250 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 5300 6400 50  0001 C CNN
	1    5300 6400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5EA81C5A
P 5300 6550
AR Path="/5EA81C5A" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5EA81C5A" Ref="#PWR040"  Part="1" 
F 0 "#PWR040" H 5300 6300 50  0001 C CNN
F 1 "GND" H 5305 6377 50  0000 C CNN
F 2 "" H 5300 6550 50  0000 C CNN
F 3 "" H 5300 6550 50  0000 C CNN
	1    5300 6550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5EA81C60
P 5300 6250
AR Path="/5EA81C60" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5EA81C60" Ref="#PWR039"  Part="1" 
F 0 "#PWR039" H 5300 6100 50  0001 C CNN
F 1 "+3.3V" H 5315 6423 50  0000 C CNN
F 2 "" H 5300 6250 50  0000 C CNN
F 3 "" H 5300 6250 50  0000 C CNN
	1    5300 6250
	1    0    0    -1  
$EndComp
Wire Notes Line
	3850 750  5550 750 
Wire Notes Line
	5550 750  5550 1700
Wire Notes Line
	5550 1700 3850 1700
Wire Notes Line
	3850 1700 3850 750 
Text Notes 4400 1650 0    50   ~ 0
Inside Shield
NoConn ~ 3850 3850
$Comp
L device:C C?
U 1 1 5E75FA3B
P 2700 6100
AR Path="/5E75FA3B" Ref="C?"  Part="1" 
AR Path="/5E6C650B/5E75FA3B" Ref="C33"  Part="1" 
F 0 "C33" H 2815 6146 50  0000 L CNN
F 1 "100n" H 2815 6055 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2738 5950 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 2700 6100 50  0001 C CNN
	1    2700 6100
	0    1    1    0   
$EndComp
$Comp
L pspice:R R30
U 1 1 5E764CE9
P 2300 6100
F 0 "R30" H 2232 6054 50  0000 R CNN
F 1 "470" H 2232 6145 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2300 6100 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-470-ohm-250-mw-1-rnd-1206-1-470-p183358.html?&trstct=pol_0&nbc=1" H 2300 6100 50  0001 C CNN
	1    2300 6100
	0    -1   -1   0   
$EndComp
$Comp
L device:D_Schottky D5
U 1 1 5E76CE12
P 3200 6250
F 0 "D5" V 3246 6171 50  0000 R CNN
F 1 "BAT54" V 3155 6171 50  0000 R CNN
F 2 "Diode_SMD:D_SOD-323_HandSoldering" H 3200 6250 50  0001 C CNN
F 3 "https://www.reichelt.de/schottkydiode-30-v-0-2-a-sod-323f-bat-54j-nxp-p219408.html?&trstct=pos_9&nbc=1" H 3200 6250 50  0001 C CNN
	1    3200 6250
	0    -1   -1   0   
$EndComp
$Comp
L pspice:R R31
U 1 1 5E773861
P 2900 6350
F 0 "R31" H 2832 6304 50  0000 R CNN
F 1 "10k" H 2832 6395 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 2900 6350 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-10-kohm-250-mw-1-rnd-1206-1-10k-p183394.html?&trstct=pol_1&nbc=1" H 2900 6350 50  0001 C CNN
	1    2900 6350
	-1   0    0    1   
$EndComp
Wire Wire Line
	3200 6100 2900 6100
Connection ~ 2900 6100
Wire Wire Line
	2900 6100 2850 6100
Wire Wire Line
	2900 6600 3050 6600
Wire Wire Line
	3200 6600 3200 6400
$Comp
L power:+3.3V #PWR?
U 1 1 5E77B697
P 3050 6600
AR Path="/5E77B697" Ref="#PWR?"  Part="1" 
AR Path="/5E6C650B/5E77B697" Ref="#PWR026"  Part="1" 
F 0 "#PWR026" H 3050 6450 50  0001 C CNN
F 1 "+3.3V" H 3065 6773 50  0000 C CNN
F 2 "" H 3050 6600 50  0000 C CNN
F 3 "" H 3050 6600 50  0000 C CNN
	1    3050 6600
	-1   0    0    1   
$EndComp
Connection ~ 3050 6600
Wire Wire Line
	3050 6600 3200 6600
Wire Wire Line
	4100 6100 3200 6100
Connection ~ 3200 6100
Text Label 4100 6250 2    50   ~ 0
SH
Text Label 2050 6100 2    50   ~ 0
SH
Text Label 4100 3050 2    50   ~ 0
SH
$Comp
L device:Q_NMOS_GSD Q?
U 1 1 5E7E6EC0
P 9100 5200
AR Path="/5E7E6EC0" Ref="Q?"  Part="1" 
AR Path="/5E6C650B/5E7E6EC0" Ref="Q5"  Part="1" 
F 0 "Q5" H 9291 5246 50  0000 L CNN
F 1 "2N7002" H 9291 5155 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 9300 5300 50  0001 C CNN
F 3 "" H 9100 5200 50  0000 C CNN
	1    9100 5200
	-1   0    0    -1  
$EndComp
$Comp
L pspice:R R36
U 1 1 5E7E8D40
P 9400 5450
F 0 "R36" H 9332 5404 50  0000 R CNN
F 1 "10k" H 9332 5495 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9400 5450 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-910-ohm-250-mw-1-rnd-1206-1-910-p212738.html?&trstct=pol_0&nbc=1" H 9400 5450 50  0001 C CNN
	1    9400 5450
	-1   0    0    1   
$EndComp
$Comp
L pspice:R R35
U 1 1 5E7E9FFC
P 9000 4650
F 0 "R35" H 8932 4604 50  0000 R CNN
F 1 "10k" H 8932 4695 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9000 4650 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-910-ohm-250-mw-1-rnd-1206-1-910-p212738.html?&trstct=pol_0&nbc=1" H 9000 4650 50  0001 C CNN
	1    9000 4650
	-1   0    0    1   
$EndComp
$Comp
L power:+3.3V #PWR0126
U 1 1 5E81AE37
P 9000 4400
F 0 "#PWR0126" H 9000 4250 50  0001 C CNN
F 1 "+3.3V" H 9015 4573 50  0000 C CNN
F 2 "" H 9000 4400 50  0000 C CNN
F 3 "" H 9000 4400 50  0000 C CNN
	1    9000 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 4900 9000 4950
Wire Wire Line
	9000 4950 8800 4950
Connection ~ 9000 4950
Wire Wire Line
	9000 4950 9000 5000
Text Label 8800 4950 2    50   ~ 0
TRIG
Text Label 3850 5150 0    50   ~ 0
TRIG
Wire Wire Line
	9750 3950 9750 5200
Wire Wire Line
	9750 5200 9400 5200
Connection ~ 9400 5200
Wire Wire Line
	9400 5200 9300 5200
$Comp
L power:GND #PWR0127
U 1 1 5E82FF83
P 9000 5400
F 0 "#PWR0127" H 9000 5150 50  0001 C CNN
F 1 "GND" H 9005 5227 50  0000 C CNN
F 2 "" H 9000 5400 50  0000 C CNN
F 3 "" H 9000 5400 50  0000 C CNN
	1    9000 5400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0128
U 1 1 5E830E3F
P 9400 5700
F 0 "#PWR0128" H 9400 5450 50  0001 C CNN
F 1 "GND" H 9405 5527 50  0000 C CNN
F 2 "" H 9400 5700 50  0000 C CNN
F 3 "" H 9400 5700 50  0000 C CNN
	1    9400 5700
	1    0    0    -1  
$EndComp
$Comp
L conn:CONN_01X04 P10
U 1 1 5E893D99
P 9950 3800
F 0 "P10" H 10028 3841 50  0000 L CNN
F 1 "UART" H 10028 3750 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 9950 3800 50  0001 C CNN
F 3 "https://www.reichelt.de/jst-stiftleiste-gerade-1x4-polig-xh-jst-xh4p-st-p185075.html?&trstct=pol_12&nbc=1" H 9950 3800 50  0001 C CNN
F 4 "SYNC" H 9950 3800 50  0001 C CNN "Label"
	1    9950 3800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0129
U 1 1 5E89C466
P 9750 3750
F 0 "#PWR0129" H 9750 3600 50  0001 C CNN
F 1 "+3.3V" H 9765 3923 50  0000 C CNN
F 2 "" H 9750 3750 50  0000 C CNN
F 3 "" H 9750 3750 50  0000 C CNN
	1    9750 3750
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR070
U 1 1 5E89DB35
P 9750 3650
F 0 "#PWR070" H 9750 3400 50  0001 C CNN
F 1 "GND" H 9755 3477 50  0000 C CNN
F 2 "" H 9750 3650 50  0000 C CNN
F 3 "" H 9750 3650 50  0000 C CNN
	1    9750 3650
	0    1    1    0   
$EndComp
$EndSCHEMATC
