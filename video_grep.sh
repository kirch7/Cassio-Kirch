#!/bin/bash

mv one_system*dat old/

make optimize || exit 1
./a.out

rm pictures/[0-9]*.png &> /dev/null || echo "nada removido"

#moviename=alpha$(grep "\#define ALPHA " define.h | awk '{print $3*1}')_$(grep -P "\#define N " define.h | awk '{print $3}')particles_$(date | awk '{print $2 $3}').avi
moviename=$(ls one_system*dat).avi

range=$(grep "\#define RANGE " define.h | awk '{print $3*0.55}')
#echo $range

steps=$(( $(grep "\#define STEPS " define.h | awk '{print $3}') / $(grep "\#define EXIT_INTERVAL " define.h | awk '{print $3}') ))

echo -e "\nsteps: $steps"
for (( i=0; i<$steps ; i++ ))
do
    array[$i]=$i
    grep -P "^${array[$i]}\t" one_system*dat > ${array[$i]}.dat
    ((echo -e "set terminal png size 1200,1200 ; set xrange[-$range:$range] ; set yrange[-$range:$range] ; set output 'pictures/${array[$i]}.png' ; plot'./${array[$i]}.dat' u 4:5" | gnuplot) && (rm ${array[$i]}.dat)) &
    if [ $((i%50)) == 0 ]; then
	echo -ne "${array[$i]} " &
    fi
done



echo -e "\n\n"
cd pictures
avconv -r 10 -i %d.png -vcodec libx264 ../$moviename
rm [0-9]*.png >> /dev/null
