#!/usr/local/bin/perl

$where = $ARGV[0];

opendir (DIR, "$where") || die $!;

while ($src = readdir(DIR))
{
    $dst = $src;
    if ($src =~ /\.test\.cool$/)
    {
	$dst =~ s/\.cool//;
    }
    elsif ($src =~ /\.cool$/)
    {
	$dst =~ s/\.cool/\.test/;
    }
    else { $dst = ""; }
    
    if ($dst ne "")
    {
	if (! (-e "./$dst"))
	{
	    print "NEW CASE: $src -> $dst\n";
	    # print "$src.out -> $dst.out\n";
	    system ("cp ../past/unknown/PA4/$src ./$dst");
	    system ("cp ../past/unknown/PA4/$src.out ./$dst.out");
	    system ("echo \"$dst; 1;\" >> newcases");
	}
	else
	{
	    print "\tAlready have: $dst\n";
	}
    }
}
