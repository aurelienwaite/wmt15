#!/bin/bash
L=`readlink -f $0`
L=`dirname $L`/target/universal/stage/lib
cp=`echo $L/*.jar|sed 's/ /:/g'`  #*/
export LD_LIBRARY_PATH=/home/blue9/aaw35/wmt13/lib
exec /home/blue7/aaw35/libs/scala-2.11.5/bin/scala -J-Xmx10000m -classpath $cp $0 $@
!#

import org.apache.spark.SparkContext
import org.apache.spark.SparkContext._
import org.apache.spark.SparkConf
import java.io._
import java.util._
import scala.io.Source


val conf = new SparkConf().setMaster("local[10]").setAppName("Simple Application")
val sc = new SparkContext(conf)

val en = Source.fromFile("/data/mifs_scratch/mh693/wmt15/data/parallel/fr_en/commoncrawl.fr-en.en", "UTF-8")
val fr = Source.fromFile("/data/mifs_scratch/mh693/wmt15/data/parallel/fr_en/commoncrawl.fr-en.fr", "UTF-8")
val joined = sc.parallelize((en.getLines zip fr.getLines).toStream,100)
val preprocessed = joined.mapPartitions{iter => 
	{

		class PipedFunc(cmd : String) {
 			var p : Process = null
   		  	var in : BufferedReader = null
   			var out : BufferedWriter = null
   			init

   			def init() = {
   				p = Runtime.getRuntime().exec(cmd)
   				in = new BufferedReader( new InputStreamReader(p.getInputStream()) )
    			out = new BufferedWriter( new OutputStreamWriter(p.getOutputStream()) )
   			}

   			def apply(input : String) : Option[String] = {
   				try{
   					out.write(input + "\n")
   					out.flush
   					val ret = in.readLine()
   					Some(ret)
				}catch{
    				case t : Throwable => {
						System.err.println(t.getMessage)
    					init
    					return None 
    				}
   				}
			}
		}
		val enTokeniser = new PipedFunc("/home/blue7/aaw35/tools/wmt15/corpusTools.28set2005/intel/applyTokenizer /home/blue7/aaw35/tools/wmt15/corpusTools.28set2005/en.tokmap")
		val frTokeniser = new PipedFunc("/home/blue7/aaw35/tools/wmt15/perl_scripts/tokenizer-fr.perl -l fr")
		iter.flatMap{ elem => 
			val (enLine, frLine) = elem
			val enTok = enTokeniser(enLine)
			val frTok = frTokeniser(frLine)
			(enTok, frTok) match {
				case (_ : Some[String], _ : Some[String]) => Some((enTok.get, frTok.get))
				case _ => None
			}
		}
	}
}

preprocessed.saveAsTextFile("/data/mifs_scratch/aaw35/sparkTest")