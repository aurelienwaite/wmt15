#!/usr/bin/perl -CSD
use utf8;

while (<>){
    chomp;

    # replace ' s by 's 
    $_ =~ s/ ' s / 's /g;

    # replace l ' by l', etc
#    $_ =~ s/([^ ]+)-([^ ]+)/$1 $2/g;
    $_ =~ s/(^| )([cdjlmnst]) ' /$1$2' /gi;
    $_ =~ s/(^| )(qu|jusqu|lorsqu|quelqu|puisqu|quoiqu|presqu) ' / $2' /gi;
    $_ =~ s/(^| )aujourd ' hui/$1aujourd'hui/g;
    $_ =~ s/(^| )aujourd' hui/$1aujourd'hui/g;
    $_ =~ s/(^| )Aujourd ' hui/$1Aujourd'hui/g;
    $_ =~ s/(^| )Aujourd' hui/$1Aujourd'hui/g;

    print $_."\n";
}
