EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 5150 1700 1300 1550
U 5FD1FA19
F0 "POWER" 50
F1 "POWER.sch" 50
F2 "GND" I L 5150 2350 50 
F3 "3V3" I L 5150 2500 50 
F4 "VDD" I L 5150 2150 50 
$EndSheet
$Sheet
S 2950 1800 1500 1450
U 5FD1FA3F
F0 "CONTROLLER" 50
F1 "CONTROLLER.sch" 50
F2 "GND" I R 4450 2350 50 
F3 "3V3" I R 4450 2500 50 
F4 "VDD" I R 4450 2150 50 
$EndSheet
Wire Wire Line
	4450 2150 5150 2150
Wire Wire Line
	5150 2350 4750 2350
Wire Wire Line
	4450 2500 5150 2500
$Comp
L Mechanical:MountingHole H3
U 1 1 611A1CE9
P 2400 4500
F 0 "H3" H 2500 4546 50  0000 L CNN
F 1 "MountingHole" H 2500 4455 50  0000 L CNN
F 2 "MountingHole:MountingHole_4.3mm_M4_Pad_Via" H 2400 4500 50  0001 C CNN
F 3 "~" H 2400 4500 50  0001 C CNN
	1    2400 4500
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 611A1EC5
P 2400 4300
F 0 "H2" H 2500 4346 50  0000 L CNN
F 1 "MountingHole" H 2500 4255 50  0000 L CNN
F 2 "MountingHole:MountingHole_4.3mm_M4_Pad_Via" H 2400 4300 50  0001 C CNN
F 3 "~" H 2400 4300 50  0001 C CNN
	1    2400 4300
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H1
U 1 1 611A21EB
P 2400 4100
F 0 "H1" H 2500 4146 50  0000 L CNN
F 1 "MountingHole" H 2500 4055 50  0000 L CNN
F 2 "MountingHole:MountingHole_4.3mm_M4_Pad_Via" H 2400 4100 50  0001 C CNN
F 3 "~" H 2400 4100 50  0001 C CNN
	1    2400 4100
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 611A25E6
P 4750 2250
F 0 "H4" H 4850 2299 50  0000 L CNN
F 1 "MountingHole_Pad" H 4850 2208 50  0000 L CNN
F 2 "MountingHole:MountingHole_4.3mm_M4_Pad_Via" H 4750 2250 50  0001 C CNN
F 3 "~" H 4750 2250 50  0001 C CNN
	1    4750 2250
	1    0    0    -1  
$EndComp
Connection ~ 4750 2350
Wire Wire Line
	4750 2350 4450 2350
$Comp
L Mechanical:MountingHole_Pad H5
U 1 1 617ADB9E
P 3750 4100
F 0 "H5" H 3850 4149 50  0000 L CNN
F 1 "MountingHole_Pad" H 3850 4058 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad_Via" H 3750 4100 50  0001 C CNN
F 3 "~" H 3750 4100 50  0001 C CNN
	1    3750 4100
	1    0    0    -1  
$EndComp
NoConn ~ 3750 4200
$Comp
L Mechanical:MountingHole_Pad H6
U 1 1 617BC886
P 3750 4550
F 0 "H6" H 3850 4599 50  0000 L CNN
F 1 "MountingHole_Pad" H 3850 4508 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_Pad_Via" H 3750 4550 50  0001 C CNN
F 3 "~" H 3750 4550 50  0001 C CNN
	1    3750 4550
	1    0    0    -1  
$EndComp
NoConn ~ 3750 4650
$EndSCHEMATC
