############################################################
#  engtok -- english tokenizer
#  
#


package engtok; 

use strict; 


sub new {
    my $pkg = shift;     
    my $tok = []; 
    
    ## abbreviations
    $tok->[0] = {};
    my $abbrev = "apr aug av bldg dec dr calif corp feb fla inc jan jul jun lt ltd mar mr mrs ms nov oct rev sep st vol vols vs";
    my @abbrev = split(/\s+/,$abbrev);
    my $a; 
    foreach $a (@abbrev) { $tok->[0]{$a} = 1; }
    
    return bless $tok, $pkg; 
}


sub tokenize_string {
    my ($tokenizer, $line, $light) = @_; 
    
    $line =~ s/^\s*//;
    $line =~ s/\s*$//;
    my @T = split(/\s+/,$line);

    my $i = 0;
    while ($i <= $#T) {
      # remove punctuation from start of word
      if ($T[$i] =~ /^(["'\(\)\[\]\$:;,\/\%])(.+)$/ and 
          $T[$i] !~ /^'[dsm]$/i and $T[$i] !~ /^'re$/i and
          $T[$i] !~ /^'ve$/i and $T[$i] !~ /^'ll$/i) {
         splice(@T,$i,1,$1,$2);
         $i++;
         # remove sentence breaking punctuation with quote from end of word
      } elsif ($T[$i] =~ /^(.+)([?!\.])(["'])$/) {
#         splice(@T,$i,1,$1,"$2$3","\n");
         splice(@T,$i,1,$1,"$2$3");
      # remove non-sentence-breaking punctuation from end of word
      } elsif ($T[$i] =~ /^(.+)([:;,"'\)\(\[\]\%])$/) {
         splice(@T,$i,1,$1,$2);
      # remove sentence-breaking punctuation (not period) from end of word
      } elsif ($T[$i] =~ /^(.+)([?!])$/ or
               $T[$i] =~ /^(.+[^\.])(\.\.+)$/) {
#         splice(@T,$i,1,$1,$2,"\n");
	  splice(@T,$i,1,$1,$2);
      # separate currency symbol from value
      } elsif ($T[$i] =~ /^([A-Za-z]+\$)(.+)$/i) {
         splice(@T,$i,1,$1,$2);
         $i++;
      # separate currency symbol other symbols
      } elsif ($T[$i] =~ /^(.*)-\$(.*)$/i) {
         splice(@T,$i,1,$1,"-","\$",$2);
         $i++;
      # split words like we're did't etcetera
      } elsif ($light==0 && $T[$i] =~ /^(.+)('re|'ve|'ll|n't|'[dsm])$/i) {
         splice(@T,$i,1,$1,$2);
      # split words with punctuation in the middle
      } elsif ($T[$i] =~ /^(.*[a-z].*)([",\(\)])(.*[a-z].*)$/i) {
         splice(@T,$i,1,$1,$2,$3);
      # separate words linked with sequence (>=2) of periods
      } elsif ($T[$i] =~ /^(.*[^\.])(\.\.+)([^\.].*)$/) {
         splice(@T,$i,1,"$1$2",$3);
      # remove initial hyphens from word
      } elsif ($T[$i] =~ /^(-+)([^\-].*)$/ and $T[$i] ne "-DOCSTART-") {
         splice(@T,$i,1,$1,$2);
      # separate sport types and first words in article titles
#      } elsif ($T[$i] =~ /^([A-Za-z]+)-(.*)$/ and defined $sport{$1}) {
#         splice(@T,$i,1,$1,"-",$2);
      # separate number and word linked with hyphen
      } elsif ($T[$i] =~ /^([0-9\/]+)-([A-Z][a-z].*)$/) {
         splice(@T,$i,1,$1,"-",$2);
      # separate number and word linked with period
      } elsif ($T[$i] =~ /^([0-9\/]+)\.([A-Z][a-z].*)$/) {
         splice(@T,$i,1,"$1.",$2);
      # separate number and word linked with period
      } elsif ($T[$i] =~ /^(.*)\.-([A-Z][a-z].*)$/) {
         splice(@T,$i,1,"$1.","-",$2);
      # separate initial from name
      } elsif ($T[$i] =~ /^([A-Z]\.)([A-Z][a-z].*)$/) {
         splice(@T,$i,1,$1,$2);
      # introduce sentence break after number followed by period
      } elsif ($i != 0 and $T[$i] =~ /^(.*[0-9])(\.)$/) {
#         splice(@T,$i,1,$1,$2,"\n");
# I do not introduce the "\n"
         splice(@T,$i,1,$1,$2);
      # split words containing a slash if they are not a URI
      } elsif ($T[$i] !~ /^(ht|f)tps*/i and 
               $T[$i] =~ /[^0-9\/\-]/ and
               $T[$i] =~ /^(.+)\/(.+)$/) {
         splice(@T,$i,1,$1,"/",$2);
      # put sentence break after period if it is not an abbreviation
      } elsif ($T[$i] =~ /^(.+)(\.)$/ and $T[$i] !~ /^\.+$/ and
               $T[$i] !~ /^[0-9]+\./) {
         my $word = $1;
         if ($i != $#T and $tokenizer->abbrev($word)) { $i++; }
         else { 
#            splice(@T,$i,1,$1,$2,"\n");
# I do not introduce the "\n"
            splice(@T,$i,1,$1,$2);
         }
      } else { $i++; }
   

#   if ($T[$#T] ne "\n") { push(@words,"\n"); }
#   $line = join(" ",@words);
#   $line =~ s/ ([?!\.]) \n (["']) / $1 $2 \n /g;
#   $line =~ s/ *\n */\n/g;
#   # print every word on a separate line
#   $line =~ s/ +/ /g;
#   $line =~ s/\n/\n\n/g;
#   $line =~ tr/ /\n/;
#   print $line;
  }
  
  return @T; 

}
    
    
sub abbrev {
    my ($t, $word) = @_;
    
    $word =~ tr/[A-Z]/[a-z]/;
    if ($word =~ /\./ and $word !~ /[0-9]/) { return(1); };
    if ($word =~ /^[a-z]$/) { return(1); };
    return(defined $t->[0]{$word});
}



1;



