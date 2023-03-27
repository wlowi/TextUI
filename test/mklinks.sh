#!/bin/bash
# 
# Create links for all cpp and header files from ../src hierarchy
#

SOURCE="../TextUI"

sh rmlinks.sh

for SOURCEDIR in $SOURCE ; do
    echo Link from $SOURCEDIR

#    for FILE in $SOURCEDIR/*.ino ; do
#        TARGET=`basename $FILE`
#        echo "  " $FILE " => " $TARGET
#        ln -s $FILE $TARGET
#    done

    for FILE in $SOURCEDIR/*.cpp ; do
        TARGET=`basename $FILE`
        echo "  " $FILE " => " $TARGET
        ln -s $FILE $TARGET
    done

    for FILE in $SOURCEDIR/*.h ; do
        TARGET=`basename $FILE`
        echo "  " $FILE " => " $TARGET
        ln -s $FILE $TARGET
    done
done

