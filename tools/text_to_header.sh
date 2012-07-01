#!/bin/sh
# This script from text file produces variable

if [ "$#" -ne "2" ]
then
    echo -e "Usage:\t\t$0  input-file  output-file";
else

    # produce file macro and text macro names
    file_macro=`echo "$2" | sed  -r         \
            -e 's|.*/([^/]+)|\1|g'          \
            -e 's/[ -.]/_/g'                \
            -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`
    
    text_macro=`echo $file_macro | sed  -r -e 's|([^_]+)_[^_]+$|\1|g'`
    
    # generate file
    echo "#ifndef $file_macro"               > $2;
    echo "#define $file_macro"               >> $2;
    echo "#define $text_macro \"\\"            >> $2;
    sed -e 's/$/\\n\\/g' -e 's/"/\\"/g'   $1    >> $2;
    echo "\""                                 >> $2;
    echo "#endif"                             >> $2;
fi

