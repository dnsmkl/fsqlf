#!/bin/sh

# Convert text file into C macro
# so it would be possible to use the text in source code.

# Usage:
#     text_to_header.sh  <input-file>  <output-file>

# C macro is put into guarded C header file.
# C macro, header file and guard macro names are based on output-file name.



if [ "$#" -ne "2" ]
then
    echo -e "Usage:\t\t$0  <input-file>  <output-file>";
else
    # Derive guard macro name (based on <output-file>):
    # - leave only file name (remove any parent folder)
    # - replace some chars (e.g. ".", " ") into underscore
    # - set upper case
    guard_macro_name=`echo "$2" | sed  -r   \
            -e 's|.*/([^/]+)|\1|g'          \
            -e 's/[ -.]/_/g'                \
            -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`

    # Derive text macro name (based on guard macro):
    # - remove last part (separated by underscore)
    text_macro_name=`echo $guard_macro_name | sed -r -e 's|([^_]+)_[^_]+$|\1|g'`

    # Generate output file.
    echo "#ifndef $guard_macro_name"            > $2;
    echo "#define $guard_macro_name"            >> $2;
    echo "#define $text_macro_name \"\\"        >> $2;
    sed -e 's/$/\\n\\/g' -e 's/"/\\"/g'  $1     >> $2; # Add '\n\' on new lines.
    echo "\""                                   >> $2;
    echo "#endif"                               >> $2;
fi

