#! /usr/bin/bash 

#Takes the two arguments from command line and stores in variables 
LOCATION1=$1
LOCATION2=$2

# gets info on first location from ipinfo.io
#saves information in location.txt to save on clutter in directory 
wget -O location.txt http://ipinfo.io/$LOCATION1
cat location.txt

# gets info on second location from ipinfo.io 
# stores information in location2.txt to save on clutter in directory 
wget -O location2.txt http://ipinfo.io/$LOCATION2
cat location2.txt



