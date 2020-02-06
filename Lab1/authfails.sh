
#!/usr/bin/bash

# Variable FILE is what the first argument passed in from command line 
FILE=$1

# Checks if a file has been supplied to the script or not 
#if  [ -z $1 ]; then
#	echo You forgot to supply a file 
#	exit 1 
#fi

# Gets all records with an authentication error 
# Gets  the IP address of those records and takes the top 10 unique occurences of an IP address and stores in top_10_ip.txt
grep -e authentication $FILE | grep -o '[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}'| sort | uniq -c | sort -rn| head -n 10 > top_10_ip.txt

# Print statement for testing
echo Top 10 ips written to: top_10_ip.txt

# Prints out top 10 ip's for testing purpouses 
more /home/niallquinn/lab1/top_10_ip.txt

# Gets the top IP address
max=`awk 'FNR == 1 {print $2}' top_10_ip.txt`

# Gets the second top IP address
max2=`awk 'FNR == 2 {print $2}' top_10_ip.txt` 

# Calls the location script with top two ip addresses to find out where they are located 
./locations.sh $max $max2
