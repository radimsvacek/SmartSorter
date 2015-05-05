#!/usr/bin/perl 

use strict;
use warnings;

{
	local *STDOUT;
	open (STDOUT, '>>', '.log');
    local @ARGV = ($ARGV[0]);
	do 'fdupe.pl';
}



my $filename = '.log';
open(my $fh, '<:encoding(UTF-8)', $filename)
	or die "Could not open file '$filename' $!";

my $row_count = 0; 
my $new_file = 0;

local *STDOUT;
open (STDOUT, '>>', 'files_to_remove');

while (my $row = <$fh>) {
	$row_count+=1;
	if($row_count<=2){
		next;}
	
	my $first;
	if(length($row) > 1){
		$first = substr($row, 0, 1);
	}
	else{
		$new_file=0;
		next;
	}

	if($new_file eq 1){
		print "$row";	
	}


	if($first eq ' '){
		$new_file=0;
	}
	else{
		$new_file=1;
		next;
	}
	
}


unlink '.log';
