#!/usr/local/bin/perl

print "--- Checking test directory for integrity ---\n";

open (IN, "< cases") || die $!;

while (<IN>)
{
    if (/(.+\.cool);/)
    {
	$f = $1;
	if (!(-e "$f")) { print "** cases include $f, but it's missing!\n"; }
	elsif (!(-e "$f.out")) { print "** cases include $f but there's no .out file!\n"; }
	else { push (@tests, $f); $map{$f} = "inlist"; }
    }
}

opendir (DIR, ".") || die $!;

while ($f = readdir(DIR))
{
    if ($f =~ /\.cool$/)
    {
	if ($map{$f} eq "") { print "** file $f appears to be extraneous.\n"; }
    }
    if ($f =~ /\.cool\.out\d*/)
    {
	$ff = $f;
	$ff =~ s/\.out\d*//;
	if ($map{$ff} eq "") { print "** file $f appears to be extraneous.\n"; }
    }
}

foreach $test (sort (keys %map))
{
    $cmd = "./reflexer $test | diff - $test.out";
    $diff = `$cmd`;
    if ($diff eq "") {  }
    else { print "** $test.out doesn't match reference lexer output\n"; }
}

print "--- Checking done ---------------------------\n";

