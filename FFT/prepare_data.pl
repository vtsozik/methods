#!/usr/bin/perl

use strict;

sub calibrate {
 my $n = 2000;
 my @coeffs = (.11, .37, .53, .71); # primes work much better
 my $pi = 2 * atan2(1,0);
 my ($i,$j,$f,$s);
 for ($i = 0; $i < $n; $i++) {
  $s = 0;
  for ($j = 0; $j <= $#coeffs; $j++) {
   $f = $coeffs[$j] * 2 * $pi * $i;
   if($j%2 eq 0) {
    $s += sin($f);
   } else {
    $s += cos($f);
   }
  }
  print "$s\n";
 }
} 

sub process_share_price() {
 my $fn = 'share_price.csv';
 open(IN, "< $fn") || die "Can't open $fn: $!\n";
 my($line, @colums, $prev, $curr, $ret);
 $prev = 0;
 while($line = <IN>) {
  chomp($line);
  @colums = split(',', $line);
  $curr = $colums[4];
  if(!($curr =~ /^[0-9, ,.]*$/)) {
   next;
  }
  if($prev gt 0) {
   $ret = ($curr - $prev) / $curr;
   print "$ret\n";
  }
  $prev = $curr;
 }
 close(IN);
}

sub process_weather {
 my ($ind) = @_;
 my $fn = 'weather.csv';
 open(IN, "< $fn") || die "Can't open $fn: $!\n";
 my($line, @colums, $val);
 while($line = <IN>) {
  chomp($line);
  @colums = split(',', $line);
  $val = $colums[$ind];
  if(!($val =~ /^[0-9, ,.]*$/)) {
   next;
  }
  print "$val\n";
 }
 close(IN);
}

sub process_radiation {
 my $fn = 'radiation.csv';
 open(IN, "< $fn") || die "Can't open $fn: $!\n";
 my($line, @colums, $val, $ind);
 $ind = 0;
 while($line = <IN>) {
  chomp($line);
  @colums = split(',', $line);
  print "$colums[$ind]\n";
 }
 close(IN);
}

sub usage {
 die "Usage: $0 <-calibrate|-share_price|-weather{1|2|3|...}|-radiation>";
}

do {
 $_ = shift @ARGV;
 case: {
  /^-calibrate$/ && do { calibrate(); last case; };
  /^-share_price$/ && do { process_share_price(); last case; };
  /^-weather([0-9]*)$/ && do { process_weather($1); last case; };
  /^-radiation$/ && do { process_radiation(); last case; };
  &usage();
 }
} while($#ARGV > 0);
