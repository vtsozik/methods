#!/usr/local/bin/perl

use strict;

my ($file, $line, $date, $year, $month, $day, $int, %rates);
my @mat = ("003.dat", "006.dat", "012.dat", "024.dat", "036.dat", "060.dat", "084.dat", "120.dat");

foreach $file (@mat) {
 open(FH, "< $file") || die "Can't open $file file $!\n";
 while($line = <FH>) {
  chomp($line) ;
  $line =~ s/[ \t]//g;
  ($date, $int) = split(',', $line);
  if($int ne "ND") {
   ($month, $day, $year) = split('\/', $date);
   $date = "$year$month$day";
   $rates{$date}{$file} = $int;
  }
 }
 close(FH); 
}
open(FH, "> merged.dat") || die "Can't open merged.dat file $!\n";
open(FHD, "> diffs.dat") || die "Can't open diffs.dat file $!\n";
print FH "date,\t";
foreach $date (@mat) {
 $date =~ s/\..*//g;
 print FH "$date,\t";
}
print FH "\n";
my ($i, $j, @dates);
@dates = sort keys %rates;
for($i = 0, $j = 0; $i <= $#dates; ++$i) { 
 my @ten = keys %{ $rates{$dates[$i]} };
 if($#ten == $#mat) {
  print FH "$dates[$i],\t";
  foreach $date (sort keys %{ $rates{$dates[$i]} }) {
   print FH "$rates{$dates[$i]}{$date},\t";
   if($j > 0) {
    #my $diff = $rates{$dates[$j]}{$date} - $rates{$dates[$j-1]}{$date};
    printf FHD "%.2f,\t", $rates{$dates[$j]}{$date} - $rates{$dates[$j-1]}{$date};
   } 
  }
  print FH "\n";
  if($j > 0) {
   print FHD "\n";
  }
  $j = $i + 1;
 }
}
close(FHD); 
close(FH); 
