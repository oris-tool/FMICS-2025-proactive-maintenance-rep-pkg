#!/bin/bash

echo "Installing faultflow dependency..."

mvn install:install-file \                          
    -Dfile=faultflow-paper.jar \
    -DgroupId=faultflow \
    -DartifactId=faultflow-paper \
    -Dversion=1.0-SNAPSHOT \
    -Dpackaging=jar \
    -Dclassifier=classes \
    -DgeneratePom=true