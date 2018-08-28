rem Build project files/makefiles
cd build

"%ORX%/build/premake4" "gmake" 
"%ORX%/build/premake4" "codelite" 
"%ORX%/build/premake4" "codeblocks" 
"%ORX%/build/premake4" "vs2013" 
"%ORX%/build/premake4" "vs2015" 
"%ORX%/build/premake4" "vs2017"
 
cd..
