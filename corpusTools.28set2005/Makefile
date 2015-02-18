corpusName ?= EsEn_corpus
#  The input corpus format is in the following format:
#  Source Sentence # Target sentence
#  the normal # mark in the corpus must be converted to \#.


BASENAME ?= corpus_ 

toolDIR ?= .
ARCH ?= intel

TMPNAME := $(shell mktemp tmp.XXXXXX)

ZIP ?= -zip


all: $(BASENAME)ES.preprocessed $(BASENAME)EN.preprocessed

$(BASENAME)ES: $(BASENAME)EN

$(BASENAME)EN: $(corpusName)
	-rm $(BASENAME)ES $(BASENAME)EN
	$(toolDIR)/$(ARCH)/mycat $< | \
	sed -e "s/<specialParagraph>//g" | \
	sed -e "s/<\/specialParagraph>//g" | \
	sed -e "s/<special>//g" | \
	sed -e "s/<\/special>//g" | \
	grep -Ev "^ <" | \
	sed -e "s/<[^<>]\+>//g" | \
	$(toolDIR)/$(ARCH)/applyRegexMap $(toolDIR)/removeNoise.regexmap | grep -v "&" |\
	$(toolDIR)/$(ARCH)/filterBiText2 -soft -realwords 10000 |\
	gawk -vsrc=$(BASENAME)ES  -vtar=$(BASENAME)EN 'BEGIN{FS=" # "}{print $$1 >> src; print $$2 >> tar }'

# gawk 'BEGIN{FS=" # "}{if($$1 ~ /^ <speaker/ && $$1 ~/ .$$/) {a=$$1; b=$$2} el
# se if($$1 ~ /^ \">$$/ ) print a$$1,"#",b$$2; else print }' > $(TMPNAME).corpus


$(BASENAME)EN.tok: $(BASENAME)EN $(toolDIR)/en.tokmap
	$(toolDIR)/$(ARCH)/mycat $< | \
	$(toolDIR)/$(ARCH)/applyRegexMap $(toolDIR)/smoothUTF8.regexmap |\
	$(toolDIR)/$(ARCH)/applyTokenizer $(toolDIR)/en.tokmap $(ZIP) > $@

$(BASENAME)ES.tok: $(BASENAME)ES $(toolDIR)/es.tokmap
	$(toolDIR)/$(ARCH)/mycat $< | \
	$(toolDIR)/$(ARCH)/applyRegexMap $(toolDIR)/smoothUTF8.regexmap |\
	$(toolDIR)/$(ARCH)/applyTokenizer $(toolDIR)/es.tokmap $(ZIP) > $@

$(BASENAME)%.wop: $(BASENAME)%.tok
	$(toolDIR)/$(ARCH)/mycat $< | $(toolDIR)/deleteParens > $@

propernouns_list: $(BASENAME)ES.wop $(BASENAME)EN.wop
	$(toolDIR)/$(ARCH)/extr-propnames $^ -propers | \
	grep -E '^[[:upper:]][[:alpha:]]' | \
	grep -Ev "[[:punct:]]" > $@

%.preprocessed: %.tok propernouns_list
	$(toolDIR)/$(ARCH)/mycat $< | $(toolDIR)/$(ARCH)/properHighlighter propernouns_list | gzip > $@ 


#removing_Hyphens
#    sed -e "s/-\([[:alpha:]]\)/ $1/g" | sed -e "s/\([[:alpha:]]\)-/$1 /g"

clean:
	-rm  $(TMPNAME)*
