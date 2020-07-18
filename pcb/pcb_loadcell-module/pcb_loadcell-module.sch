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
P 1850 1450
F 0 "P17" H 1928 1491 50  0000 L CNN
F 1 "MNT_ins" H 1928 1400 50  0000 L CNN
F 2 "MountingHole:MountingHole_4.3mm_M4_Pad_Via" H 1850 1450 50  0001 C CNN
F 3 "" H 1850 1450 50  0000 C CNN
	1    1850 1450
	0    -1   -1   0   
$EndComp
$Comp
L conn:CONN_01X01 P20
U 1 1 5E363021
P 2750 1450
F 0 "P20" H 2828 1491 50  0000 L CNN
F 1 "MNT_shield" H 2828 1400 50  0000 L CNN
F 2 "MountingHole:MountingHole_4.3mm_M4_DIN965" H 2750 1450 50  0001 C CNN
F 3 "" H 2750 1450 50  0000 C CNN
	1    2750 1450
	0    -1   -1   0   
$EndComp
$Comp
L conn:CONN_01X01 P18
U 1 1 5E3B719F
P 2100 1450
F 0 "P18" H 2178 1491 50  0000 L CNN
F 1 "MNT_ins" H 2178 1400 50  0000 L CNN
F 2 "MountingHole:MountingHole_4.3mm_M4_DIN965" H 2100 1450 50  0001 C CNN
F 3 "" H 2100 1450 50  0000 C CNN
	1    2100 1450
	0    -1   -1   0   
$EndComp
$Comp
L conn:CONN_01X01 P19
U 1 1 5E3B74C3
P 2400 1450
F 0 "P19" H 2478 1491 50  0000 L CNN
F 1 "MNT_ins" H 2478 1400 50  0000 L CNN
F 2 "MountingHole:MountingHole_4.3mm_M4_DIN965" H 2400 1450 50  0001 C CNN
F 3 "" H 2400 1450 50  0000 C CNN
	1    2400 1450
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR060
U 1 1 5E3B7786
P 1850 1650
F 0 "#PWR060" H 1850 1400 50  0001 C CNN
F 1 "GND" H 1855 1477 50  0000 C CNN
F 2 "" H 1850 1650 50  0000 C CNN
F 3 "" H 1850 1650 50  0000 C CNN
	1    1850 1650
	1    0    0    -1  
$EndComp
NoConn ~ 2400 1650
NoConn ~ 2100 1650
NoConn ~ 2750 1650
$Sheet
S 8300 950  1450 1850
U 5E6C650B
F0 "Digital" 50
F1 "Digital.sch" 50
F2 "BAT+" I L 8300 2200 50 
F3 "3V3" I L 8300 2450 50 
F4 "GND" I L 8300 2700 50 
F5 "DT" I L 8300 1950 50 
F6 "RATE" I L 8300 1650 50 
F7 "SCK" I L 8300 1800 50 
$EndSheet
$Comp
L modules:BoostConverter U2
U 1 1 5EA1C72A
P 7000 4850
AR Path="/5EA1C72A" Ref="U2"  Part="1" 
AR Path="/5E7B747C/5EA1C72A" Ref="U?"  Part="1" 
F 0 "U2" H 7025 5015 50  0000 C CNN
F 1 "BoostConverter" H 7025 4924 50  0000 C CNN
F 2 "buck:MT3608_module_boost" H 7000 4850 50  0001 C CNN
F 3 "https://www.banggood.com/DC-2V-24V-To-5V-28V-2A-Step-Up-Boost-Converter-Power-Supply-Module-Adjustable-Regulator-Board-p-1566600.html?rmmds=search&cur_warehouse=CN" H 7000 4850 50  0001 C CNN
	1    7000 4850
	1    0    0    -1  
$EndComp
$Comp
L modules:BuckConverter U3
U 1 1 5EA1C730
P 8450 4050
AR Path="/5EA1C730" Ref="U3"  Part="1" 
AR Path="/5E7B747C/5EA1C730" Ref="U?"  Part="1" 
F 0 "U3" H 8475 4215 50  0000 C CNN
F 1 "BuckConverter" H 8475 4124 50  0000 C CNN
F 2 "buck:dsn-mini-360_buck" H 8450 4050 50  0001 C CNN
F 3 "https://www.banggood.com/DC-DC-7-28V-to-5V-3A-Step-Down-Power-Supply-Module-Buck-Converter-Replace-LM2596-p-1536688.html?rmmds=search" H 8450 4050 50  0001 C CNN
	1    8450 4050
	1    0    0    -1  
$EndComp
$Comp
L regul:7805 U4
U 1 1 5EA1C736
P 8750 5000
AR Path="/5EA1C736" Ref="U4"  Part="1" 
AR Path="/5E7B747C/5EA1C736" Ref="U?"  Part="1" 
F 0 "U4" H 8750 5315 50  0000 C CNN
F 1 "7805" H 8750 5224 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 8750 5000 50  0001 C CNN
F 3 "https://www.reichelt.de/spannungsregler-fest-5-v-dpak-mc-7805-cdtg-p189143.html?&trstct=pos_8&nbc=1" H 8750 5000 50  0001 C CNN
	1    8750 5000
	1    0    0    -1  
$EndComp
$Comp
L pspice:C C6
U 1 1 5EA1C73C
P 9450 5250
AR Path="/5EA1C73C" Ref="C6"  Part="1" 
AR Path="/5E7B747C/5EA1C73C" Ref="C?"  Part="1" 
F 0 "C6" H 9628 5296 50  0000 L CNN
F 1 "100n" H 9628 5205 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 9450 5250 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 9450 5250 50  0001 C CNN
	1    9450 5250
	1    0    0    -1  
$EndComp
$Comp
L pspice:C C5
U 1 1 5EA1C742
P 8250 5250
AR Path="/5EA1C742" Ref="C5"  Part="1" 
AR Path="/5E7B747C/5EA1C742" Ref="C?"  Part="1" 
F 0 "C5" H 8428 5296 50  0000 L CNN
F 1 "330n" H 8428 5205 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 8250 5250 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-330nf-50v-125-c-kem-x7r1206-330n-p207157.html?&trstct=pol_4&nbc=1" H 8250 5250 50  0001 C CNN
	1    8250 5250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR021
U 1 1 5EA1C748
P 9450 4900
AR Path="/5EA1C748" Ref="#PWR021"  Part="1" 
AR Path="/5E7B747C/5EA1C748" Ref="#PWR?"  Part="1" 
F 0 "#PWR021" H 9450 4750 50  0001 C CNN
F 1 "+5V" H 9465 5073 50  0000 C CNN
F 2 "" H 9450 4900 50  0000 C CNN
F 3 "" H 9450 4900 50  0000 C CNN
	1    9450 4900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR016
U 1 1 5EA1C74E
P 8750 5250
AR Path="/5EA1C74E" Ref="#PWR016"  Part="1" 
AR Path="/5E7B747C/5EA1C74E" Ref="#PWR?"  Part="1" 
F 0 "#PWR016" H 8750 5000 50  0001 C CNN
F 1 "GND" H 8755 5077 50  0000 C CNN
F 2 "" H 8750 5250 50  0000 C CNN
F 3 "" H 8750 5250 50  0000 C CNN
	1    8750 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR022
U 1 1 5EA1C754
P 9450 5500
AR Path="/5EA1C754" Ref="#PWR022"  Part="1" 
AR Path="/5E7B747C/5EA1C754" Ref="#PWR?"  Part="1" 
F 0 "#PWR022" H 9450 5250 50  0001 C CNN
F 1 "GND" H 9455 5327 50  0000 C CNN
F 2 "" H 9450 5500 50  0000 C CNN
F 3 "" H 9450 5500 50  0000 C CNN
	1    9450 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 5EA1C75A
P 8250 5500
AR Path="/5EA1C75A" Ref="#PWR015"  Part="1" 
AR Path="/5E7B747C/5EA1C75A" Ref="#PWR?"  Part="1" 
F 0 "#PWR015" H 8250 5250 50  0001 C CNN
F 1 "GND" H 8255 5327 50  0000 C CNN
F 2 "" H 8250 5500 50  0000 C CNN
F 3 "" H 8250 5500 50  0000 C CNN
	1    8250 5500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5EA1C760
P 7450 5050
AR Path="/5EA1C760" Ref="#PWR013"  Part="1" 
AR Path="/5E7B747C/5EA1C760" Ref="#PWR?"  Part="1" 
F 0 "#PWR013" H 7450 4800 50  0001 C CNN
F 1 "GND" H 7455 4877 50  0000 C CNN
F 2 "" H 7450 5050 50  0000 C CNN
F 3 "" H 7450 5050 50  0000 C CNN
	1    7450 5050
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
P 6600 5050
AR Path="/5EA1C76C" Ref="#PWR011"  Part="1" 
AR Path="/5E7B747C/5EA1C76C" Ref="#PWR?"  Part="1" 
F 0 "#PWR011" H 6600 4800 50  0001 C CNN
F 1 "GND" H 6605 4877 50  0000 C CNN
F 2 "" H 6600 5050 50  0000 C CNN
F 3 "" H 6600 5050 50  0000 C CNN
	1    6600 5050
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
P 8050 4250
AR Path="/5EA1C778" Ref="#PWR014"  Part="1" 
AR Path="/5E7B747C/5EA1C778" Ref="#PWR?"  Part="1" 
F 0 "#PWR014" H 8050 4000 50  0001 C CNN
F 1 "GND" H 8055 4077 50  0000 C CNN
F 2 "" H 8050 4250 50  0000 C CNN
F 3 "" H 8050 4250 50  0000 C CNN
	1    8050 4250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5EA1C77E
P 8900 4250
AR Path="/5EA1C77E" Ref="#PWR017"  Part="1" 
AR Path="/5E7B747C/5EA1C77E" Ref="#PWR?"  Part="1" 
F 0 "#PWR017" H 8900 4000 50  0001 C CNN
F 1 "GND" H 8905 4077 50  0000 C CNN
F 2 "" H 8900 4250 50  0000 C CNN
F 3 "" H 8900 4250 50  0000 C CNN
	1    8900 4250
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR020
U 1 1 5EA1C784
P 9300 4150
AR Path="/5EA1C784" Ref="#PWR020"  Part="1" 
AR Path="/5E7B747C/5EA1C784" Ref="#PWR?"  Part="1" 
F 0 "#PWR020" H 9300 4000 50  0001 C CNN
F 1 "+3.3V" H 9315 4323 50  0000 C CNN
F 2 "" H 9300 4150 50  0000 C CNN
F 3 "" H 9300 4150 50  0000 C CNN
	1    9300 4150
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
P 6500 4850
AR Path="/5EA1C790" Ref="#PWR09"  Part="1" 
AR Path="/5E7B747C/5EA1C790" Ref="#PWR?"  Part="1" 
F 0 "#PWR09" H 6500 4700 50  0001 C CNN
F 1 "+BATT" H 6515 5023 50  0000 C CNN
F 2 "" H 6500 4850 50  0000 C CNN
F 3 "" H 6500 4850 50  0000 C CNN
	1    6500 4850
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
P 6000 5100
AR Path="/5EA1C7A2" Ref="C3"  Part="1" 
AR Path="/5E7B747C/5EA1C7A2" Ref="C?"  Part="1" 
F 0 "C3" H 6118 5146 50  0000 L CNN
F 1 "10µ" H 6118 5055 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_6.3x5.7" H 6038 4950 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-elko-10-f-50-v-2000-h-eln-rv2-50v100mu-p246741.html?&trstct=pol_2&nbc=12" H 6000 5100 50  0001 C CNN
	1    6000 5100
	1    0    0    -1  
$EndComp
$Comp
L device:C C4
U 1 1 5EA1C7A8
P 6300 5100
AR Path="/5EA1C7A8" Ref="C4"  Part="1" 
AR Path="/5E7B747C/5EA1C7A8" Ref="C?"  Part="1" 
F 0 "C4" H 6415 5146 50  0000 L CNN
F 1 "100n" H 6415 5055 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 6338 4950 50  0001 C CNN
F 3 "https://www.reichelt.de/vielschicht-kerko-100nf-50v-125-c-kem-x7r1206b100n-p207152.html?&trstct=pol_0&nbc=1" H 6300 5100 50  0001 C CNN
	1    6300 5100
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
P 6000 5250
AR Path="/5EA1C7BA" Ref="#PWR07"  Part="1" 
AR Path="/5E7B747C/5EA1C7BA" Ref="#PWR?"  Part="1" 
F 0 "#PWR07" H 6000 5000 50  0001 C CNN
F 1 "GND" H 6005 5077 50  0000 C CNN
F 2 "" H 6000 5250 50  0000 C CNN
F 3 "" H 6000 5250 50  0000 C CNN
	1    6000 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5EA1C7C0
P 6300 5250
AR Path="/5EA1C7C0" Ref="#PWR08"  Part="1" 
AR Path="/5E7B747C/5EA1C7C0" Ref="#PWR?"  Part="1" 
F 0 "#PWR08" H 6300 5000 50  0001 C CNN
F 1 "GND" H 6305 5077 50  0000 C CNN
F 2 "" H 6300 5250 50  0000 C CNN
F 3 "" H 6300 5250 50  0000 C CNN
	1    6300 5250
	1    0    0    -1  
$EndComp
$Comp
L conn:CONN_01X02 P7
U 1 1 5EA1C7C6
P 6600 4100
AR Path="/5EA1C7C6" Ref="P7"  Part="1" 
AR Path="/5E7B747C/5EA1C7C6" Ref="P?"  Part="1" 
F 0 "P7" H 6600 4250 50  0000 C CNN
F 1 "LED_PWR" V 6700 4100 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 6600 4100 50  0001 C CNN
F 3 "https://www.reichelt.de/jst-stiftleiste-gerade-1x2-polig-xh-jst-xh2p-st-p185073.html?&trstct=pos_7&nbc=1" H 6600 4100 50  0001 C CNN
	1    6600 4100
	0    1    -1   0   
$EndComp
$Comp
L power:+BATT #PWR010
U 1 1 5EA1C7CC
P 6550 4300
AR Path="/5EA1C7CC" Ref="#PWR010"  Part="1" 
AR Path="/5E7B747C/5EA1C7CC" Ref="#PWR?"  Part="1" 
F 0 "#PWR010" H 6550 4150 50  0001 C CNN
F 1 "+BATT" H 6565 4473 50  0000 C CNN
F 2 "" H 6550 4300 50  0000 C CNN
F 3 "" H 6550 4300 50  0000 C CNN
	1    6550 4300
	0    -1   -1   0   
$EndComp
$Comp
L pspice:R R1
U 1 1 5EA1C7D2
P 6900 4300
AR Path="/5EA1C7D2" Ref="R1"  Part="1" 
AR Path="/5E7B747C/5EA1C7D2" Ref="R?"  Part="1" 
F 0 "R1" H 6832 4254 50  0000 R CNN
F 1 "270" H 6832 4345 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 6900 4300 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-270-ohm-250-mw-1-rnd-1206-1-270-p183355.html?&trstct=pol_0&nbc=1" H 6900 4300 50  0001 C CNN
	1    6900 4300
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5EA1C7D8
P 7150 4300
AR Path="/5EA1C7D8" Ref="#PWR012"  Part="1" 
AR Path="/5E7B747C/5EA1C7D8" Ref="#PWR?"  Part="1" 
F 0 "#PWR012" H 7150 4050 50  0001 C CNN
F 1 "GND" H 7155 4127 50  0000 C CNN
F 2 "" H 7150 4300 50  0000 C CNN
F 3 "" H 7150 4300 50  0000 C CNN
	1    7150 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 4950 9450 4950
Wire Wire Line
	9450 4950 9450 4900
Wire Wire Line
	9450 5000 9450 4950
Connection ~ 9450 4950
Wire Wire Line
	8250 5000 8250 4950
Wire Wire Line
	8250 4950 8350 4950
Connection ~ 8250 4950
Wire Wire Line
	6600 4950 6500 4950
Wire Wire Line
	6500 4850 6500 4950
Connection ~ 6500 4950
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
	8050 4150 7900 4150
Wire Wire Line
	7850 4950 7900 4950
Wire Wire Line
	6000 4750 6000 4950
Wire Wire Line
	3300 4750 3300 4950
$Comp
L jumper:Jumper_2_Open JP1
U 1 1 5EA1C800
P 7650 4950
AR Path="/5EA1C800" Ref="JP1"  Part="1" 
AR Path="/5E7B747C/5EA1C800" Ref="JP?"  Part="1" 
F 0 "JP1" H 7650 5185 50  0000 C CNN
F 1 "JP_VBOOST" H 7650 5094 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 7650 4950 50  0001 C CNN
F 3 "~" H 7650 4950 50  0001 C CNN
	1    7650 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7900 4150 7900 4950
Connection ~ 7900 4950
Wire Wire Line
	7900 4950 8250 4950
$Comp
L jumper:Jumper_2_Open JP2
U 1 1 5EA1C809
P 9100 4150
AR Path="/5EA1C809" Ref="JP2"  Part="1" 
AR Path="/5E7B747C/5EA1C809" Ref="JP?"  Part="1" 
F 0 "JP2" H 9100 4385 50  0000 C CNN
F 1 "JP_3V3" H 9100 4294 50  0000 C CNN
F 2 "jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm" H 9100 4150 50  0001 C CNN
F 3 "~" H 9100 4150 50  0001 C CNN
	1    9100 4150
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
	6000 4950 6300 4950
Wire Wire Line
	2600 4950 3000 4950
Connection ~ 3000 4950
Wire Wire Line
	3000 4950 3300 4950
Connection ~ 3300 4950
Connection ~ 6000 4950
Connection ~ 6300 4950
Wire Wire Line
	6300 4950 6500 4950
Text Label 7900 4950 0    50   ~ 0
VBOOST
$Comp
L conn:CONN_01X02 P6
U 1 1 5EA1C841
P 4150 5600
AR Path="/5EA1C841" Ref="P6"  Part="1" 
AR Path="/5E7B747C/5EA1C841" Ref="P?"  Part="1" 
F 0 "P6" H 4150 5750 50  0000 C CNN
F 1 "SWITCH" V 4250 5600 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 4150 5600 50  0001 C CNN
F 3 "https://www.reichelt.de/jst-stiftleiste-gerade-1x2-polig-xh-jst-xh2p-st-p185073.html?&trstct=pos_7&nbc=1" H 4150 5600 50  0001 C CNN
	1    4150 5600
	-1   0    0    -1  
$EndComp
Text HLabel 9300 4150 2    50   Input ~ 0
3V3
Text HLabel 9450 4950 2    50   Input ~ 0
5V
$Comp
L power:+BATT #PWR023
U 1 1 5EA340EA
P 8300 2200
AR Path="/5EA340EA" Ref="#PWR023"  Part="1" 
AR Path="/5E7B747C/5EA340EA" Ref="#PWR?"  Part="1" 
F 0 "#PWR023" H 8300 2050 50  0001 C CNN
F 1 "+BATT" H 8315 2373 50  0000 C CNN
F 2 "" H 8300 2200 50  0000 C CNN
F 3 "" H 8300 2200 50  0000 C CNN
	1    8300 2200
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR018
U 1 1 5EA4043E
P 7000 2400
AR Path="/5EA4043E" Ref="#PWR018"  Part="1" 
AR Path="/5E7B747C/5EA4043E" Ref="#PWR?"  Part="1" 
F 0 "#PWR018" H 7000 2250 50  0001 C CNN
F 1 "+5V" H 7015 2573 50  0000 C CNN
F 2 "" H 7000 2400 50  0000 C CNN
F 3 "" H 7000 2400 50  0000 C CNN
	1    7000 2400
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
P 8300 2700
AR Path="/5EA41F87" Ref="#PWR025"  Part="1" 
AR Path="/5E7B747C/5EA41F87" Ref="#PWR?"  Part="1" 
F 0 "#PWR025" H 8300 2450 50  0001 C CNN
F 1 "GND" H 8305 2527 50  0000 C CNN
F 2 "" H 8300 2700 50  0000 C CNN
F 3 "" H 8300 2700 50  0000 C CNN
	1    8300 2700
	0    1    1    0   
$EndComp
$Comp
L power:+3.3V #PWR024
U 1 1 5EA43CD5
P 8300 2450
AR Path="/5EA43CD5" Ref="#PWR024"  Part="1" 
AR Path="/5E7B747C/5EA43CD5" Ref="#PWR?"  Part="1" 
F 0 "#PWR024" H 8300 2300 50  0001 C CNN
F 1 "+3.3V" H 8315 2623 50  0000 C CNN
F 2 "" H 8300 2450 50  0000 C CNN
F 3 "" H 8300 2450 50  0000 C CNN
	1    8300 2450
	0    -1   -1   0   
$EndComp
$Sheet
S 5750 1050 1250 1750
U 5E766EDC
F0 "Analog" 50
F1 "Analog.sch" 50
F2 "RATE" I R 7000 1650 50 
F3 "SCK" I R 7000 1800 50 
F4 "DT" I R 7000 1950 50 
F5 "GND" I R 7000 2700 50 
F6 "+5V" I R 7000 2400 50 
$EndSheet
Wire Wire Line
	8300 1950 7000 1950
Wire Wire Line
	8300 1650 7000 1650
Wire Wire Line
	7000 1800 8300 1800
$Comp
L pspice:R R18
U 1 1 5E7CE219
P 4200 5100
AR Path="/5E7CE219" Ref="R18"  Part="1" 
AR Path="/5E7B747C/5E7CE219" Ref="R?"  Part="1" 
F 0 "R18" H 4132 5054 50  0000 R CNN
F 1 "10k" H 4132 5145 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4200 5100 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-270-ohm-250-mw-1-rnd-1206-1-270-p183355.html?&trstct=pol_0&nbc=1" H 4200 5100 50  0001 C CNN
	1    4200 5100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3300 4750 3950 4750
Wire Wire Line
	3950 5100 3950 4750
Wire Wire Line
	4450 5100 4450 5550
Wire Wire Line
	4450 5550 4350 5550
Connection ~ 4450 5100
$Comp
L power:GND #PWR027
U 1 1 5E7E5A43
P 4350 5650
AR Path="/5E7E5A43" Ref="#PWR027"  Part="1" 
AR Path="/5E7B747C/5E7E5A43" Ref="#PWR?"  Part="1" 
F 0 "#PWR027" H 4350 5400 50  0001 C CNN
F 1 "GND" H 4355 5477 50  0000 C CNN
F 2 "" H 4350 5650 50  0000 C CNN
F 3 "" H 4350 5650 50  0000 C CNN
	1    4350 5650
	1    0    0    -1  
$EndComp
$Comp
L device:D D2
U 1 1 5E7FA503
P 5450 5300
F 0 "D2" H 5450 5085 50  0000 C CNN
F 1 "1n4148" H 5450 5176 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-323_HandSoldering" H 5450 5300 50  0001 C CNN
F 3 "" H 5450 5300 50  0000 C CNN
	1    5450 5300
	1    0    0    1   
$EndComp
$Comp
L device:Q_NMOS_GSD Q3
U 1 1 5E802AEF
P 5050 5300
F 0 "Q3" H 5241 5346 50  0000 L CNN
F 1 "2N7002" H 5241 5255 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23_Handsoldering" H 5250 5400 50  0001 C CNN
F 3 "" H 5050 5300 50  0000 C CNN
	1    5050 5300
	-1   0    0    -1  
$EndComp
$Comp
L pspice:R R19
U 1 1 5E80302F
P 5250 5550
AR Path="/5E80302F" Ref="R19"  Part="1" 
AR Path="/5E7B747C/5E80302F" Ref="R?"  Part="1" 
F 0 "R19" H 5182 5504 50  0000 R CNN
F 1 "10k" H 5182 5595 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5250 5550 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-270-ohm-250-mw-1-rnd-1206-1-270-p183355.html?&trstct=pol_0&nbc=1" H 5250 5550 50  0001 C CNN
	1    5250 5550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5250 5800 5100 5800
Wire Wire Line
	4950 5800 4950 5500
Wire Wire Line
	5300 5300 5250 5300
Connection ~ 5250 5300
$Comp
L power:GND #PWR058
U 1 1 5E81737E
P 5100 5800
AR Path="/5E81737E" Ref="#PWR058"  Part="1" 
AR Path="/5E7B747C/5E81737E" Ref="#PWR?"  Part="1" 
F 0 "#PWR058" H 5100 5550 50  0001 C CNN
F 1 "GND" H 5105 5627 50  0000 C CNN
F 2 "" H 5100 5800 50  0000 C CNN
F 3 "" H 5100 5800 50  0000 C CNN
	1    5100 5800
	1    0    0    -1  
$EndComp
Connection ~ 5100 5800
Wire Wire Line
	5100 5800 4950 5800
$Comp
L pspice:R R20
U 1 1 5E8177DC
P 5600 5550
AR Path="/5E8177DC" Ref="R20"  Part="1" 
AR Path="/5E7B747C/5E8177DC" Ref="R?"  Part="1" 
F 0 "R20" H 5532 5504 50  0000 R CNN
F 1 "10k" H 5532 5595 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 5600 5550 50  0001 C CNN
F 3 "https://www.reichelt.de/smd-widerstand-1206-270-ohm-250-mw-1-rnd-1206-1-270-p183355.html?&trstct=pol_0&nbc=1" H 5600 5550 50  0001 C CNN
	1    5600 5550
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5600 5800 5250 5800
Connection ~ 5250 5800
Wire Wire Line
	4950 5100 4450 5100
Wire Wire Line
	5600 5300 5600 4350
Wire Wire Line
	5600 4350 2200 4350
Connection ~ 5600 5300
Connection ~ 2200 4350
Wire Wire Line
	2200 4350 2200 4250
$Comp
L transistors:IRF7606PBF Q2
U 1 1 5E825197
P 4750 4750
F 0 "Q2" H 4750 5167 50  0000 C CNN
F 1 "Si4403DDY" H 4750 5076 50  0000 C CNN
F 2 "Package_SO:SO-8_3.9x4.9mm_P1.27mm" H 4750 5167 50  0001 C CIN
F 3 "https://www.digikey.com/product-detail/en/vishay-siliconix/SI4403DDY-T1-GE3/SI4403DDY-T1-GE3CT-ND/7622821" H 4750 5076 50  0001 C CNN
	1    4750 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 4900 4450 5100
Wire Wire Line
	4450 4750 3950 4750
Connection ~ 3950 4750
Wire Wire Line
	4450 4550 4450 4650
Wire Wire Line
	4450 4650 4450 4750
Connection ~ 4450 4650
Connection ~ 4450 4750
Wire Wire Line
	6000 4750 5050 4750
Wire Wire Line
	5050 4550 5050 4650
Connection ~ 5050 4750
Connection ~ 5050 4650
Wire Wire Line
	5050 4650 5050 4750
Wire Wire Line
	5050 4850 5050 4750
$EndSCHEMATC
