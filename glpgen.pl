#!/usr/bin/perl
use strict;
use warnings;
use POSIX; # to use ceil and floor
#open a file
#write lines 1 2 3 4 5 6
#loop for 2048 and 2048 with step size 10,increase by 10
#write RECT 
my $step = 1024;
my $layout_x=2048;
my $layout_y=2048;
my $origin=0;
my $filename = "all".$step.".glp";
my $loop_x=0;
my $loop_y=0;
my $count=0;

open(my $fh, '>', $filename) or die "Could not open file '$filename' $!";
print $fh "BEGIN\n";
print $fh "EQUIV  1  1000  MICRON  +X,+Y\n";
print $fh "CNAME U\n";
print $fh "LEVEL M1OPC\n";
print $fh "\n";
print $fh "CELL U PRIME\n";
if ($layout_x % $step==0){
    $loop_x = int(floor($layout_x / $step));
} else {
 $loop_x = int (floor($layout_x / $step) + 1);
}
if ($layout_y % $step==0){
    $loop_y = int(floor($layout_y / $step));
} else {
 $loop_y = int (floor($layout_y / $step) + 1);
}
print "segment in x is ".$loop_x;
print "\n";
print "segment in y is ".$loop_y;
print "\n";
for( my $i = 1; $i < $loop_x+1; $i = $i + 1 ) {
    my $lrx= $origin+$i*$step; #lower right x of rectangle
    if($lrx>$layout_x){
        $lrx=$layout_x;
    }
    for( my $j = 1; $j < $loop_y+1; $j = $j + 1 ) {
        my $lry= $origin+$j*$step; #lower right y of rectangle
        if($lry>$layout_y){
            $lry=$layout_y;
        }
        my $llx=$origin;
        my $lly=$lry;
        print $fh "RECT N M1OPC  $llx $lly $lrx $lry\n";
        $count++;
    }
}
print $fh "ENDMSG";
close($fh);

print "total rectangle $count \n";

