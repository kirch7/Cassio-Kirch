#!/bin/bash

mv one_system*dat old/
rm *.dat

#make optimize || exit 1
./boids4 || exit $?

rm pictures/[0-9]*.png &> /dev/null || echo "nada removido"

moviename=$(ls one_system*dat).avi

gpParameters="notitle w p ps 3 pt 7"

range=$(./boids4 -p | grep "RANGE " | awk '{print $3*0.5}')

steps=$(( $(./boids4 -p | grep "STEPS " | awk '{print $3}') / $(./boids4 -p | grep "EXIT_INTERVAL " | awk '{print $3}') ))

echo -e "\nsteps: $steps"
for (( i=0; i<$steps ; i++ ))
do
    array[$i]=$i
    #grep -P "^${array[$i]}\t" one_system*dat > ${array[$i]}.dat
    ((echo -e "set terminal png size 1200,1200;
               set xrange[-$range:$range];
               set yrange[-$range:$range];
               set output 'pictures/${array[$i]}.png';
               plot './${array[$i]}_0.dat' u 3:4 $gpParameters lc rgb 'red',\
 './${array[$i]}_1.dat' u 3:4 $gpParameters lc rgb 'blue' ") | gnuplot)
    if [ $((i%100)) == 0 ]; then
	echo -ne "${array[$i]} " &
    fi
done



echo -e "\n\n"
cd pictures

FFMpegParameters="-r 10 -i %d.png -vcodec libx264"
avconv $FFMpegParameters ../$moviename ||
    ffmpeg $FFMpegParameters ../$moviename

