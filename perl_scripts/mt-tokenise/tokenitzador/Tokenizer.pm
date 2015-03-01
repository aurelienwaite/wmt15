
package Tokenizer;
use strict;

# BREU MODIFICACIO PER ADRIA DE GISPERT (considera @ una lletra, i no se separa mai)
# Verticalitza un text i separa signes de puntuacio, nombres, etc.
# Modificacions: 
# No separa les abreviacions especificades en l'arxiu abrev.dat; 
#    · aquestes han de seguir el següent patró:  (AlphaNum+\.)+
#    · Ex: ee.uu. No es separarà.
#    · Nota: Si apareix "ee.uu..." tenen prioritat els "..."; per tant es generen "ee", ".", "uu", "..."

sub new {
    my ($pkg, %options) = @_;
    my $t = {};

    # Pas de les opcions del tokenitzador

    if ($options{HANDLER}) {
	$t->{TH} = $options{HANDLER};
	delete $options{HANDLER};
    }
    if ($options{LIST}) {
	$t->{TL} = $options{LIST};
	delete $options{LIST};
    }
    $t->{LG} = "sp";
    if ($options{LANGUAGE}) {
	$t->{LG} = $options{LANGUAGE};
	delete $options{LANGUAGE};
    }

    %{$t->{ABR}} = ();
    if ($options{ABREVIATIONS}) {
	# inicialitzacio del tractament d'abreviacions
	open ABR, $options{ABREVIATIONS} or die "Tokenizer->new: No es pot obrir $options{ABREVIATIONS}; $!\n";
	# omplir taula de hash amb les abreviacions valides
	my $a;
	while (($a)=split(" ",<ABR>)) {
	    $a =~ tr/A-ZÑÇÁÉÍÓÚÏÜÀÈÌÒÙ/a-zñçáéíóúïüàèìòù/;
	    $t->{ABR}{$a} = 1;
	}
	delete($options{ABREVIATIONS});
    }
    if (keys %options) {
	die "Tokenizer->new: Unknown options: ", join(" ", keys %options), "!\n";
    }
    
    # Inicialitzacio de les expressions regulars

    $t->{RE}{ALPHA}      = '[^\<\>\[\](\-.,;:\?!¿¡«»\'")\\^~|}{_/+% 0-9]';
    $t->{RE}{ALPHANUM}   = '[^\<\>\[\](\-.,;:\?!¿¡«»\'")\\^~|}{_/+%\s]';
    $t->{RE}{NOALPHA}    = '[\<\>\[\](\-.,;:\?!¿¡«»\'")\\^~|}{_/+% 0-9]';
    $t->{RE}{NOALPHANUM} = '[\<\>\[\](\-.,;:\?!¿¡«»\'")\\^~|}{_/+%\s]';
    $t->{RE}{PARTNUM}    = '[^\<\>\[\](;:\?!¿¡«»\'")\\^~|}{\s]';
    
    $t->{RE}{BLANKLINE}  = '^(\s)*$';
    $t->{RE}{ALTRES}     = '[\<\>\[\](-.,;:\?!¿¡«»\'")\\^~|}{_/+%]';

    $t->{ibyte} = 0;
    $t->{long} = 0;
    return bless $t, $pkg;
}


sub clear {
    my $t = shift;
    $t->{ibyte} = 0;
    $t->{actual} = "";
}

sub tokenizeString {
    my ($t, $s) = @_;
    my @l = split("\n", $s);
    foreach  $s (@l) {
	$_ = $s."\n";
#	print STDERR "Tokenizing: $_\n";

	$t->tokenize;
	if ($t->{long}) {
	    $t->{long}++;
	}
	else {
	    $t->{ibyte}++;
	}
    }
}

sub tokenizeStream {
    my ($t, $fh) = @_;
    while (<$fh>) {
	$t->tokenize;
	if ($t->{long}) {
	    $t->{long}++;
	}
	else {
	    $t->{ibyte}++;
	}
    }
}


sub tokenize {
    my ($t) = @_;
    my ($l1);

    if (/$t->{RE}{BLANKLINE}/) {
	$t->{ibyte} += length; 
	return;
    }
	
    while (/^.+$/) {
	$l1=length;
	
	s%^((\s)+)$%$2% &&              # Elimina blancs del final de la linia
	    do {
		$t->{ibyte} += length $1 -1;
		next;
	    };
	s%^(\s+)(\S)%$2% && 		# Elimina blancs al principi de linia
	    do {
		$t->{ibyte} += length $1;
		next;
	    };	
####        s%^(([A-Za-z]\.)+)%% && 			# Mante inicials amb el punt enganxat
####		do {print "$1\n\n"; next ITEM;};
	s%^($t->{RE}{PARTNUM}*(\d)$t->{RE}{ALPHANUM}*)%% &&  # Llegeix nombres i codis
	    do {
		$t->new_token($1);
		next;
	    };
	s%^(\.\.\.)%% && 			# Mante els punts suspensius
	    do {
		$t->new_token($1);
		next;
	    };
	s%^(\<\<)($t->{RE}{ALPHANUM}+)%$2% &&          # separa cometes << 
	    do {
		$t->new_token($1);
		next;
	    };
	s%^($t->{RE}{ALPHA}+)(\@)($t->{RE}{ALPHA}+)((\.$t->{RE}{ALPHA}+)*)%% && # mante adreces e-mail 
	    do {
		$t->new_token($1.$2.$3.$4);
		next;
	    }; 
# linia modificada per fer QGO catala. Cal deixar-la com estava...
#	s%^($t->{RE}{ALPHA}+)(\'\-)$%% && # Mante compostos de final de linia
	s%^($t->{RE}{ALPHA}+)(\-)$%% && # Mante compostos de final de linia
	    do {
		$t->{actual} .= $1;
		$t->{long} += length($1) + 1;
		next;
	    };

        if ($t->{LG} eq "cat") {
           s%^([dlmtsnDLMTSN])(\')($t->{RE}{ALPHA})%$3% &&    # separa apostrofs davant mot
	       do {
		   $t->new_token($1.$2);
		   next;
	       };
        }
        elsif ($t->{LG} eq "eng") {
	    ### separa 's 'd 'm 'll 've 're darrera mot
	    s%^($t->{RE}{ALPHA}+)(\')([sdm]|ll|ve|re)($t->{RE}{NOALPHANUM})%$4% &&
		do {
		    $t->new_token($1);
		    $t->new_token($2.$3);
		    next;
		};
	};
	
	s%^($t->{RE}{ALPHA}+)(\'|\-|\_)($t->{RE}{ALPHA}+)%$3% && # Mante compostos amb - _ o '
	    do {
		$t->{actual} .= $1.$2;
		$t->{long} += length($1) + length($2);
		next;
	    };
	s%^(($t->{RE}{ALPHANUM}+\.)+)(?!\.\.)%% && 	# X: tractament d'abreviacions
	    do {
		my $w = $1;
		$a = $w;
		$a =~ tr/A-ZÑÇÁÉÍÓÚÏÜÀÈÌÒÙ/a-zñçáéíóúïüàèìòù/;
		if ($t->{ABR}{$a}) {
		    $t->new_token($w);
		}
		else {
		    foreach $a (split(/\./, $w)) {
			$t->new_token($a);
			$t->new_token(".");
		    }
		}
		next;
	    };
	s%^($t->{RE}{ALPHANUM}+)($t->{RE}{NOALPHANUM})%$2% && 	# Escriu paraula o numero
	    do {
		$t->new_token($1);
		next;
	    };
	s%^(\>\>)%% && 			        # Escriu cometes >>
	    do {
		$t->new_token($1);
		next;
	    };
	s%^($t->{RE}{ALTRES})%% && 			# Escriu caracter no alfanumeric
	    do {
		$t->new_token($1);
		next;
	    };
    }
} 


sub new_token {
    my ($t, $w) = @_;
    $w = $t->{actual}.$w;
    $t->{actual} = "";
    $t->{long} += length $1;
    if (exists($t->{TH})) {
	my $th = $t->{TH};
	&$th($w, $t->{ibyte}, $t->{long});
    }
    if (exists($t->{TL})) {
	push @{$t->{TL}}, $w;
    }
    $t->{ibyte} += $t->{long};
    $t->{long} = 0;
}




1;
