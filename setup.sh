#!/bin/sh

cd build

$ORX/build/premake4 "gmake" 
$ORX/build/premake4 "codelite" 
$ORX/build/premake4 "codeblocks" 
$ORX/build/premake4 "xcode" 
 
cd..


