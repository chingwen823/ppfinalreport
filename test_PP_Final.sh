#!/bin/sh

TIMES_PER_FILE=3
FILE_LIST="ch3_standard_utf8.csv \
ch4_standard_utf8.csv \
ch5_standard_utf8.csv \
ch6_standard_utf8.csv \
ch7_standard_utf8.csv \
ch8_standard_utf8.csv \
shortline_standard_utf8.csv"

for core in 4 3 2 1;
do
	echo "-------------------------------------------------------------------------------"
	echo "$core threads:"
	for f in $FILE_LIST;
	do
		echo "$f"
		for i in `seq 1 $TIMES_PER_FILE`;
		do
			echo -n "$i, "
			/usr/bin/time -f "%e s" ./Test_PP_Final $core ../data/$f
			sleep 10;
		done
	done
	echo ""
done

