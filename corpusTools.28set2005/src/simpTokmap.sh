less en.tokmap | grep EXCEPTION | grep "\." | tr ' ' '\n' | ~/tt2/simplifyText.out | gawk '{if((n=split($1,a,"."))<3) print $1}' | sed -e 's/\.$//g' | sort -u >> abbreviations.en
cat abbreviations.en | ~/tt2/dedup.pl | grep -v EXCEPTION | paste -d " " -s >> en.tokmap
