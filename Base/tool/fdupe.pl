#!/usr/bin/perl
#
# fdupe tool - finding duplicate files
#
# $Id: fdupe,v 1.7 2011/10/14 20:11:21 root Exp root $
#
# Source code Copyright (c) 1998,2011 Bernhard Schneider.
# May be used only for non-commercial purposes with
# appropriate acknowledgement of copyright.
#
# FILE :        fdupe
# DESCRIPTION : script finds duplicate Files.
# AUTHOR:       Bernhard Schneider <bernhard@neaptide.org>
# hints, crrections & ideas are welcome
#
# usage: fdupe.pl <path> <path> ...
#        find / -xdev | fdupe.pl
#
# how to select and remove duplicates:
#   redirect output to >file, edit the file and mark lines you 
#   wish to move/delete with a preceding dash (-)
#   Use following script to delete marked files:
#   #!/usr/bin/perl -n
#   chomp; unlink if s/^-//;
#
# history:
# 12.05.99 - goto statment replaced with next
# 14.05.99 - minor changes
# 18.05.99 - removed confusing 'for $y'
#            included hash-search 
# 20.05.99 - minor changes
# 02.03.00 - some functions rewritten, optimized for speed
# 10.01.01 - hint-fix by Ozzie |ozric at kyuzz.org|
# 05.03.02 - fixed hangups by reading block/char-Devices
# 08.09.11 - skips checking of hard links
# 14.10.11 - accept file names from stdin
#
#use strict; # uncomment for debugging

$|=1; 
local (*F1,*F2); my %farray = (); my $statF1;

# ------------------------------
# traverse directories
sub scan ($) {
    my ($dir) = $_[0];
    opendir (DIR, $dir) or die "($dir) $!:$@";
    map {
          (-d) ? scan ($_) : push @{$farray{-s $_}},$_
             unless (-l or -S  or -p or -c or -b);
    } map "$dir/$_", grep !/^\.\.?$/, readdir (DIR); closedir (DIR);
}

# ------------------------------
# get chunk of bytes from a file
sub getchunk ($$) {
  my ($fsize,$pfname) = @_;
  my $chunksize = 32;
  my ($nread,$buff);
  
  return undef unless open(F1,$$pfname);

  $statF1 = [(stat  F1)[3,1]];
  binmode F1;
  $nread = read (F1,$buff,$chunksize);
  ($nread == $chunksize || $nread == $fsize) ? "$buff" : undef;
}  

# ------------------------------
# compare two files
sub mycmp ($) {
  my ($fptr) = $_[0];
  my ($buffa, $buffb);
  my ($nread1,$nread2);
  my $statF2;
  my ($buffsize) = 16*1024;
  
  return -1 unless (open(F2,"<$$fptr"));
  
  $statF2 = [(stat  F2)[3,1]];

  return 0 
  	if ($statF2->[0] > 1 && $statF1->[1] == $statF2->[1]);

  binmode F2;
  seek (F1,0,0);
  
  do {  $nread1 = read (F1,$buffa,$buffsize);
    	$nread2 = read (F2,$buffb,$buffsize);

    	if (($nread1 != $nread2) || ($buffa cmp $buffb)) {
      	  return -1;
        }
  } while ($nread1);
  
  return 0;
}

# ------------------------------

print "collecting files and sizes ...\n";

if (-t STDIN) {
	$ARGV[0] = '.' unless $ARGV[0]; # use wd if no arguments given
	map scan $_, @ARGV;
} else {	
	while (<STDIN>)  {
		s°[\r\n]$°°g;
		push @{$farray{-s $_}},$_
		 unless (-l or -S  or -p or -c or -b);
	}
}

print "now comparing ...\n";
for my $fsize (reverse sort {$a <=> $b} keys %farray) {

  my ($i,$fptr,$fref,$pnum,%dupes,%index,$chunk);

  # skip files with unique file size
  next if $#{$farray{$fsize}} == 0; 
  
  $pnum  = 0;
  %dupes = %index = ();
  
  nx:
  for (my $nx=0;$nx<=$#{$farray{$fsize}};$nx++) # $nx now 1..count of files 
  {                                             # with the same size
	$fptr = \$farray{$fsize}[$nx];          # ref to the first file
    $chunk = getchunk $fsize,$fptr;
    if ($pnum) {
	  for $i (@{$index{$chunk}}) {
         $fref = ${$dupes{$i}}[0];
	     unless (mycmp $fref) {
            # found duplicate, collecting
	        push @{$dupes{$i}},$fptr;
			next nx;
	     }
	  }
    }

    # nothing found, collecting 
    push @{$dupes{$pnum}},$fptr;
    push @{$index{$chunk}}, $pnum++;
  }
  # show found dupes for actual size
  for $i (keys %dupes) {
    $#{$dupes{$i}} || next;
    print "\n size: $fsize\n\n";
    for (@{$dupes{$i}}) {
        print $$_,"\n"; 
    }
  }
}

close F1;
close F2;


