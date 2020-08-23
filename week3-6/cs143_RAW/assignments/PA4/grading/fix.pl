#!/usr/local/bin/perl

opendir (DIR, ".") || die $!;

while ($out = readdir(DIR))
{
    $src = $out;
    if ($out =~ /\.out$/)
    {
	$src =~ s/\.out//;
	$src =~ s/\.test//;
	$src =~ s/\.cool//;

	open (IN, "< $out") || die $!;
	open (OUT, "> $out.temp") || die $!;
	
	while (<IN>)
	{
	    if (/.*$src.*?:/)
	    {
		$adj = $_;
		$adj =~ s/.*$src.*?:/$src\.test:/;
		print OUT $adj;
	    }
	    else
	    {
		print OUT $_;
	    }
	}
	close IN;
	close OUT;
	system ("cp -f $out.temp $out");
    }

    
    
}
