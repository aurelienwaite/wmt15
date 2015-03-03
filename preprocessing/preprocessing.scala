#!/bin/bash
L=`readlink -f $0`
L=`dirname $L`/target/universal/stage/lib
cp=`echo $L/*.jar|sed 's/ /:/g'`:target/scala-2.11/preprocessing_2.11-0.1-SNAPSHOT.jar  #*/
export LD_LIBRARY_PATH=/home/blue9/aaw35/wmt13/lib
exec /home/blue7/aaw35/libs/scala-2.11.5/bin/scala -J-Xmx8G -J-Dfile.encoding=UTF-8 -classpath $cp $0 $@
!#

import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.SparkConf
import org.apache.spark.storage._
import scala.io.Source
import scala.collection.immutable._
import uk.ac.cam.eng.preprocessing._
import java.text.Normalizer
import org.apache.commons.lang._

val conf = new SparkConf().setMaster("local[16]").setAppName("WMT-15 Preprocessing")
val sc = new SparkContext(conf)

def preprocessEnFr(enFileName : String, frFileName : String) = {
  val en = Source.fromFile(enFileName, "UTF-8")
  val fr = Source.fromFile(frFileName, "UTF-8")
  val joined = sc.parallelize((en.getLines zip fr.getLines).toStream,500)
  val cleanedEng = joined.map(_._1).pipe("../perl_scripts/clean-utf8.pl").map(StringEscapeUtils.unescapeXml(_))
    .pipe("../perl_scripts/normalize-punctuation.perl en")
  val cleanedFr = joined.map(_._2).pipe("../perl_scripts/clean-utf8-wmt07.v2.pl").map(Normalizer.normalize(_, Normalizer.Form.NFC))
    .map(StringEscapeUtils.unescapeXml(_)).pipe("../perl_scripts/normalize-punctuation.perl fr")
  val applyAachenTok = cleanedEng.zip(cleanedFr).mapPartitions{iter => 
  	{
  		val enTokeniser = new PipedFunc("../corpusTools.28set2005/intel/applyTokenizer ../corpusTools.28set2005/en.tokmap")
  		iter.flatMap{ elem => 
  			val (enLine, frLine) = elem
  			val enTok = enTokeniser(enLine)
        if (enTok.isEmpty) None else
          Some( (enTok.get, frLine) )
  		} 
  	}
  }
  val enTok = applyAachenTok.map(_._1)
    .pipe("../scripts/postAachanTokeniser.sh").pipe("../perl_scripts/engtok-1.0/tokenizer.pl").map(squeeze(_))
  val frTok = applyAachenTok.map(_._2).pipe("../perl_scripts/tokenizer-fr.perl -l fr").pipe("../perl_scripts/retokenise_fr_wmt07.pl")
    .map(squeeze(_))
  enTok.zip(frTok).filter(!truncateByLength(_, 0)).filter(truncateIfMatch(".")(_))
    .filter(truncateIfMatch("#")(_)).filter(truncateByLength(_)).filter(truncateByFertility(_))
}

/**
* Common crawl preprocessing is different due to language detection
*/
def preprocessEnFrCC() = {
  val joined = preprocessEnFr("/data/mifs_scratch/mh693/wmt15/data/parallel/fr_en/commoncrawl.fr-en.en", "/data/mifs_scratch/mh693/wmt15/data/parallel/fr_en/commoncrawl.fr-en.fr")
  val langDetect = joined.mapPartitions{iter =>
    val detector = new LangDetect("langdetect-09-13-2011/profiles")
    iter.filter(tuple => detector("en", tuple._1) && detector("fr", tuple._2))
  }
  langDetect persist StorageLevel.DISK_ONLY
  langDetect.map(_._1).saveAsTextFile("/data/mifs_scratch/aaw35/wmt15/data/preprocessed/fr_en/commoncrawl.fr-en.en.tok")
  langDetect.map(_._2).saveAsTextFile("/data/mifs_scratch/aaw35/wmt15/data/preprocessed/fr_en/commoncrawl.fr-en.fr.tok")
}

def preprocessEnFrAndSave(enIn : String, frIn : String, enOut : String, frOut : String) = {
  val joined = preprocessEnFr(enIn, frIn)
  joined persist StorageLevel.DISK_ONLY
  joined.map(_._1).saveAsTextFile(enOut)
  joined.map(_._2).saveAsTextFile(frOut)
}

//preprocessEnFrCC

val euroParlEn = "/data/mifs_scratch/mh693/wmt15/data/parallel/fr_en/europarl-v7.fr-en.en"
val euroParlFr = "/data/mifs_scratch/mh693/wmt15/data/parallel/fr_en/europarl-v7.fr-en.fr"
val euroParlEnOut = "/data/mifs_scratch/aaw35/wmt15/data/preprocessed/fr_en/europarl-v7.fr-en.en.tok"
val euroParlFrOut = "/data/mifs_scratch/aaw35/wmt15/data/preprocessed/fr_en/europarl-v7.fr-en.fr.tok"
//preprocessEnFrAndSave(euroParlEn, euroParlFr, euroParlEnOut, euroParlFrOut)

val gigaEn = "/data/mifs_scratch/aaw35/wmt15/data/parallel/fr_en/giga-fren.release2.en"
val gigaFr = "/data/mifs_scratch/aaw35/wmt15/data/parallel/fr_en/giga-fren.release2.fr"
val gigaEnOut = "/data/mifs_scratch/aaw35/wmt15/data/preprocessed/fr_en/giga-fren.release2.en.tok"
val gigaFrOut = "/data/mifs_scratch/aaw35/wmt15/data/preprocessed/fr_en/giga-fren.release2.fr.tok"
//preprocessEnFrAndSave(gigaEn, gigaFr, gigaEnOut, gigaFrOut)

val ncEn = "/data/mifs_scratch/mh693/wmt15/data/parallel/fr_en/news-commentary-v10.fr-en.en"
val ncFr = "/data/mifs_scratch/mh693/wmt15/data/parallel/fr_en/news-commentary-v10.fr-en.fr"
val ncEnOut = "/data/mifs_scratch/aaw35/wmt15/data/preprocessed/fr_en/news-commentary-v10.fr-en.en.tok"
val ncFrOut = "/data/mifs_scratch/aaw35/wmt15/data/preprocessed/fr_en/news-commentary-v10.fr-en.fr.tok"
//preprocessEnFrAndSave(ncEn, ncFr, ncEnOut, ncFrOut)

val unEn = "/data/mifs_scratch/mh693/wmt15/data/parallel/fr_en/undoc.2000.fr-en.en"
val unFr = "/data/mifs_scratch/mh693/wmt15/data/parallel/fr_en/undoc.2000.fr-en.fr"
val unEnOut = "/data/mifs_scratch/aaw35/wmt15/data/preprocessed/fr_en/undoc.2000.fr-en.en.tok"
val unFrOut = "/data/mifs_scratch/aaw35/wmt15/data/preprocessed/fr_en/undoc.2000.fr-en.fr.tok"
//preprocessEnFrAndSave(unEn, unFr, unEnOut, unFrOut)

def preprocessEnMonolingual(enIn : String, enOut : String)={
  val cleanedEng = sc.textFile(enIn, 500).pipe("../perl_scripts/clean-utf8.pl").map(StringEscapeUtils.unescapeXml(_))
    .pipe("../perl_scripts/normalize-punctuation.perl en")
  val applyAachenTok = cleanedEng.mapPartitions{iter => {
      val enTokeniser = new PipedFunc("../corpusTools.28set2005/intel/applyTokenizer ../corpusTools.28set2005/en.tokmap")
      iter.flatMap{ enTokeniser(_)}
    }
  }.pipe("../scripts/postAachanTokeniser.sh").pipe("../perl_scripts/engtok-1.0/tokenizer.pl").map(squeeze(_))
  applyAachenTok.saveAsTextFile(enOut)
}

val ncMonoEn = "/data/mifs_scratch/aaw35/wmt15/data/monolingual/en/news-commentary-v10.en"
val ncMonoEnOut = "/data/mifs_scratch/aaw35/wmt15/data/preprocessed/en/news-commentary-v10.en.tok"
preprocessEnMonolingual(ncMonoEn, ncMonoEnOut)

val ndMonoEn = "/data/mifs_scratch/mh693/wmt15/data/monolingual/en/news-discuss-v1.en.txt"
val ndMonoEnOut = "/data/mifs_scratch/aaw35/wmt15/data/preprocessed/en/news-discuss-v1.en.tok"
preprocessEnMonolingual(ndMonoEn, ndMonoEnOut)

val n2013 = "/data/mifs_scratch/mh693/wmt15/data/monolingual/en/news.2013.en.shuffled"
val n2013Out = "/data/mifs_scratch/aaw35/wmt15/data/preprocessed/en/news.2013.en.shuffled.tok"
preprocessEnMonolingual(n2013, n2013Out)

val n2014 = "/data/mifs_scratch/mh693/wmt15/data/monolingual/en/news.2014.en.shuffled"
val n2014Out = "/data/mifs_scratch/aaw35/wmt15/data/preprocessed/en/news.2014.en.shuffled.tok"
preprocessEnMonolingual(n2014, n2014Out)

