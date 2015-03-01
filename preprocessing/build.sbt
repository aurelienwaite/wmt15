import NativePackagerKeys._

name := "preprocessing"

scalaVersion := "2.11.4"

sourcesInBase := false

libraryDependencies += "org.apache.spark" %% "spark-core" % "1.2.0"

enablePlugins(JavaAppPackaging)
