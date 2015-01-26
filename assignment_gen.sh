#!/bin/bash

DIR="mi-run-kofrofil"
ZIP="mi-run-kofrofil.zip"
LOC=/tmp

rm -rf $LOC/$DIR
rm -rf $LOC/$ZIP

mkdir -p $LOC/$DIR
cp -rv ./* $LOC/$DIR/
cd $LOC
zip -r $ZIP $DIR

echo "Done. The result is in $LOC/$ZIP"
