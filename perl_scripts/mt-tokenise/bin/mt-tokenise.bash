#! /bin/bash
if [ "$SOFTMT_SOURCE_DIR" != "" ]; then
	src_path="$SOFTMT_SOURCE_DIR/mt-tokenise"
else
	src_path="/home/blue1/ad465/softmt/source/mt-tokenise"
fi

#E_tok="tokenitzador/tokenizer.pl"
E_tok="engtok-1.0/tokenizer.pl"
CS_tok="tokenitzador/tokenizer.pl"

function usage
{
    echo "############################################"
    echo "####   mt-tokenise: Tokenization tool   ####"
    echo "############################################"
    echo "";
    echo "usage: mt-tokenise [cat|eng|leng|spa] < input_text > output_text"
    echo "";
    echo "note='leng' is a light eng tokenizer (don't separate 'll 're...)"; exit;
}

lng=""
tok=""
light=0

case $1 in
    spa )          	        
	lng=sp
	tok=$CS_tok
	;;
    cat ) 
	lng=cat
	tok=$CS_tok
	;;
    eng ) 
	lng=eng
	tok=$E_tok
	;;
    leng ) 
	lng=eng
	tok=$E_tok
	light=1
	;;
    * ) usage;
	exit 1
esac

if [ "$lng" == "eng" ]; then
    perl $src_path/$tok $light
else
    perl $src_path/$tok $lng
fi
