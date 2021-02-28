EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 9 9
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
L Device:R R?
U 1 1 605F8E31
P 4200 1350
AR Path="/5FD1FA3F/605F8E31" Ref="R?"  Part="1" 
AR Path="/605F6319/605F8E31" Ref="R13"  Part="1" 
F 0 "R13" V 4300 1350 50  0000 C CNN
F 1 "1k" V 4200 1350 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 4130 1350 50  0001 C CNN
F 3 "~" H 4200 1350 50  0001 C CNN
F 4 "C131398" H 4200 1350 50  0001 C CNN "LCSC"
F 5 "311-1.00KFRCT-ND" H 4200 1350 50  0001 C CNN "Digikey"
	1    4200 1350
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 605F8E37
P 4200 1550
AR Path="/5FD1FA3F/605F8E37" Ref="R?"  Part="1" 
AR Path="/605F6319/605F8E37" Ref="R14"  Part="1" 
F 0 "R14" V 4300 1550 50  0000 C CNN
F 1 "1k" V 4200 1550 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 4130 1550 50  0001 C CNN
F 3 "~" H 4200 1550 50  0001 C CNN
F 4 "C131398" H 4200 1550 50  0001 C CNN "LCSC"
F 5 "311-1.00KFRCT-ND" H 4200 1550 50  0001 C CNN "Digikey"
	1    4200 1550
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 605F8E3D
P 4200 1750
AR Path="/5FD1FA3F/605F8E3D" Ref="R?"  Part="1" 
AR Path="/605F6319/605F8E3D" Ref="R15"  Part="1" 
F 0 "R15" V 4300 1750 50  0000 C CNN
F 1 "1k" V 4200 1750 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 4130 1750 50  0001 C CNN
F 3 "~" H 4200 1750 50  0001 C CNN
F 4 "C131398" H 4200 1750 50  0001 C CNN "LCSC"
F 5 "311-1.00KFRCT-ND" H 4200 1750 50  0001 C CNN "Digikey"
	1    4200 1750
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 605F8E43
P 3300 1550
AR Path="/5FD1FA3F/605F8E43" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/605F8E43" Ref="#PWR0114"  Part="1" 
F 0 "#PWR0114" H 3300 1300 50  0001 C CNN
F 1 "GND" H 3305 1377 50  0000 C CNN
F 2 "" H 3300 1550 50  0001 C CNN
F 3 "" H 3300 1550 50  0001 C CNN
	1    3300 1550
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 605F8E4E
P 4300 3650
AR Path="/5FD1FA3F/605F8E4E" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/605F8E4E" Ref="#PWR0115"  Part="1" 
F 0 "#PWR0115" H 4300 3400 50  0001 C CNN
F 1 "GND" H 4305 3477 50  0000 C CNN
F 2 "" H 4300 3650 50  0001 C CNN
F 3 "" H 4300 3650 50  0001 C CNN
	1    4300 3650
	-1   0    0    -1  
$EndComp
$Comp
L Isolator:SFH617A-1 U?
U 1 1 605F8E58
P 5600 4800
AR Path="/5FD1FA3F/605F8E58" Ref="U?"  Part="1" 
AR Path="/605F6319/605F8E58" Ref="U5"  Part="1" 
F 0 "U5" H 5600 5125 50  0000 C CNN
F 1 "EL357NC" H 5600 5034 50  0000 C CNN
F 2 "optocoupler:EL357N" H 5400 4600 50  0001 L CIN
F 3 "http://www.everlight.com/file/ProductFile/201407061738295695.pdf" H 5600 4800 50  0001 L CNN
F 4 "EL357N(C)(TA)-VG" H 5600 4800 50  0001 C CNN "Partnumber"
F 5 "1080-1601-1-ND" H 5600 4800 50  0001 C CNN "Digikey"
F 6 "C29981" H 5600 4800 50  0001 C CNN "LCSC"
	1    5600 4800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 605F8E5F
P 5150 4700
AR Path="/5FD1FA3F/605F8E5F" Ref="R?"  Part="1" 
AR Path="/605F6319/605F8E5F" Ref="R17"  Part="1" 
F 0 "R17" V 5050 4650 50  0000 L CNN
F 1 "210" V 5150 4650 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 5080 4700 50  0001 C CNN
F 3 "~" H 5150 4700 50  0001 C CNN
F 4 "C488733" H 5150 4700 50  0001 C CNN "LCSC"
F 5 "311-210FRCT-ND" H 5150 4700 50  0001 C CNN "Digikey"
	1    5150 4700
	0    1    -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 605F8E65
P 6250 5150
AR Path="/5FD1FA3F/605F8E65" Ref="R?"  Part="1" 
AR Path="/605F6319/605F8E65" Ref="R20"  Part="1" 
F 0 "R20" V 6150 5150 50  0000 C CNN
F 1 "1k" V 6250 5150 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 6180 5150 50  0001 C CNN
F 3 "~" H 6250 5150 50  0001 C CNN
F 4 "C131398" H 6250 5150 50  0001 C CNN "LCSC"
F 5 "311-1.00KFRCT-ND" H 6250 5150 50  0001 C CNN "Digikey"
	1    6250 5150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 605F8E6B
P 5900 5300
AR Path="/5FD1FA3F/605F8E6B" Ref="R?"  Part="1" 
AR Path="/605F6319/605F8E6B" Ref="R21"  Part="1" 
F 0 "R21" V 5800 5250 50  0000 L CNN
F 1 "100" V 5900 5250 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 5830 5300 50  0001 C CNN
F 3 "~" H 5900 5300 50  0001 C CNN
F 4 "C137392" H 5900 5300 50  0001 C CNN "LCSC"
F 5 "P100FCT-ND" H 5900 5300 50  0001 C CNN "Digikey"
	1    5900 5300
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 605F8E71
P 6000 6050
AR Path="/5FD1FA3F/605F8E71" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/605F8E71" Ref="#PWR0123"  Part="1" 
F 0 "#PWR0123" H 6000 5800 50  0001 C CNN
F 1 "GND" H 6005 5877 50  0000 C CNN
F 2 "" H 6000 6050 50  0001 C CNN
F 3 "" H 6000 6050 50  0001 C CNN
	1    6000 6050
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 605F8E77
P 5900 4700
AR Path="/5FD1FA3F/605F8E77" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/605F8E77" Ref="#PWR0122"  Part="1" 
F 0 "#PWR0122" H 5900 4550 50  0001 C CNN
F 1 "+3.3V" V 5915 4828 50  0000 L CNN
F 2 "" H 5900 4700 50  0001 C CNN
F 3 "" H 5900 4700 50  0001 C CNN
	1    5900 4700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 605F8E7D
P 4500 5750
AR Path="/5FD1FA3F/605F8E7D" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/605F8E7D" Ref="#PWR0117"  Part="1" 
F 0 "#PWR0117" H 4500 5500 50  0001 C CNN
F 1 "GND" H 4505 5577 50  0000 C CNN
F 2 "" H 4500 5750 50  0001 C CNN
F 3 "" H 4500 5750 50  0001 C CNN
	1    4500 5750
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x06_Female J?
U 1 1 605F8E83
P 4300 5550
AR Path="/5FD1FA3F/605F8E83" Ref="J?"  Part="1" 
AR Path="/605F6319/605F8E83" Ref="J6"  Part="1" 
F 0 "J6" H 4328 5526 50  0000 L CNN
F 1 "SYNC" H 4328 5435 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-6_1x06_P5.00mm_Horizontal" H 4300 5550 50  0001 C CNN
F 3 "~" H 4300 5550 50  0001 C CNN
F 4 "C8379" H 4300 5550 50  0001 C CNN "LCSC"
	1    4300 5550
	-1   0    0    1   
$EndComp
Wire Wire Line
	4850 5650 4850 5550
Wire Wire Line
	4850 5550 4500 5550
Wire Wire Line
	5300 5650 4850 5650
Wire Wire Line
	5300 5450 4500 5450
$Comp
L Isolator:SFH617A-1 U?
U 1 1 605F8E90
P 5600 5550
AR Path="/5FD1FA3F/605F8E90" Ref="U?"  Part="1" 
AR Path="/605F6319/605F8E90" Ref="U6"  Part="1" 
F 0 "U6" H 5600 5875 50  0000 C CNN
F 1 "EL357NC" H 5600 5784 50  0000 C CNN
F 2 "optocoupler:EL357N" H 5400 5350 50  0001 L CIN
F 3 "http://www.everlight.com/file/ProductFile/201407061738295695.pdf" H 5600 5550 50  0001 L CNN
F 4 "EL357N(C)(TA)-VG" H 5600 5550 50  0001 C CNN "Partnumber"
F 5 "1080-1601-1-ND" H 5600 5550 50  0001 C CNN "Digikey"
F 6 "C29981" H 5600 5550 50  0001 C CNN "LCSC"
	1    5600 5550
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 605F8E96
P 4550 3900
AR Path="/5FD1FA3F/605F8E96" Ref="R?"  Part="1" 
AR Path="/605F6319/605F8E96" Ref="R12"  Part="1" 
F 0 "R12" V 4450 3900 50  0000 C CNN
F 1 "10" V 4550 3900 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 4480 3900 50  0001 C CNN
F 3 "~" H 4550 3900 50  0001 C CNN
F 4 "C108080" H 4550 3900 50  0001 C CNN "LCSC"
F 5 "CR1206-FX-10R0ELFCT-ND" H 4550 3900 50  0001 C CNN "Digikey"
	1    4550 3900
	0    -1   1    0   
$EndComp
Wire Wire Line
	5150 3900 4700 3900
$Comp
L power:+3.3V #PWR?
U 1 1 605F8E9D
P 5350 3600
AR Path="/5FD1FA3F/605F8E9D" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/605F8E9D" Ref="#PWR0118"  Part="1" 
F 0 "#PWR0118" H 5350 3450 50  0001 C CNN
F 1 "+3.3V" H 5365 3773 50  0000 C CNN
F 2 "" H 5350 3600 50  0001 C CNN
F 3 "" H 5350 3600 50  0001 C CNN
	1    5350 3600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 605F8EA3
P 5350 4200
AR Path="/5FD1FA3F/605F8EA3" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/605F8EA3" Ref="#PWR0119"  Part="1" 
F 0 "#PWR0119" H 5350 3950 50  0001 C CNN
F 1 "GND" H 5355 4027 50  0000 C CNN
F 2 "" H 5350 4200 50  0001 C CNN
F 3 "" H 5350 4200 50  0001 C CNN
	1    5350 4200
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 605F8EA9
P 4950 3300
AR Path="/5FD1FA3F/605F8EA9" Ref="R?"  Part="1" 
AR Path="/605F6319/605F8EA9" Ref="R16"  Part="1" 
F 0 "R16" V 4850 3300 50  0000 C CNN
F 1 "10" V 4950 3300 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 4880 3300 50  0001 C CNN
F 3 "~" H 4950 3300 50  0001 C CNN
F 4 "C108080" H 4950 3300 50  0001 C CNN "LCSC"
F 5 "CR1206-FX-10R0ELFCT-ND" H 4950 3300 50  0001 C CNN "Digikey"
	1    4950 3300
	0    -1   1    0   
$EndComp
Wire Wire Line
	5550 3300 5350 3300
$Comp
L power:+3.3V #PWR?
U 1 1 605F8EB0
P 5750 3000
AR Path="/5FD1FA3F/605F8EB0" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/605F8EB0" Ref="#PWR0120"  Part="1" 
F 0 "#PWR0120" H 5750 2850 50  0001 C CNN
F 1 "+3.3V" H 5765 3173 50  0000 C CNN
F 2 "" H 5750 3000 50  0001 C CNN
F 3 "" H 5750 3000 50  0001 C CNN
	1    5750 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 605F8EB6
P 5750 3600
AR Path="/5FD1FA3F/605F8EB6" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/605F8EB6" Ref="#PWR0121"  Part="1" 
F 0 "#PWR0121" H 5750 3350 50  0001 C CNN
F 1 "GND" H 5755 3427 50  0000 C CNN
F 2 "" H 5750 3600 50  0001 C CNN
F 3 "" H 5750 3600 50  0001 C CNN
	1    5750 3600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4300 3550 4400 3550
Wire Wire Line
	4400 3550 4400 3900
Wire Wire Line
	4300 3450 4800 3450
Wire Wire Line
	4800 3450 4800 3300
$Comp
L Device:D_Schottky_Dual_Series_AKC D?
U 1 1 605F8EC0
P 5350 3900
AR Path="/5FD1FA3F/605F8EC0" Ref="D?"  Part="1" 
AR Path="/605F6319/605F8EC0" Ref="D2"  Part="1" 
F 0 "D2" V 5350 3980 50  0000 L CNN
F 1 "BAT54S" V 5305 3980 50  0001 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5350 3900 50  0001 C CNN
F 3 "~" H 5350 3900 50  0001 C CNN
F 4 "1727-1868-1-ND" H 5350 3900 50  0001 C CNN "Digikey"
F 5 "C47546" H 5350 3900 50  0001 C CNN "LCSC"
	1    5350 3900
	0    1    -1   0   
$EndComp
$Comp
L Device:D_Schottky_Dual_Series_AKC D?
U 1 1 605F8EC6
P 5750 3300
AR Path="/5FD1FA3F/605F8EC6" Ref="D?"  Part="1" 
AR Path="/605F6319/605F8EC6" Ref="D3"  Part="1" 
F 0 "D3" V 5750 3380 50  0000 L CNN
F 1 "BAT54S" V 5705 3380 50  0001 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 5750 3300 50  0001 C CNN
F 3 "~" H 5750 3300 50  0001 C CNN
F 4 "1727-1868-1-ND" H 5750 3300 50  0001 C CNN "Digikey"
F 5 "C47546" H 5750 3300 50  0001 C CNN "LCSC"
	1    5750 3300
	0    1    -1   0   
$EndComp
Wire Wire Line
	5000 4900 5000 5350
Wire Wire Line
	4500 5350 5000 5350
Wire Wire Line
	4850 4700 4850 5250
Wire Wire Line
	4500 5250 4850 5250
Wire Wire Line
	5000 4700 4850 4700
Wire Wire Line
	5000 4900 5300 4900
$Comp
L Connector:AudioJack3 J?
U 1 1 605F8ED8
P 4100 3550
AR Path="/5FD1FA3F/605F8ED8" Ref="J?"  Part="1" 
AR Path="/605F6319/605F8ED8" Ref="J5"  Part="1" 
F 0 "J5" H 3821 3483 50  0000 R CNN
F 1 "AudioJack3" H 3821 3574 50  0000 R CNN
F 2 "connectors_custom:3.5mm_audio_PJ-3126-C" H 4100 3550 50  0001 C CNN
F 3 "~" H 4100 3550 50  0001 C CNN
F 4 "CP1-3523N-ND" H 4100 3550 50  0001 C CNN "Digikey"
F 5 "C145816" H 4100 3550 50  0001 C CNN "LCSC"
	1    4100 3550
	1    0    0    1   
$EndComp
$Comp
L Device:LED_RKGB D?
U 1 1 605F8EDE
P 3500 1550
AR Path="/5FD1FA3F/605F8EDE" Ref="D?"  Part="1" 
AR Path="/605F6319/605F8EDE" Ref="D9"  Part="1" 
F 0 "D9" H 3500 2047 50  0000 C CNN
F 1 "LED_RGBK" H 3500 1956 50  0000 C CNN
F 2 "LED_THT:LED_D5.0mm-4_RGB" H 3500 1500 50  0001 C CNN
F 3 "~" H 3500 1500 50  0001 C CNN
F 4 "C330759" H 3500 1550 50  0001 C CNN "LCSC"
F 5 "1830-1014-ND" H 3500 1550 50  0001 C CNN "Digikey"
	1    3500 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 1350 4050 1350
Wire Wire Line
	4050 1550 3700 1550
Wire Wire Line
	3700 1750 4050 1750
Text Notes 3750 1150 0    50   ~ 0
FIGURE PACKAGE
$Comp
L Device:Q_NMOS_GSD Q?
U 1 1 606958E6
P 7550 5300
AR Path="/5FD1FA3F/606958E6" Ref="Q?"  Part="1" 
AR Path="/605F6319/606958E6" Ref="Q5"  Part="1" 
F 0 "Q5" H 7754 5300 50  0000 L CNN
F 1 "BSS806NE" H 7754 5255 50  0001 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7750 5400 50  0001 C CNN
F 3 "https://www.infineon.com/dgdl/Infineon-BSS806NE-DS-v02_01-en.pdf?fileId=db3a304340f610c201410d1548de3366" H 7550 5300 50  0001 C CNN
F 4 "C91714" H 7550 5300 50  0001 C CNN "LCSC"
F 5 "BSS806NEH6327XTSA1CT-ND" H 7550 5300 50  0001 C CNN "Digikey"
	1    7550 5300
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_2_Open JP?
U 1 1 606958EC
P 6800 5300
AR Path="/5FD1FA3F/606958EC" Ref="JP?"  Part="1" 
AR Path="/605F6319/606958EC" Ref="JP7"  Part="1" 
F 0 "JP7" H 6800 5535 50  0000 C CNN
F 1 "Jumper_2_Open" H 6800 5444 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6800 5300 50  0001 C CNN
F 3 "~" H 6800 5300 50  0001 C CNN
	1    6800 5300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 606958F3
P 7150 5300
AR Path="/5FD1FA3F/606958F3" Ref="R?"  Part="1" 
AR Path="/605F6319/606958F3" Ref="R54"  Part="1" 
F 0 "R54" V 7250 5250 50  0000 L CNN
F 1 "1k" V 7150 5250 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7080 5300 50  0001 C CNN
F 3 "~" H 7150 5300 50  0001 C CNN
F 4 "C131398" H 7150 5300 50  0001 C CNN "LCSC"
F 5 "311-1.00KFRCT-ND" H 7150 5300 50  0001 C CNN "Digikey"
	1    7150 5300
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 606958F9
P 7650 5500
AR Path="/5FD1FA3F/606958F9" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/606958F9" Ref="#PWR0126"  Part="1" 
F 0 "#PWR0126" H 7650 5250 50  0001 C CNN
F 1 "GND" H 7655 5327 50  0000 C CNN
F 2 "" H 7650 5500 50  0001 C CNN
F 3 "" H 7650 5500 50  0001 C CNN
	1    7650 5500
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 606958FF
P 7650 4500
AR Path="/5FD1FA3F/606958FF" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/606958FF" Ref="#PWR0125"  Part="1" 
F 0 "#PWR0125" H 7650 4350 50  0001 C CNN
F 1 "+3.3V" H 7665 4673 50  0000 C CNN
F 2 "" H 7650 4500 50  0001 C CNN
F 3 "" H 7650 4500 50  0001 C CNN
	1    7650 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D?
U 1 1 60695905
P 7650 4650
AR Path="/5FD1FA3F/60695905" Ref="D?"  Part="1" 
AR Path="/605F6319/60695905" Ref="D10"  Part="1" 
F 0 "D10" V 7689 4532 50  0000 R CNN
F 1 "LED" V 7598 4532 50  0000 R CNN
F 2 "LED_SMD:LED_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7650 4650 50  0001 C CNN
F 3 "~" H 7650 4650 50  0001 C CNN
F 4 "732-4989-1-ND" H 7650 4650 50  0001 C CNN "Digikey"
F 5 "C125088" H 7650 4650 50  0001 C CNN "LCSC"
	1    7650 4650
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 6069590B
P 7650 4950
AR Path="/5FD1FA3F/6069590B" Ref="R?"  Part="1" 
AR Path="/605F6319/6069590B" Ref="R56"  Part="1" 
F 0 "R56" H 7720 4996 50  0000 L CNN
F 1 "100" H 7720 4905 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7580 4950 50  0001 C CNN
F 3 "~" H 7650 4950 50  0001 C CNN
F 4 "C137392" H 7650 4950 50  0001 C CNN "LCSC"
F 5 "P100FCT-ND" H 7650 4950 50  0001 C CNN "Digikey"
	1    7650 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 5300 7350 5300
$Comp
L Device:R R?
U 1 1 60695912
P 7350 5450
AR Path="/5FD1FA3F/60695912" Ref="R?"  Part="1" 
AR Path="/605F6319/60695912" Ref="R55"  Part="1" 
F 0 "R55" V 7450 5400 50  0000 L CNN
F 1 "10k" V 7350 5400 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7280 5450 50  0001 C CNN
F 3 "~" H 7350 5450 50  0001 C CNN
F 4 "C132649" H 7350 5450 50  0001 C CNN "LCSC"
F 5 "541-10.0KFCT-ND" H 7350 5450 50  0001 C CNN "Digikey"
	1    7350 5450
	-1   0    0    1   
$EndComp
Connection ~ 7350 5300
$Comp
L power:GND #PWR?
U 1 1 60695919
P 7350 5600
AR Path="/5FD1FA3F/60695919" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/60695919" Ref="#PWR0124"  Part="1" 
F 0 "#PWR0124" H 7350 5350 50  0001 C CNN
F 1 "GND" H 7355 5427 50  0000 C CNN
F 2 "" H 7350 5600 50  0001 C CNN
F 3 "" H 7350 5600 50  0001 C CNN
	1    7350 5600
	1    0    0    -1  
$EndComp
Text HLabel 6250 5300 0    50   Input ~ 0
SYNC
Wire Wire Line
	6250 5450 6250 5300
Wire Wire Line
	6600 5300 6250 5300
Text HLabel 4900 3900 3    50   Input ~ 0
OWB_SENSE
Text HLabel 4350 1350 2    50   Input ~ 0
RED
Text HLabel 4350 1550 2    50   Input ~ 0
GREEN
Text HLabel 4350 1750 2    50   Input ~ 0
BLUE
Text HLabel 6500 3150 2    50   Input ~ 0
DQ
Text HLabel 1350 2700 0    50   Input ~ 0
+3V3
Text HLabel 1350 2800 0    50   Input ~ 0
GND
$Comp
L power:+3.3V #PWR?
U 1 1 607B824A
P 1350 2700
AR Path="/5FD1FA3F/607B824A" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/607B824A" Ref="#PWR0112"  Part="1" 
F 0 "#PWR0112" H 1350 2550 50  0001 C CNN
F 1 "+3.3V" H 1365 2873 50  0000 C CNN
F 2 "" H 1350 2700 50  0001 C CNN
F 3 "" H 1350 2700 50  0001 C CNN
	1    1350 2700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 607BB540
P 1350 2800
AR Path="/5FD1FA3F/607BB540" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/607BB540" Ref="#PWR0113"  Part="1" 
F 0 "#PWR0113" H 1350 2550 50  0001 C CNN
F 1 "GND" H 1355 2627 50  0000 C CNN
F 2 "" H 1350 2800 50  0001 C CNN
F 3 "" H 1350 2800 50  0001 C CNN
	1    1350 2800
	-1   0    0    -1  
$EndComp
Connection ~ 6250 5300
Wire Wire Line
	6250 5000 6250 4900
Wire Wire Line
	6250 4900 5900 4900
$Comp
L Switch:SW_Push SW1
U 1 1 60A9034A
P 6250 4700
F 0 "SW1" V 6204 4848 50  0000 L CNN
F 1 "SW_Push" V 6295 4848 50  0000 L CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H8mm" H 6250 4900 50  0001 C CNN
F 3 "~" H 6250 4900 50  0001 C CNN
F 4 "C294505" H 6250 4700 50  0001 C CNN "LCSC"
F 5 "450-1644-ND" H 6250 4700 50  0001 C CNN "Digikey"
	1    6250 4700
	0    1    1    0   
$EndComp
Connection ~ 6250 4900
$Comp
L power:+3.3V #PWR?
U 1 1 60A90CBA
P 6250 4500
AR Path="/5FD1FA3F/60A90CBA" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/60A90CBA" Ref="#PWR0127"  Part="1" 
F 0 "#PWR0127" H 6250 4350 50  0001 C CNN
F 1 "+3.3V" V 6265 4628 50  0000 L CNN
F 2 "" H 6250 4500 50  0001 C CNN
F 3 "" H 6250 4500 50  0001 C CNN
	1    6250 4500
	1    0    0    -1  
$EndComp
$Comp
L power_custom:V+ #PWR0140
U 1 1 60289099
P 1400 3200
F 0 "#PWR0140" H 1400 3050 50  0001 C CNN
F 1 "V+" V 1415 3328 50  0000 L CNN
F 2 "" H 1400 3200 50  0001 C CNN
F 3 "" H 1400 3200 50  0001 C CNN
	1    1400 3200
	0    1    1    0   
$EndComp
Text HLabel 1400 3200 0    50   Input ~ 0
V+
$Comp
L power_custom:V+ #PWR0141
U 1 1 60290F53
P 5150 6200
F 0 "#PWR0141" H 5150 6050 50  0001 C CNN
F 1 "V+" V 5165 6328 50  0000 L CNN
F 2 "" H 5150 6200 50  0001 C CNN
F 3 "" H 5150 6200 50  0001 C CNN
	1    5150 6200
	0    1    1    0   
$EndComp
$Comp
L Connector:TestPoint_2Pole TP13
U 1 1 604E11D8
P 4950 6200
F 0 "TP13" H 4950 6395 50  0000 C CNN
F 1 "ISYNC" H 4950 6304 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4950 6200 50  0001 C CNN
F 3 "~" H 4950 6200 50  0001 C CNN
	1    4950 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 6200 4750 5650
Wire Wire Line
	4750 5650 4500 5650
$Comp
L Sensor_Temperature:DS18B20Z U?
U 1 1 5FF0A813
P 6800 3600
AR Path="/5FD1FA3F/5FF0A813" Ref="U?"  Part="1" 
AR Path="/605F6319/5FF0A813" Ref="U13"  Part="1" 
F 0 "U13" H 6456 3646 50  0000 R CNN
F 1 "DS18B20Z" H 6456 3555 50  0000 R CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 5800 3350 50  0001 C CNN
F 3 "http://datasheets.maximintegrated.com/en/ds/DS18B20.pdf" H 6650 3850 50  0001 C CNN
F 4 "C97190" H 6800 3600 50  0001 C CNN "LCSC"
F 5 "DS18B20Z+-ND" H 6800 3600 50  0001 C CNN "Digikey"
	1    6800 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 3300 6450 3300
Wire Wire Line
	6450 3300 6450 3900
Wire Wire Line
	6450 3900 6800 3900
$Comp
L power:GND #PWR?
U 1 1 5FF0BB83
P 6800 3900
AR Path="/5FD1FA3F/5FF0BB83" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/5FF0BB83" Ref="#PWR0149"  Part="1" 
F 0 "#PWR0149" H 6800 3650 50  0001 C CNN
F 1 "GND" H 6805 3727 50  0000 C CNN
F 2 "" H 6800 3900 50  0001 C CNN
F 3 "" H 6800 3900 50  0001 C CNN
	1    6800 3900
	-1   0    0    -1  
$EndComp
Connection ~ 6800 3900
$Comp
L Jumper:Jumper_3_Open JP8
U 1 1 5FF0EA1F
P 6500 3000
F 0 "JP8" H 6500 3224 50  0000 C CNN
F 1 "Jumper_3_Open" H 6500 3133 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 6500 3000 50  0001 C CNN
F 3 "~" H 6500 3000 50  0001 C CNN
	1    6500 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 3000 7100 3000
Wire Wire Line
	6250 3000 6050 3000
Wire Wire Line
	6050 3000 6050 2600
Wire Wire Line
	6050 2600 5350 2600
Wire Wire Line
	5350 2600 5350 3300
Connection ~ 5350 3300
Wire Wire Line
	5350 3300 5100 3300
Wire Wire Line
	7100 3000 7100 3600
$Comp
L power:+3.3V #PWR?
U 1 1 5FFA1462
P 5900 5150
AR Path="/5FD1FA3F/5FFA1462" Ref="#PWR?"  Part="1" 
AR Path="/605F6319/5FFA1462" Ref="#PWR077"  Part="1" 
F 0 "#PWR077" H 5900 5000 50  0001 C CNN
F 1 "+3.3V" V 5915 5278 50  0000 L CNN
F 2 "" H 5900 5150 50  0001 C CNN
F 3 "" H 5900 5150 50  0001 C CNN
	1    5900 5150
	1    0    0    -1  
$EndComp
$Comp
L Device:Q_NMOS_GSD Q?
U 1 1 5FFA1BAC
P 6100 5850
AR Path="/5FD1FA3F/5FFA1BAC" Ref="Q?"  Part="1" 
AR Path="/605F6319/5FFA1BAC" Ref="Q7"  Part="1" 
F 0 "Q7" H 6304 5850 50  0000 L CNN
F 1 "BSS806NE" H 6304 5805 50  0001 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 6300 5950 50  0001 C CNN
F 3 "https://www.infineon.com/dgdl/Infineon-BSS806NE-DS-v02_01-en.pdf?fileId=db3a304340f610c201410d1548de3366" H 6100 5850 50  0001 C CNN
F 4 "C91714" H 6100 5850 50  0001 C CNN "LCSC"
F 5 "BSS806NEH6327XTSA1CT-ND" H 6100 5850 50  0001 C CNN "Digikey"
	1    6100 5850
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5FFA93DA
P 6300 5700
AR Path="/5FD1FA3F/5FFA93DA" Ref="R?"  Part="1" 
AR Path="/605F6319/5FFA93DA" Ref="R67"  Part="1" 
F 0 "R67" V 6200 5700 50  0000 C CNN
F 1 "1k" V 6300 5700 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 6230 5700 50  0001 C CNN
F 3 "~" H 6300 5700 50  0001 C CNN
F 4 "C131398" H 6300 5700 50  0001 C CNN "LCSC"
F 5 "311-1.00KFRCT-ND" H 6300 5700 50  0001 C CNN "Digikey"
	1    6300 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 5550 6300 5450
Wire Wire Line
	6300 5450 6250 5450
Wire Wire Line
	6000 5650 5900 5650
$EndSCHEMATC