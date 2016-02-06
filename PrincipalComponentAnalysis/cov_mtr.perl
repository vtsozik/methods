#!/usr/local/bin/perl

use strict;

my (@dat, @cov, @means, $i, $j, $k, $line, @drow, @crow);
open(FHD, "< diffs.dat") || die "Can't open diffs.dat file $!\n";
for($i = 0; $line = <FHD>; ++$i) {
 chomp($line);
 $line =~ s/[\s]//g;
 @drow = split(',', $line);
 for($j = 0; $j <= $#drow; ++$j) { 
  if($i == 0) {
   push(@means, $drow[$j]);
  } else {
   $means[$j] += $drow[$j];
  }
 }
 push(@dat, [@drow]);
}
close(FHD);
for($i = 0; $i <= $#dat; ++$i) {
 @drow = @{$dat[$i]};
 for($j = 0; $j <= $#drow; ++$j) {
  for($k = 0; $k <= $#drow; ++$k) {
   if($i == 0) {
    if($j == 0) {
     $means[$k] /= $#dat;
    }
    if($j <= $k) {
     push(@crow, (($dat[$i][$j] - $means[$j]) * ($dat[$i][$k] - $means[$k])));
    } else {
     push(@crow, 0);
    }
   } else {
    if($j <= $k) {
     $cov[$j][$k] += (($dat[$i][$j] - $means[$j]) * ($dat[$i][$k] - $means[$k])) 
    }
   }
  }
  if($i == 0) {
   push(@cov, [@crow]);
   $#crow = -1;
  }
 } 
}
for($i = 0; $i <= $#cov; ++$i) {
 for($j = 0; $j <= $#cov; ++$j) {
  if($i > $j) {
   $cov[$i][$j] = $cov[$j][$i];
  } else {
   $cov[$i][$j] /= $#dat;
  }
 }
}
open(FHD, "> data.dat") || die "Can't open data.dat file $!\n";
for($i = 0; $i <= $#cov; ++$i) {
 for($j = 0; $j <= $#cov; ++$j) {
  print FHD "$cov[$i][$j] ";
 }
 print FHD "\n";
}
close(FHD);
