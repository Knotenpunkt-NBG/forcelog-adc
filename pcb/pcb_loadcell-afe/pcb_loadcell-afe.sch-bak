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
$Comp
L conn:CONN_01X01 P17
U 1 1 5E361AC8
P 1600 1350
F 0 "P17" H 1678 1391 50  0000 L CNN
F 1 "MNT_ins" H 1678 1300 50  0000 L CNN
F 2 "MountingHole:MountingHole_4.3mm_M4_Pad_Via" H 1600 1350 50  0001 C CNN
F 3 "" H 1600 1350 50  0000 C CNN
	1    1600 1350
	0    -1   -1   0   
$EndComp
$Comp
L conn:CONN_01X01 P20
U 1 1 5E363021
P 2500 1350
F 0 "P20" H 2578 1391 50  0000 L CNN
F 1 "MNT_shield" H 2578 1300 50  0000 L CNN
F 2 "MountingHole:MountingHole_4.3mm_M4_DIN965_Pad" H 2500 1350 50  0001 C CNN
F 3 "" H 2500 1350 50  0000 C CNN
	1    2500 1350
	0    -1   -1   0   
$EndComp
$Comp
L conn:CONN_01X01 P18
U 1 1 5E3B719F
P 1850 1350
F 0 "P18" H 1928 1391 50  0000 L CNN
F 1 "MNT_ins" H 1928 1300 50  0000 L CNN
F 2 "MountingHole:MountingHole_4.3mm_M4_DIN965_Pad" H 1850 1350 50  0001 C CNN
F 3 "" H 1850 1350 50  0000 C CNN
	1    1850 1350
	0    -1   -1   0   
$EndComp
$Comp
L conn:CONN_01X01 P19
U 1 1 5E3B74C3
P 2150 1350
F 0 "P19" H 2228 1391 50  0000 L CNN
F 1 "MNT_ins" H 2228 1300 50  0000 L CNN
F 2 "MountingHole:MountingHole_4.3mm_M4_DIN965_Pad" H 2150 1350 50  0001 C CNN
F 3 "" H 2150 1350 50  0000 C CNN
	1    2150 1350
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR060
U 1 1 5E3B7786
P 2500 1550
F 0 "#PWR060" H 2500 1300 50  0001 C CNN
F 1 "GND" H 2505 1377 50  0000 C CNN
F 2 "" H 2500 1550 50  0000 C CNN
F 3 "" H 2500 1550 50  0000 C CNN
	1    2500 1550
	1    0    0    -1  
$EndComp
NoConn ~ 2150 1550
NoConn ~ 1850 1550
NoConn ~ 1600 1550
$Sheet
S 8300 950  1450 1850
U 5E6C650B
F0 "Digital" 50
F1 "Digital.sch" 50
F2 "BUSY" I L 8300 1700 50 
F3 "BAT+" I L 8300 1800 50 
F4 "3V3" I L 8300 1900 50 
F5 "GND" I L 8300 2000 50 
F6 "HBRIDGE" I L 8300 2100 50 
F7 "V~CS" I L 8300 2200 50 
F8 "~EXT" I L 8300 2300 50 
F9 "VMOSI" I L 8300 2600 50 
F10 "MISO" I L 8300 2400 50 
F11 "CLK" I L 8300 2500 50 
$EndSheet
Text Label 7000 1700 0    50   ~ 0
VBOOST
$Comp
L modules:BoostConverter U2
U 1 1 5EA1C72A
P 7350 4700
AR Path="/5EA1C72A" Ref="U2"  Part="1" 
AR Path="/5E7B747C/5EA1C72A" Ref="U?"  Part="1" 
F 0 "U2" H 7375 4865 50  0000 C CNN
F 1 "BoostConverter" H 7375 4774 50  0000 C CNN
F 2 "buck:MT3608_module_boost" H 7350 4700 50  0001 C CNN
F 3 "https://www.banggood.com/DC-2V-24V-To-5V-28V-2A-Step-Up-Boost-Converter-Power-Supply-Module-Adjustable-Regulator-Board-p-1566600.html?rmmds=search&cur_warehouse=CN" H 7350 4700 50  0001 C CNN
	1    7350 4700
	1    0    0    -1  
$EndComp
$Comp
L modules:BuckConverter U3
U 1 1 5EA1C730
P 8800 3900
AR Path="/5EA1C730" Ref="U3"  Part="1" 
AR Path="/5E7B747C/5EA1C730" Ref="U?"  Part="1" 
F 0 "U3" H 8825 4065 50  0000 C CNN
F 1 "BuckConverter" H 8825 3974 50  0000 C CNN
F 2 "buck:dsn-mini-360_buck" H 8800 3900 50  0001 C CNN
F 3 "https://www.banggood.com/DC-DC-7-28V-to-5V-3A-Step-Down-Power-Supply-Module-Buck-Converter-Replace-LM2596-p-1536688.html?rmmds=search" H 8800 3900 50  0001 C CNN
	1    8800 3900
	1    0    0    -1  
$EndComp
$Comp
L regul:7805 U4
U 1 1 5EA1C736
P 9100 4850
AR Path="/5EA1C736" Ref="U4"  Part="1" 
AR Path="/5E7B747C/5EA1C736" Ref="U?"  Part="1" 
F 0 "U4" H 9100 5165 50  0000 C CNN
F 1 "7805" H 9100 5074 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 9100 4850 50  0001 C CNN
F 3 "https://www.reichelt.de/spannungsregler-fest-5-v-dpak-mc-7805-cdtg-p189143.html?&trstct=pos_8&nbc=1" H 9100 4850 50  0001 C CNN
	1    9100 4850
	1    0    0    -1  
$EndComp
$Comp
L pspice:C C6
U 1 1 5EA1C73C
P 9800 5100
AR Path="/5EA1C73C" Ref="C6"  Part="1" 
AR Path="/5E7B747C/5EA1C73C" Ref="C?"  Part="1" 
F 0 "C6" H 9978 5146 50  0000 L CNN
F 1 "100n" H 9978 5055 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9800 5100 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 9800 5100 50  0001 C CNN
	1    9800 5100
	1    0    0    -1  
$EndComp
$Comp
L pspice:C C5
U 1 1 5EA1C742
P 8600 5100
AR Path="/5EA1C742" Ref="C5"  Part="1" 
AR Path="/5E7B747C/5EA1C742" Ref="C?"  Part="1" 
F 0 "C5" H 8778 5146 50  0000 L CNN
F 1 "330n" H 8778 5055 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8600 5100 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-330nf-50v-125-c-kem-x7r1206-330n-p207157.html?&trstct=pol_4&nbc=1" H 8600 5100 50  0001 C CNN
	1    8600 5100
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR021
U 1 1 5EA1C748
P 9800 4750
AR Path="/5EA1C748" Ref="#PWR021"  Part="1" 
AR Path="/5E7B747C/5EA1C748" Ref="#PWR?"  Part="1" 
F 0 "#PWR021" H 9800 4600 50  0001 C CNN
F 1 "+5V" H 9815 4923 50  0000 C CNN
F 2 "" H 9800 4750 50  0000 C CNN
F 3 "" H 9800 4750 50  0000 C CNN
	1    9800 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5EA1C74E
P 9100 5100
AR Path="/5EA1C74E" Ref="#PWR016"  Part="1" 
AR Path="/5E7B747C/5EA1C74E" Ref="#PWR?"  Part="1" 
F 0 "#PWR016" H 9100 4850 50  0001 C CNN
F 1 "GND" H 9105 4927 50  0000 C CNN
F 2 "" H 9100 5100 50  0000 C CNN
F 3 "" H 9100 5100 50  0000 C CNN
	1    9100 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR022
U 1 1 5EA1C754
P 9800 5350
AR Path="/5EA1C754" Ref="#PWR022"  Part="1" 
AR Path="/5E7B747C/5EA1C754" Ref="#PWR?"  Part="1" 
F 0 "#PWR022" H 9800 5100 50  0001 C CNN
F 1 "GND" H 9805 5177 50  0000 C CNN
F 2 "" H 9800 5350 50  0000 C CNN
F 3 "" H 9800 5350 50  0000 C CNN
	1    9800 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5EA1C75A
P 8600 5350
AR Path="/5EA1C75A" Ref="#PWR015"  Part="1" 
AR Path="/5E7B747C/5EA1C75A" Ref="#PWR?"  Part="1" 
F 0 "#PWR015" H 8600 5100 50  0001 C CNN
F 1 "GND" H 8605 5177 50  0000 C CNN
F 2 "" H 8600 5350 50  0000 C CNN
F 3 "" H 8600 5350 50  0000 C CNN
	1    8600 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5EA1C760
P 7800 4900
AR Path="/5EA1C760" Ref="#PWR013"  Part="1" 
AR Path="/5E7B747C/5EA1C760" Ref="#PWR?"  Part="1" 
F 0 "#PWR013" H 7800 4650 50  0001 C CNN
F 1 "GND" H 7805 4727 50  0000 C CNN
F 2 "" H 7800 4900 50  0000 C CNN
F 3 "" H 7800 4900 50  0000 C CNN
	1    7800 4900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5EA1C766
P 2100 5450
AR Path="/5EA1C766" Ref="#PWR03"  Part="1" 
AR Path="/5E7B747C/5EA1C766" Ref="#PWR?"  Part="1" 
F 0 "#PWR03" H 2100 5200 50  0001 C CNN
F 1 "GND" H 2105 5277 50  0000 C CNN
F 2 "" H 2100 5450 50  0000 C CNN
F 3 "" H 2100 5450 50  0000 C CNN
	1    2100 5450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5EA1C76C
P 6950 4900
AR Path="/5EA1C76C" Ref="#PWR011"  Part="1" 
AR Path="/5E7B747C/5EA1C76C" Ref="#PWR?"  Part="1" 
F 0 "#PWR011" H 6950 4650 50  0001 C CNN
F 1 "GND" H 6955 4727 50  0000 C CNN
F 2 "" H 6950 4900 50  0000 C CNN
F 3 "" H 6950 4900 50  0000 C CNN
	1    6950 4900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5EA1C772
P 2750 5150
AR Path="/5EA1C772" Ref="#PWR04"  Part="1" 
AR Path="/5E7B747C/5EA1C772" Ref="#PWR?"  Part="1" 
F 0 "#PWR04" H 2750 4900 50  0001 C CNN
F 1 "GND" H 2755 4977 50  0000 C CNN
F 2 "" H 2750 5150 50  0000 C CNN
F 3 "" H 2750 5150 50  0000 C CNN
	1    2750 5150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5EA1C778
P 8400 4100
AR Path="/5EA1C778" Ref="#PWR014"  Part="1" 
AR Path="/5E7B747C/5EA1C778" Ref="#PWR?"  Part="1" 
F 0 "#PWR014" H 8400 3850 50  0001 C CNN
F 1 "GND" H 8405 3927 50  0000 C CNN
F 2 "" H 8400 4100 50  0000 C CNN
F 3 "" H 8400 4100 50  0000 C CNN
	1    8400 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5EA1C77E
P 9250 4100
AR Path="/5EA1C77E" Ref="#PWR017"  Part="1" 
AR Path="/5E7B747C/5EA1C77E" Ref="#PWR?"  Part="1" 
F 0 "#PWR017" H 9250 3850 50  0001 C CNN
F 1 "GND" H 9255 3927 50  0000 C CNN
F 2 "" H 9250 4100 50  0000 C CNN
F 3 "" H 9250 4100 50  0000 C CNN
	1    9250 4100
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR020
U 1 1 5EA1C784
P 9650 4000
AR Path="/5EA1C784" Ref="#PWR020"  Part="1" 
AR Path="/5E7B747C/5EA1C784" Ref="#PWR?"  Part="1" 
F 0 "#PWR020" H 9650 3850 50  0001 C CNN
F 1 "+3.3V" H 9665 4173 50  0000 C CNN
F 2 "" H 9650 4000 50  0000 C CNN
F 3 "" H 9650 4000 50  0000 C CNN
	1    9650 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5EA1C78A
P 1850 4550
AR Path="/5EA1C78A" Ref="#PWR02"  Part="1" 
AR Path="/5E7B747C/5EA1C78A" Ref="#PWR?"  Part="1" 
F 0 "#PWR02" H 1850 4300 50  0001 C CNN
F 1 "GND" H 1855 4377 50  0000 C CNN
F 2 "" H 1850 4550 50  0000 C CNN
F 3 "" H 1850 4550 50  0000 C CNN
	1    1850 4550
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR09
U 1 1 5EA1C790
P 6850 4700
AR Path="/5EA1C790" Ref="#PWR09"  Part="1" 
AR Path="/5E7B747C/5EA1C790" Ref="#PWR?"  Part="1" 
F 0 "#PWR09" H 6850 4550 50  0001 C CNN
F 1 "+BATT" H 6865 4873 50  0000 C CNN
F 2 "" H 6850 4700 50  0000 C CNN
F 3 "" H 6850 4700 50  0000 C CNN
	1    6850 4700
	1    0    0    -1  
$EndComp
$Comp
L device:C C1
U 1 1 5EA1C796
P 3000 5100
AR Path="/5EA1C796" Ref="C1"  Part="1" 
AR Path="/5E7B747C/5EA1C796" Ref="C?"  Part="1" 
F 0 "C1" H 3115 5146 50  0000 L CNN
F 1 "100n" H 3115 5055 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 3038 4950 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 3000 5100 50  0001 C CNN
	1    3000 5100
	1    0    0    -1  
$EndComp
$Comp
L device:CP C2
U 1 1 5EA1C79C
P 3300 5100
AR Path="/5EA1C79C" Ref="C2"  Part="1" 
AR Path="/5E7B747C/5EA1C79C" Ref="C?"  Part="1" 
F 0 "C2" H 3418 5146 50  0000 L CNN
F 1 "10µ" H 3418 5055 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.7" H 3338 4950 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-elko-10-f-50-v-2000-h-eln-rv2-50v100mu-p246741.html?&trstct=pol_2&nbc=1" H 3300 5100 50  0001 C CNN
	1    3300 5100
	1    0    0    -1  
$EndComp
$Comp
L device:CP C3
U 1 1 5EA1C7A2
P 6350 4950
AR Path="/5EA1C7A2" Ref="C3"  Part="1" 
AR Path="/5E7B747C/5EA1C7A2" Ref="C?"  Part="1" 
F 0 "C3" H 6468 4996 50  0000 L CNN
F 1 "10µ" H 6468 4905 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.7" H 6388 4800 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-elko-10-f-50-v-2000-h-eln-rv2-50v100mu-p246741.html?&trstct=pol_2&nbc=1" H 6350 4950 50  0001 C CNN
	1    6350 4950
	1    0    0    -1  
$EndComp
$Comp
L device:C C4
U 1 1 5EA1C7A8
P 6650 4950
AR Path="/5EA1C7A8" Ref="C4"  Part="1" 
AR Path="/5E7B747C/5EA1C7A8" Ref="C?"  Part="1" 
F 0 "C4" H 6765 4996 50  0000 L CNN
F 1 "100n" H 6765 4905 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 6688 4800 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 6650 4950 50  0001 C CNN
	1    6650 4950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 5EA1C7AE
P 3000 5250
AR Path="/5EA1C7AE" Ref="#PWR05"  Part="1" 
AR Path="/5E7B747C/5EA1C7AE" Ref="#PWR?"  Part="1" 
F 0 "#PWR05" H 3000 5000 50  0001 C CNN
F 1 "GND" H 3005 5077 50  0000 C CNN
F 2 "" H 3000 5250 50  0000 C CNN
F 3 "" H 3000 5250 50  0000 C CNN
	1    3000 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5EA1C7B4
P 3300 5250
AR Path="/5EA1C7B4" Ref="#PWR06"  Part="1" 
AR Path="/5E7B747C/5EA1C7B4" Ref="#PWR?"  Part="1" 
F 0 "#PWR06" H 3300 5000 50  0001 C CNN
F 1 "GND" H 3305 5077 50  0000 C CNN
F 2 "" H 3300 5250 50  0000 C CNN
F 3 "" H 3300 5250 50  0000 C CNN
	1    3300 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5EA1C7BA
P 6350 5100
AR Path="/5EA1C7BA" Ref="#PWR07"  Part="1" 
AR Path="/5E7B747C/5EA1C7BA" Ref="#PWR?"  Part="1" 
F 0 "#PWR07" H 6350 4850 50  0001 C CNN
F 1 "GND" H 6355 4927 50  0000 C CNN
F 2 "" H 6350 5100 50  0000 C CNN
F 3 "" H 6350 5100 50  0000 C CNN
	1    6350 5100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5EA1C7C0
P 6650 5100
AR Path="/5EA1C7C0" Ref="#PWR08"  Part="1" 
AR Path="/5E7B747C/5EA1C7C0" Ref="#PWR?"  Part="1" 
F 0 "#PWR08" H 6650 4850 50  0001 C CNN
F 1 "GND" H 6655 4927 50  0000 C CNN
F 2 "" H 6650 5100 50  0000 C CNN
F 3 "" H 6650 5100 50  0000 C CNN
	1    6650 5100
	1    0    0    -1  
$EndComp
$Comp
L conn:CONN_01X02 P7
U 1 1 5EA1C7C6
P 6950 3950
AR Path="/5EA1C7C6" Ref="P7"  Part="1" 
AR Path="/5E7B747C/5EA1C7C6" Ref="P?"  Part="1" 
F 0 "P7" H 6950 4100 50  0000 C CNN
F 1 "LED_PWR" V 7050 3950 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 6950 3950 50  0001 C CNN
F 3 "https://www.reichelt.de/jst-stiftleiste-gerade-1x2-polig-xh-jst-xh2p-st-p185073.html?&trstct=pos_7&nbc=1" H 6950 3950 50  0001 C CNN
	1    6950 3950
	0    1    -1   0   
$EndComp
$Comp
L power:+BATT #PWR010
U 1 1 5EA1C7CC
P 6900 4150
AR Path="/5EA1C7CC" Ref="#PWR010"  Part="1" 
AR Path="/5E7B747C/5EA1C7CC" Ref="#PWR?"  Part="1" 
F 0 "#PWR010" H 6900 4000 50  0001 C CNN
F 1 "+BATT" H 6915 4323 50  0000 C CNN
F 2 "" H 6900 4150 50  0000 C CNN
F 3 "" H 6900 4150 50  0000 C CNN
	1    6900 4150
	0    -1   -1   0   
$EndComp
$Comp
L pspice:R R1
U 1 1 5EA1C7D2
P 7250 4150
AR Path="/5EA1C7D2" Ref="R1"  Part="1" 
AR Path="/5E7B747C/5EA1C7D2" Ref="R?"  Part="1" 
F 0 "R1" H 7182 4104 50  0000 R CNN
F 1 "270" H 7182 4195 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 7250 4150 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-270-ohm-250-mw-1-rnd-1206-1-270-p183355.html?&trstct=pol_0&nbc=1" H 7250 4150 50  0001 C CNN
	1    7250 4150
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5EA1C7D8
P 7500 4150
AR Path="/5EA1C7D8" Ref="#PWR012"  Part="1" 
AR Path="/5E7B747C/5EA1C7D8" Ref="#PWR?"  Part="1" 
F 0 "#PWR012" H 7500 3900 50  0001 C CNN
F 1 "GND" H 7505 3977 50  0000 C CNN
F 2 "" H 7500 4150 50  0000 C CNN
F 3 "" H 7500 4150 50  0000 C CNN
	1    7500 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 4800 9800 4800
Wire Wire Line
	9800 4800 9800 4750
Wire Wire Line
	9800 4850 9800 4800
Connection ~ 9800 4800
Wire Wire Line
	8600 4850 8600 4800
Wire Wire Line
	8600 4800 8700 4800
Connection ~ 8600 4800
Wire Wire Line
	6950 4800 6850 4800
Wire Wire Line
	6850 4700 6850 4800
Connection ~ 6850 4800
$Comp
L modules:TP4056withprotect U1
U 1 1 5EA1C7E8
P 2100 4800
AR Path="/5EA1C7E8" Ref="U1"  Part="1" 
AR Path="/5E7B747C/5EA1C7E8" Ref="U?"  Part="1" 
F 0 "U1" H 2325 4875 50  0000 C CNN
F 1 "TP4056withprotect" H 2125 4300 50  0000 C CNN
F 2 "buck:TP4056_module_prot" H 2100 4750 50  0001 C CNN
F 3 "https://www.banggood.com/TP4056-Micro-USB-5V-1A-Lithium-Battery-Charging-Module-Board-TE585-Lipo-Charger-p-1225993.html?rmmds=search&cur_warehouse=CN" H 2100 4750 50  0001 C CNN
	1    2100 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 4500 2100 4600
Wire Wire Line
	1950 4500 1850 4500
Wire Wire Line
	1850 4500 1850 4550
Wire Wire Line
	2750 5150 2750 5100
Wire Wire Line
	2750 5100 2600 5100
$Comp
L conn:CONN_01X02 P2
U 1 1 5EA1C7F3
P 1200 5000
AR Path="/5EA1C7F3" Ref="P2"  Part="1" 
AR Path="/5E7B747C/5EA1C7F3" Ref="P?"  Part="1" 
F 0 "P2" H 1200 5150 50  0000 C CNN
F 1 "BAT" H 1300 5000 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 1200 5000 50  0001 C CNN
F 3 "https://www.reichelt.de/jst-stiftleiste-gerade-1x2-polig-xh-jst-xh2p-st-p185073.html?&trstct=pos_7&nbc=1" H 1200 5000 50  0001 C CNN
	1    1200 5000
	-1   0    0    1   
$EndComp
Wire Wire Line
	1400 5050 1400 5100
Wire Wire Line
	1400 5100 1600 5100
Wire Wire Line
	1400 4950 1500 4950
Wire Wire Line
	8400 4000 8250 4000
Wire Wire Line
	8200 4800 8250 4800
Wire Wire Line
	3300 4750 3300 4950
$Comp
L jumper:Jumper_2_Open JP1
U 1 1 5EA1C800
P 8000 4800
AR Path="/5EA1C800" Ref="JP1"  Part="1" 
AR Path="/5E7B747C/5EA1C800" Ref="JP?"  Part="1" 
F 0 "JP1" H 8000 5035 50  0000 C CNN
F 1 "JP_VBOOST" H 8000 4944 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 8000 4800 50  0001 C CNN
F 3 "~" H 8000 4800 50  0001 C CNN
	1    8000 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 4000 8250 4800
Connection ~ 8250 4800
Wire Wire Line
	8250 4800 8600 4800
$Comp
L jumper:Jumper_2_Open JP2
U 1 1 5EA1C809
P 9450 4000
AR Path="/5EA1C809" Ref="JP2"  Part="1" 
AR Path="/5E7B747C/5EA1C809" Ref="JP?"  Part="1" 
F 0 "JP2" H 9450 4235 50  0000 C CNN
F 1 "JP_3V3" H 9450 4144 50  0000 C CNN
F 2 "jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 9450 4000 50  0001 C CNN
F 3 "~" H 9450 4000 50  0001 C CNN
	1    9450 4000
	1    0    0    -1  
$EndComp
$Comp
L conn:CONN_01X01 P3
U 1 1 5EA1C80F
P 1500 4700
AR Path="/5EA1C80F" Ref="P3"  Part="1" 
AR Path="/5E7B747C/5EA1C80F" Ref="P?"  Part="1" 
F 0 "P3" H 1578 4741 50  0000 L CNN
F 1 "TP_BAT" H 1578 4650 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 1500 4700 50  0001 C CNN
F 3 "" H 1500 4700 50  0000 C CNN
	1    1500 4700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1500 4900 1500 4950
Connection ~ 1500 4950
Wire Wire Line
	1500 4950 1600 4950
$Comp
L conn:CONN_01X01 P1
U 1 1 5EA1C818
P 1000 4300
AR Path="/5EA1C818" Ref="P1"  Part="1" 
AR Path="/5E7B747C/5EA1C818" Ref="P?"  Part="1" 
F 0 "P1" H 1078 4341 50  0000 L CNN
F 1 "TP_GND" H 1078 4250 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 1000 4300 50  0001 C CNN
F 3 "" H 1000 4300 50  0000 C CNN
	1    1000 4300
	0    -1   -1   0   
$EndComp
$Comp
L conn:CONN_01X02 P5
U 1 1 5EA1C81E
P 2750 4650
AR Path="/5EA1C81E" Ref="P5"  Part="1" 
AR Path="/5E7B747C/5EA1C81E" Ref="P?"  Part="1" 
F 0 "P5" H 2750 4800 50  0000 C CNN
F 1 "MOD_CHRG" V 2850 4650 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2750 4650 50  0001 C CNN
F 3 "" H 2750 4650 50  0000 C CNN
	1    2750 4650
	0    -1   1    0   
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5EA1C824
P 1000 4500
AR Path="/5EA1C824" Ref="#PWR01"  Part="1" 
AR Path="/5E7B747C/5EA1C824" Ref="#PWR?"  Part="1" 
F 0 "#PWR01" H 1000 4250 50  0001 C CNN
F 1 "GND" H 1005 4327 50  0000 C CNN
F 2 "" H 1000 4500 50  0000 C CNN
F 3 "" H 1000 4500 50  0000 C CNN
	1    1000 4500
	1    0    0    -1  
$EndComp
$Comp
L conn:CONN_01X04 P4
U 1 1 5EA1C82A
P 2250 4050
AR Path="/5EA1C82A" Ref="P4"  Part="1" 
AR Path="/5E7B747C/5EA1C82A" Ref="P?"  Part="1" 
F 0 "P4" H 2328 4091 50  0000 L CNN
F 1 "CHRG" H 2328 4000 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 2250 4050 50  0001 C CNN
F 3 "https://www.reichelt.de/jst-stiftleiste-gerade-1x4-polig-xh-jst-xh4p-st-p185075.html?&trstct=pol_12&nbc=1" H 2250 4050 50  0001 C CNN
	1    2250 4050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2100 4500 2200 4500
Wire Wire Line
	2200 4500 2200 4350
Wire Wire Line
	1950 4500 1950 4250
Wire Wire Line
	1950 4250 2100 4250
Wire Wire Line
	2300 4250 2300 4450
Wire Wire Line
	2300 4450 2700 4450
Wire Wire Line
	2400 4250 2800 4250
Wire Wire Line
	2800 4250 2800 4450
Wire Wire Line
	6350 4800 6650 4800
Wire Wire Line
	2600 4950 3000 4950
Connection ~ 3000 4950
Wire Wire Line
	3000 4950 3300 4950
Connection ~ 3300 4950
Connection ~ 6350 4800
Connection ~ 6650 4800
Wire Wire Line
	6650 4800 6850 4800
Text Label 8250 4800 0    50   ~ 0
VBOOST
$Comp
L conn:CONN_01X02 P6
U 1 1 5EA1C841
P 4200 5700
AR Path="/5EA1C841" Ref="P6"  Part="1" 
AR Path="/5E7B747C/5EA1C841" Ref="P?"  Part="1" 
F 0 "P6" H 4200 5850 50  0000 C CNN
F 1 "SWITCH" V 4300 5700 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 4200 5700 50  0001 C CNN
F 3 "https://www.reichelt.de/jst-stiftleiste-gerade-1x2-polig-xh-jst-xh2p-st-p185073.html?&trstct=pos_7&nbc=1" H 4200 5700 50  0001 C CNN
	1    4200 5700
	-1   0    0    -1  
$EndComp
Text HLabel 9650 4000 2    50   Input ~ 0
3V3
Text HLabel 9800 4800 2    50   Input ~ 0
5V
$Comp
L power:+BATT #PWR023
U 1 1 5EA340EA
P 8300 1800
AR Path="/5EA340EA" Ref="#PWR023"  Part="1" 
AR Path="/5E7B747C/5EA340EA" Ref="#PWR?"  Part="1" 
F 0 "#PWR023" H 8300 1650 50  0001 C CNN
F 1 "+BATT" H 8315 1973 50  0000 C CNN
F 2 "" H 8300 1800 50  0000 C CNN
F 3 "" H 8300 1800 50  0000 C CNN
	1    8300 1800
	0    -1   -1   0   
$EndComp
Text Label 8300 2200 2    50   ~ 0
~CS
Text Label 7000 1950 0    50   ~ 0
~CS
Text Label 7000 2100 0    50   ~ 0
~EXT
Text Label 8300 2300 2    50   ~ 0
~EXT
Text Label 8300 2100 2    50   ~ 0
HBRIDGE
Text Label 7000 2500 0    50   ~ 0
HBRIDGE
$Comp
L power:+5V #PWR018
U 1 1 5EA4043E
P 7000 2600
AR Path="/5EA4043E" Ref="#PWR018"  Part="1" 
AR Path="/5E7B747C/5EA4043E" Ref="#PWR?"  Part="1" 
F 0 "#PWR018" H 7000 2450 50  0001 C CNN
F 1 "+5V" H 7015 2773 50  0000 C CNN
F 2 "" H 7000 2600 50  0000 C CNN
F 3 "" H 7000 2600 50  0000 C CNN
	1    7000 2600
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR019
U 1 1 5EA41A6B
P 7000 2700
AR Path="/5EA41A6B" Ref="#PWR019"  Part="1" 
AR Path="/5E7B747C/5EA41A6B" Ref="#PWR?"  Part="1" 
F 0 "#PWR019" H 7000 2450 50  0001 C CNN
F 1 "GND" H 7005 2527 50  0000 C CNN
F 2 "" H 7000 2700 50  0000 C CNN
F 3 "" H 7000 2700 50  0000 C CNN
	1    7000 2700
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR025
U 1 1 5EA41F87
P 8300 2000
AR Path="/5EA41F87" Ref="#PWR025"  Part="1" 
AR Path="/5E7B747C/5EA41F87" Ref="#PWR?"  Part="1" 
F 0 "#PWR025" H 8300 1750 50  0001 C CNN
F 1 "GND" H 8305 1827 50  0000 C CNN
F 2 "" H 8300 2000 50  0000 C CNN
F 3 "" H 8300 2000 50  0000 C CNN
	1    8300 2000
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR024
U 1 1 5EA43CD5
P 8300 1900
AR Path="/5EA43CD5" Ref="#PWR024"  Part="1" 
AR Path="/5E7B747C/5EA43CD5" Ref="#PWR?"  Part="1" 
F 0 "#PWR024" H 8300 1750 50  0001 C CNN
F 1 "+3.3V" H 8315 2073 50  0000 C CNN
F 2 "" H 8300 1900 50  0000 C CNN
F 3 "" H 8300 1900 50  0000 C CNN
	1    8300 1900
	0    -1   -1   0   
$EndComp
Text Label 7000 2300 0    50   ~ 0
BUSY
Text Label 8300 1700 2    50   ~ 0
BUSY
Text Label 8300 2500 2    50   ~ 0
CLK
Text Label 7000 2200 0    50   ~ 0
CLK
Text Label 8300 2600 2    50   ~ 0
MOSI
Text Label 7000 2400 0    50   ~ 0
MOSI
$Sheet
S 5750 1050 1250 1750
U 5E766EDC
F0 "Analog" 50
F1 "Analog.sch" 50
F2 "VBOOST" I R 7000 1700 50 
F3 "MISO" I R 7000 1800 50 
F4 "~CS" I R 7000 1950 50 
F5 "~EXT" I R 7000 2100 50 
F6 "CLK" I R 7000 2200 50 
F7 "BUSY" I R 7000 2300 50 
F8 "MOSI" I R 7000 2400 50 
F9 "HBRIDGE" I R 7000 2500 50 
F10 "5V" I R 7000 2600 50 
F11 "GND" I R 7000 2700 50 
$EndSheet
Wire Wire Line
	8300 2400 7750 2400
Wire Wire Line
	7750 2400 7750 1800
Wire Wire Line
	7750 1800 7000 1800
$Comp
L transistors:IRF7606PBF Q3
U 1 1 5E7C2D3F
P 4700 4750
F 0 "Q3" H 4700 5167 50  0000 C CNN
F 1 "Si4403DDY" H 4700 5076 50  0000 C CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 4700 5167 50  0001 C CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf7606pbf.pdf" H 4700 5076 50  0001 C CNN
	1    4700 4750
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R32
U 1 1 5E7C9825
P 4150 5250
AR Path="/5E7C9825" Ref="R32"  Part="1" 
AR Path="/5E7B747C/5E7C9825" Ref="R?"  Part="1" 
F 0 "R32" H 4082 5204 50  0000 R CNN
F 1 "10k" H 4082 5295 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4150 5250 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-270-ohm-250-mw-1-rnd-1206-1-270-p183355.html?&trstct=pol_0&nbc=1" H 4150 5250 50  0001 C CNN
	1    4150 5250
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0124
U 1 1 5E7D38BB
P 4400 5750
AR Path="/5E7D38BB" Ref="#PWR0124"  Part="1" 
AR Path="/5E7B747C/5E7D38BB" Ref="#PWR?"  Part="1" 
F 0 "#PWR0124" H 4400 5500 50  0001 C CNN
F 1 "GND" H 4405 5577 50  0000 C CNN
F 2 "" H 4400 5750 50  0000 C CNN
F 3 "" H 4400 5750 50  0000 C CNN
	1    4400 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 4900 4400 5250
Connection ~ 4400 5250
Wire Wire Line
	4400 5250 4400 5650
Wire Wire Line
	3900 5250 3900 4750
Wire Wire Line
	3300 4750 3900 4750
Wire Wire Line
	4400 4750 4400 4650
Connection ~ 4400 4650
Wire Wire Line
	4400 4650 4400 4550
Wire Wire Line
	4400 4750 3900 4750
Connection ~ 4400 4750
Connection ~ 3900 4750
$Comp
L device:Q_NMOS_GSD Q4
U 1 1 5E7DA45D
P 5200 5450
F 0 "Q4" H 5391 5496 50  0000 L CNN
F 1 "2N7002" H 5391 5405 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 5400 5550 50  0001 C CNN
F 3 "" H 5200 5450 50  0000 C CNN
	1    5200 5450
	-1   0    0    -1  
$EndComp
$Comp
L pspice:R R33
U 1 1 5E7DBA5C
P 5500 5700
AR Path="/5E7DBA5C" Ref="R33"  Part="1" 
AR Path="/5E7B747C/5E7DBA5C" Ref="R?"  Part="1" 
F 0 "R33" H 5432 5654 50  0000 R CNN
F 1 "10k" H 5432 5745 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5500 5700 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-270-ohm-250-mw-1-rnd-1206-1-270-p183355.html?&trstct=pol_0&nbc=1" H 5500 5700 50  0001 C CNN
	1    5500 5700
	1    0    0    -1  
$EndComp
$Comp
L device:D D6
U 1 1 5E7DD1A6
P 5750 5450
F 0 "D6" H 5750 5665 50  0000 C CNN
F 1 "1N4148" H 5750 5574 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-323_HandSoldering" H 5750 5450 50  0001 C CNN
F 3 "" H 5750 5450 50  0000 C CNN
	1    5750 5450
	1    0    0    -1  
$EndComp
$Comp
L pspice:R R34
U 1 1 5E7DD86E
P 6000 5700
AR Path="/5E7DD86E" Ref="R34"  Part="1" 
AR Path="/5E7B747C/5E7DD86E" Ref="R?"  Part="1" 
F 0 "R34" H 5932 5654 50  0000 R CNN
F 1 "10k" H 5932 5745 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 6000 5700 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-270-ohm-250-mw-1-rnd-1206-1-270-p183355.html?&trstct=pol_0&nbc=1" H 6000 5700 50  0001 C CNN
	1    6000 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 5650 5100 5950
Wire Wire Line
	5100 5950 5300 5950
Connection ~ 5500 5950
Wire Wire Line
	5500 5950 6000 5950
$Comp
L power:GND #PWR0125
U 1 1 5E7DEAD8
P 5300 5950
AR Path="/5E7DEAD8" Ref="#PWR0125"  Part="1" 
AR Path="/5E7B747C/5E7DEAD8" Ref="#PWR?"  Part="1" 
F 0 "#PWR0125" H 5300 5700 50  0001 C CNN
F 1 "GND" H 5305 5777 50  0000 C CNN
F 2 "" H 5300 5950 50  0000 C CNN
F 3 "" H 5300 5950 50  0000 C CNN
	1    5300 5950
	1    0    0    -1  
$EndComp
Connection ~ 5300 5950
Wire Wire Line
	5300 5950 5500 5950
Wire Wire Line
	5400 5450 5500 5450
Connection ~ 5500 5450
Wire Wire Line
	5500 5450 5600 5450
Wire Wire Line
	5900 5450 6000 5450
Wire Wire Line
	5100 5250 4400 5250
Wire Wire Line
	6000 4750 5000 4750
Wire Wire Line
	5000 4550 5000 4650
Connection ~ 5000 4650
Wire Wire Line
	5000 4650 5000 4750
Connection ~ 5000 4750
Wire Wire Line
	5000 4750 5000 4850
Wire Wire Line
	6350 4800 6000 4800
Wire Wire Line
	6000 4800 6000 4750
Wire Wire Line
	6000 5450 6000 5050
Wire Wire Line
	6000 5050 5600 5050
Wire Wire Line
	5600 5050 5600 4150
Wire Wire Line
	5600 4150 3400 4150
Wire Wire Line
	3400 4150 3400 4350
Wire Wire Line
	3400 4350 2200 4350
Connection ~ 6000 5450
Connection ~ 2200 4350
Wire Wire Line
	2200 4350 2200 4250
$EndSCHEMATC
