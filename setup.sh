#!/bin/sh

OS=`uname -s`

cd build

$ORX/build/premake4 "gmake" 
$ORX/build/premake4 "codelite" 
$ORX/build/premake4 "codeblocks" 

if [ "${OS}" = "Darwin" ]; then
	$ORX/build/premake4 "xcode4" 
fi
 
cd ..