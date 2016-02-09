#!/usr/bin/perl -w
# $Id: pbitreecalc.perl,v 1.10 2013-10-17 18:26:14-07 - - $
#
# DESCRIPTION
#    See programming project specifications.
#

use strict;
use warnings;
use Getopt::Std;

my $exit_status = 0;
END {exit $exit_status}
sub note(@) { print STDERR "$0: @_"; };
$SIG{'__WARN__'} = sub { note @_; $exit_status = 1; };
$SIG{'__DIE__'} = sub { warn @_; exit; };

my $NUMPAT = '[+-]?(\d+\.?\d*|\.\d+)([Ee][+-]?\d+)?';
my $VARPAT = '[a-z]';
my $INFINITY = 1000e1000;
my $NAN = - $INFINITY / $INFINITY;
my %values;
my %trees;

my %options;
getopts ("eo:", \%options);
my $outfilename = $options{"o"};
open my $outfile, ">$outfilename" or die "$outfilename: $!\n"
     if defined $outfilename;
$outfile = *STDOUT unless $outfile;;

sub stringify ($);
sub stringify ($) {
   my ($tree) = @_;
   if ($tree->{LEAF}) {
      return $tree->{LEAF};
   }elsif ($tree->{OPER}) {
      return "(" . (stringify $tree->{LEFT}) . $tree->{OPER}
                 . (stringify $tree->{RIGHT}) . ")";
   }else {
      return "null";
   }
}

sub div ($$) {
   my ($left, $right) = @_;
   return $right != 0 ? eval {$left / $right}
        : $left > 0 ? $INFINITY
        : $left < 0 ? - $INFINITY
        : $NAN;
}


sub evaluate ($);
sub evaluate ($) {
   my ($tree) = @_;
   if ($tree->{LEAF}) {
      my $value = $values{$tree->{LEAF}};
      return defined $value ? $value : $NAN;
   }elsif ($tree->{OPER}) {
      my $left = evaluate $tree->{LEFT};
      my $right = evaluate $tree->{RIGHT};
      my $oper = $tree->{OPER};
      $left =~ s/inf/1e1000000/;
      $right =~ s/inf/1e1000000/;
      return "nan" if $left =~ m/nan/i or $right =~ m/nan/i;
      my $result = $oper eq "/"
           ? div $left, $right
           : eval "$left $oper $right";
      return $result;
   }else {
      return "null";
   }
}

sub display ($) {
   my ($var) = @_;
   my $value = $values{$var};
   $value = $NAN unless defined $value;
   print $outfile "$var: $value\n";
   my $tree = $trees{$var};
   print $outfile "   @{[stringify $tree]}\n";
}

sub syntax ($$) {
   my ($filename, $linenr) = @_;
   warn "$filename: $linenr: syntax error\n";
}


push @ARGV, "-" unless @ARGV;
for my $filename (@ARGV) {
   open my $file, "<$filename" or warn "$filename: $!\n" and next;
   LINE:
   while (my $line = <$file>) {
      chomp $line;
      print $outfile "**** $line\n" if $options{"e"};
      $line =~ s/\s+//g;
      $line =~ s/.*/lc $&/e;
      next if $line =~ m/^#/;
      if ($line =~ m/^($VARPAT):($NUMPAT)$/) {
         my ($var, $value) = ($1, $2);
         $values{$var} = $value + 0;
         $trees{$var} = undef;
         display $var;
      }elsif ($line =~ m/^($VARPAT)\?$/) {
         my $var = $1;
         my $newvalue = evaluate $trees{$var};
         $values{$var} = $newvalue unless $newvalue eq "null";
         display $var;
      }elsif ($line =~ m/^($VARPAT)=(.*)/) {
         my ($var, $rpn) = ($1, $2);
         my @stack;
         while ($rpn =~ s/^.//) {
            my $char = $&;
            if ($char =~ m/$VARPAT/) {
               push @stack, {LEAF=>$char};
            }elsif ($char =~ m/\+|\-|\*|\//) {
               my $right = pop @stack;
               my $left = pop @stack;
               syntax $filename, $. and next LINE
                      unless $left and $right;
               push @stack, {OPER=>$char, LEFT=>$left, RIGHT=>$right};
            }else {
               syntax $filename, $.;
               next LINE;
            }
         }
         syntax $filename, $. and next unless @stack == 1;
         $values{$var} = evaluate ($trees{$var} = pop @stack);
         display $var;
      }else {
         syntax $filename, $.;
      }
   }
   close $file;
}

print $outfile "**** FINAL SYMBOL TABLE ****\n";
for my $var (sort keys %values) {
   my $value = $values{$var};
   display $var if defined $value and $value !~ m/nan/i;
}

close $outfile;
