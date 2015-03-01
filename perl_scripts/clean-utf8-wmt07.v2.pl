#!/usr/bin/perl -CSD
use utf8;

##
# clean data by replacing certain characters with other more common characters or by deleting certain rare characters.
##

while(<>){
    chomp();
    s/\xc2\x20/\x20/g;
    s/\xc3\x20/\xc3\xa0/g;
    s/\xc5\x93/\xc3\xba/g;
    s/\xc5\xa1/\x69/g;
    s/\xe2\x80\xa2\x20//g;
    s/\xc2\xa0/\x20/g;
    #s/â/\x27/g;
    s/\xe2\x80\x99/\x27/g; #same as line before with \x notation
    #s/Î/\x41/g;
    s/\xce\x91/\x41/g;
    #s/â¦/.../g;
    s/\xe2\x80\xa6/.../g;
    #s/ â /\xE2\x80\x92/g;
    s/ \xe2\x80\x93 /\xe2\x80\x92/g;

    # frase freak en grec... (compte que a sota hi ha la regla Beta que ha d'anar sota!)
#    s/\x3c\x73\x70\x65\x63\x69\x61\x6c\x3e\x20\xcf\x83\xcf\x80\xce\xb5\xcf\x8d\xce\xb4\xce\xb5\x20\xce\xb2\xcf\x81\xce\xb1\xce\xb4\xce\xad\xcf\x89\xcf\x82\x20\x3c\x2f\x73\x70\x65\x63\x69\x61\x6c\x3e\x20//g;
#    s/\xce\xb2/beta/g;

#    s/\xce\xb1/\xc3\xa1/g;
#    s/\xce\xbd/\xc3\xad/g;
#    s/\xce\xb9/\xc3\xa9/g;

#    s/\xcf\x83/\xc3\xb3/g;
#    s/\xcf\x81/\xc3\xb1/g;
    
#    s/\xce\xad/\xc3\xa9/g;  # una cosa es francÃ¨s (no sÃ© que Ã©s)
#    s/\xe4\xa1\xa2\xc2\x80\xc2\x93/ /g; # Mesi......a
#    s/\xe4\x9f\xa7/ G/g; # Mesi......a (ENG)
    

#   s/\x20\x3c\x73\x70\x65\x63\x69\x61\x6c\x3e\x20\x22\x20\x3c\x2f\x73\x70\x65\x63\x69\x61\x6c\x3e\x20\x3c\x73\x70\x65\x63\x69\x61\x6c\x3e\x20\x4f\xce\xaf\xce\xbc\xce\xbf\xc3\xa9\x20\x3c\x2f\x73\x70\x65\x63\x69\x61\x6c\x3e\x20\x3c\x73\x70\x65\x63\x69\x61\x6c\x3e\x20\xce\xb7\x20\x3c\x2f\x73\x70\x65\x63\x69\x61\x6c\x3e\x20\x3c\x73\x70\x65\x63\x69\x61\x6c\x3e\x20\xcf\x84\xce\xac\xce\xbb\xc3\xa1\xc3\xa9\xc3\xad\xc3\xa1\x20\x3c\x2f\x73\x70\x65\x63\x69\x61\x6c\x3e\x20\x3c\x73\x70\x65\x63\x69\x61\x6c\x3e\x20\x22\x20\x3c\x2f\x73\x70\x65\x63\x69\x61\x6c\x3e//g; # vaya carro (SPA)
#    s/\x20\xe2\x80\x93\x20\x3c\x73\x70\x65\x63\x69\x61\x6c\x3e\x20\x22\xce\x9f\xce\xaf\xce\xbc\xce\xbf\xce\xb9\x20\xce\xb7\x20\xcf\x84\xce\xac\xce\xbb\xce\xb1\xce\xb9\xce\xbd\xce\xb1\x22\x20\x3c\x2f\x73\x70\x65\x63\x69\x61\x6c\x3e//g; # vaya carro (ENG)

#    s/\xec\x9f/i\xc3\xb3/g;

    s/\s+/ /g;  # multiples espais
    print "$_\n";

}

