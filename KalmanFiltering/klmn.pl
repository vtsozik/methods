#!/usr/local/bin/perl

use strict;

my $in = "./data.dat";
my $out = "./results.txt";
my $p = 0.10;
my $q = 0.08;
my $r = 0.01;
my($k, $ex, $nex, $nnex, $np, $nnp, $y);

open(IN, "< $in") || die "Can't open $in file: $!\n";
open(OUT, "> $out") || die "Can't open $out file: $!\n";
print OUT "E[x], y, VAR[x], k\n";
$ex = <IN>;
chomp($ex);
while($y = <IN>) {
 chomp($y);
 $nex = $ex;
 $np = $p + $q;
 $k = $np / ($np + $r);
 $nnex = $nex + $k * ($y - $nex) ;
 $nnp = $np * (1 - $k) * (1 - $k) + $r * $k * $k;
 print OUT "$ex, $y, $p, $k\n";
 $ex = $nnex;
 $p = $nnp;
}
close(IN);
close(OUT);
