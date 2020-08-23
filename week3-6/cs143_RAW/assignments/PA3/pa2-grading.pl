#!/usr/bin/perl -w

use strict;

use FileHandle;
use Getopt::Long;

my @check_files = ( "cool.y", "cool.cup" );
my $grading_dir = "./grading";
my $grading_cmd = "./143publicgrading PA3";
my $just_unpack;
my $just_run;
my $verbose;
my $skip_check;

sub usage {
    print "Usage: $0 [options]\n";
    print "    Options: -dir <path>  - specifies where to unpack/run\n";
    print "                            grading scripts [default = \"$grading_dir\"]\n";
    print "             -cmd <path>  - specifies what command to run for grading\n";
    print "                            [default = \"$grading_cmd\"]\n";
    print "             -x           - unpack script and test cases, but do not run\n";
    print "             -r           - don't unpack, just run existing script\n";
    print "             -v           - enable verbose output\n";
    return "\n";
}

die usage()
    unless(GetOptions("dir=s" => \$grading_dir,
		      "cmd=s" => \$grading_cmd,
		      "x" => \$just_unpack,
		      "r" => \$just_run,
		      "v" => \$verbose,
		      "skip" => \$skip_check,
		      "check=s" => \@check_files,
		      "help" => sub { usage(); exit 0; }));

unless($skip_check) {
    print "Checking that you appear to be in the assignment directory...\n"
	if($verbose);
    my(@found) = grep({ -r $_ } @check_files);
    unless(@found) {
	die "$0: could not find any of the following files - are you running\n  this from the assignment directory?\n    " . join("\n    ", @check_files) . "\n";
    }
}

print "Creating grading directory '$grading_dir' if necessary...\n"
    if($verbose);
unless((-d $grading_dir) or
       mkdir($grading_dir)) {
    die "$0: '$grading_dir' doesn't appear to be a directory and can't be created: $!";
}

print "Changing to grading directory '$grading_dir'...\n"
    if($verbose);
unless(chdir($grading_dir)) {
    die "$0: can't change directory to '$grading_dir'...\n";
}

unless($just_run) {
    print "Unpacking grading script and test cases...\n"
	if($verbose);

    my $tar_options = $verbose ? "-zxvf" : "-zxf";
    my $fh = new FileHandle("| tar $tar_options -") ||
	die "$0: couldn't run uudecode or tar: $!\n";

    # skip the first line
    my $dummy = <DATA>;
    die unless($dummy =~ /^begin /);
    binmode($fh);
    while(defined(my $line = <DATA>)) {
	$line =~ s/\s+$//;
	last if($line =~ /^end$/);
	my $unpacked = unpack('u', $line);	
	next unless(defined($unpacked));
	print $fh $unpacked;
    }
    $fh->close;
}

unless($just_unpack) {
    print "Running command: $grading_cmd\n"
	if($verbose);

    system($grading_cmd);
}


__DATA__
begin 644 /dev/stdout
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
9LSX'2D&HSX$T:-"@X9MO_C\'!P&4`'`#````
`
end
