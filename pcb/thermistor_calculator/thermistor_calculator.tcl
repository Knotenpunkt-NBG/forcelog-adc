# Inputs  for MF52 (B=3950)
set vratio1 0.45
set vratio2 0.80
set ratio_tol1 0.01
set ratio_tol2 0.05
set Rntc_min 4.2e3
set Rntc_max 26e3

# Stepping controls
set tpPriv(step) 100 ;# Step size
set tpPriv(startR)  100  ;#Step start
set tpPriv(maxR1R2) 250e3 ;# Step end
set tpPriv(stop) 0

console show

################################################################
proc get_ratio { Rntc r1 r2 } {
   set Rpara   [expr { ( 1.0* $Rntc * $r2 )/( $Rntc  + $r2)} ]
   set Vratio  [expr { ( 1.0* $Rpara      )/( $Rpara + $r1)} ]
   return $Vratio
}

################################################################
proc within_tol {num val tol} {
   if {$num >= ($val-$tol) && $num <= ($val+$tol)} {return 1}
   return 0
}

################################################################
proc  testTP4056Temp {Rntc1 Rntc2 Ratio1 Ratio2 tol1 tol2} {
global tpPriv
   set op {}
   set tpPriv(found) 0
   for {set r1 $tpPriv(startR)} {$r1<$tpPriv(maxR1R2)} {incr r1 $tpPriv(step)} {
   puts "$r1" ; update
       for {set r2 $tpPriv(startR)} {$r2<$tpPriv(maxR1R2)} {incr r2 $tpPriv(step)} {

          set VratioCalc1  [ get_ratio $Rntc1 $r1 $r2]

          if { [within_tol $VratioCalc1 $Ratio1 $tol1] } { ;# if this is ok check the other ratio

             set VratioCalc2 [ get_ratio $Rntc2 $r1 $r2]

             if { [within_tol $VratioCalc2 $Ratio2 $tol2] } {

                set frat1 [format "%2.3f" $VratioCalc1]
                set frat2 [format "%2.3f" $VratioCalc2]
                puts "Found $r1 $r2 Ratio1 $frat1 Ratio2 $frat2" ; update
                lappend op  "Found $r1 $r2 Ratio1 $frat1 Ratio2 $frat2\n"
                incr tpPriv(found)
             }
          }
          if {$tpPriv(stop)} {break}
       }
       if {$tpPriv(stop)} {break}
   }
   set fh [open op.txt w]
   puts $fh [ join $op ]
   close $fh
}

proc stop {} {set ::tpPriv(stop) 1}

pack [ button .b -text Stop -command stop ]

testTP4056Temp $Rntc_min $Rntc_max $vratio1 $vratio2 $ratio_tol1 $ratio_tol2

puts "\nFound: $tpPriv(found)\n\n"
