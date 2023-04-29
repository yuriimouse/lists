#!/bin/bash

V_MAJOR=1
V_MINOR=`git log --pretty=format:'%h' -n 1`
V_BUILD=`date +%s`
VERSION=$V_MAJOR.$V_MINOR.$V_BUILD
echo $VERSION