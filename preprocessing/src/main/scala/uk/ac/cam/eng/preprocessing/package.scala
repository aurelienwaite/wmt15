package uk.ac.cam.eng

package object preprocessing {

	def truncateByLength(toFilter : (String, String), length: Int = 100 ) : Boolean = 
		toFilter._1.split(" ").length <= length && toFilter._2.split(" ").length <= length

	def testFertility(l1 : Int, l2 : Int)(min : Int, max : Int ) : Boolean = 
		(l1 == min && l2 > max) || (l2 == min && l1 > max)

	def truncateByFertility(toFilter : (String, String), fertility: Double = 2.4) : Boolean = {
		val length1 = toFilter._1.length
		val length2 = toFilter._2.length
		val test = testFertility(length1, length2)_
		val reject =  test(1, 5) || test(2, 8) || test(3, 9) || 
			(length1 > 3 && length1 > length2 * fertility) || (length2 > 3 && length2 > length1 * fertility) 
		!reject
	}
	
	def truncateIfMatch(toMatch : String)(toFilter : (String, String) ) : Boolean = 
		toFilter._1 != toMatch && toFilter._2 != toMatch

	def squeeze(toSqueeze : String) =
		toSqueeze.replaceAll("\\s+"," ").trim

}