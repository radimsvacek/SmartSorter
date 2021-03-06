#!/bin/bash

printf "Script checks if everything needed for SmartSorter is installed!\n\n"

LIBS_OK=true

libs=( "libexif.so.12" "libpoppler.so.19" "libpoppler-qt4.so.3" )

Qt=( "libQtCore.so.4" "libQtGui.so.4" "libQtWebKit.so.4" "libQtXml.so.4")

for i in "${libs[@]}" 
do
	ldconfig -p | grep "$i" >/dev/null  2>&1 && {
		echo -e "\tOK: $i is installed."
	} || {
		echo "ERROR: $i is not installed."
		LIBS_OK=false
	}
done

for i in "${Qt[@]}" 
do
	ldconfig -p | grep "$i" >/dev/null  2>&1 && {
		echo -e "\tOK: $i is installed."
	} || {
		echo "ERROR: $i is not installed."
		LIBS_OK=false
	}
done

command -v "qmake" >/dev/null  2>&1 && {
	echo -e "\tOK: qmake is installed."
} || {
	echo "ERROR: qmake is not installed."
	LIBS_OK=false
}

if [ "$LIBS_OK" = true ] ; then
	echo -e "\nEverything is OK!\n\tMakefile was created!"
	echo "Now you can compile app with MAKE command."
	qmake ../smart_sorter.pro
else
	echo -e "\n\tError: Some libraries missing. Install all needed libraries first!"
fi
