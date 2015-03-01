#! /usr/local/bin/perl


## llegeix text de l'stdin i el tokenitza.
## el llenguatge es pot canviar modificant 
## el parametre LANGUAGE al fer el new.
my $path;
if ($ENV{'SOFTMT_SOURCE_DIR'} eq "") {
	$path = "/veu4/usuaris02/soft/mt-tokenise/tokenitzador";
	use lib "/veu4/usuaris02/soft/mt-tokenise/tokenitzador";
}
else {
	$path = $ENV{'SOFTMT_SOURCE_DIR'} . "/mt-tokenise/tokenitzador";
	use lib $ENV{'SOFTMT_SOURCE_DIR'} . "/mt-tokenise/tokenitzador";
}
use Tokenizer;

use strict;

  
  my $lg=shift(@ARGV); # sp,cat,eng
  my $tk;
  my @tokens; # token List

  $tk = Tokenizer->new (ABREVIATIONS=>$path."/abrev_".$lg.".dat",
			LIST=>\@tokens, LANGUAGE=>$lg);
 
  ## convertir la frase en una sequencia de <Paraula>, 
  while (<>) {
    @tokens=();
    $tk->tokenizeString($_);
    print "@tokens\n";
  }



