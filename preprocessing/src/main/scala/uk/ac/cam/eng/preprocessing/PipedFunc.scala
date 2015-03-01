package uk.ac.cam.eng.preprocessing

import java.io._
import java.util._


class PipedFunc(cmd : String) {
	var p : Process = null
  	var in : BufferedReader = null
	var out : BufferedWriter = null
	init

	def init() = {
		p = Runtime.getRuntime().exec(cmd)
		in = new BufferedReader( new InputStreamReader(p.getInputStream(), "UTF-8") )
		out = new BufferedWriter( new OutputStreamWriter(p.getOutputStream(), "UTF-8") )
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
