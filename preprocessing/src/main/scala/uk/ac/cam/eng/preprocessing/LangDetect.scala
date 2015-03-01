package uk.ac.cam.eng.preprocessing

import com.cybozu.labs.langdetect.DetectorFactory
import com.cybozu.labs.langdetect.Language
import com.cybozu.labs.langdetect.LangDetectException;


class LangDetect(profiles : String){

	LangDetect.loadProfiles(profiles)

	def apply(lang : String, input : String) : Boolean = {
		if(input.split(" ").length < 10) return true
		try{
			val detector = DetectorFactory.create
			detector.append(input.trim);
			val langlist = detector.getProbabilities
			if (langlist.isEmpty()) return true
			val detected = langlist.get(0)
			return lang == detected.lang && detected.prob > 0.999995
		}catch{
			case t : Throwable => return true
		}	
	}

}

object LangDetect{

	var profilesLoaded = false

	def loadProfiles(profiles : String) = {
		LangDetect.synchronized {
			if (!profilesLoaded){
				DetectorFactory.loadProfile(profiles)
				profilesLoaded = true
			}
		}
	}
}