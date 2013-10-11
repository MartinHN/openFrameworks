#! /bin/bash
#
# GLEW
# OpenGL Extensions Wrangler
# http://glew.sourceforge.net/
#
# uses a Makefile build system,
# use "make glew.lib" to build only the lib without demos/tests
# the OPT flag is used for CFLAGS (& LDFLAGS I think?)

VER=1.9.0

FORMULA_TYPES=( "osx" "vs" "win_cb" )

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://downloads.sourceforge.net/project/glew/glew/$VER/glew-$VER.tgz
	tar -xf glew-$VER.tgz
	mv glew-$VER glew
	rm glew-$VER.tgz
}

# executed inside the lib src dir
function build() {

	if [ "$TYPE" == "osx" ] ; then
		# help from http://stackoverflow.com/questions/12229714/building-glew-for-mac-osx
		
		# 32 bit
		make clean; make glew.lib OPT="-arch i386"
		mv lib/libGLEW.a libGLEW-i386.a

		# 64 bit
		make clean; make glew.lib OPT="-arch x86_64"
		mv lib/libGLEW.a libGLEW-x86_64.a

		# link into universal lib
		lipo -c libGLEW-i386.a libGLEW-x86_64.a -o libGLEW.a

	elif [ "$TYPE" == "vs" ] ; then
		#cd build/vc10
		#MSBuild.exe glew_static.vcxproj
		echoWarning "TODO: build vs"

	elif [ "$TYPE" == "win_cb" ] ; then
		#make glew.lib
		echoWarning "TODO: build win_cb"
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include
	cp -Rv include/* $1/include

	# libs
	if [ "$TYPE" == "osx" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v libGLEW.a $1/lib/$TYPE/glew.a

	elif [ "$TYPE" == "vs" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/glew32s.lib $1/lib/$TYPE

	elif [ "$TYPE" == "win_cb" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v lib/glew32s.lib $1/lib/$TYPE
	fi
}

# executed inside the lib src dir
function clean() {

	if [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: clean vs"
	
	else
		make clean
		rm -f *.a *.lib
	fi
}
