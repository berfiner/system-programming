export HISTTIMEFORMAT='%Y-%m-%d '

b=`history > history_output.txt`

if [ "$1" = "today" ];then
newdate=$(date -v-0d +%F)

fi
if [ "$2" = "day" ];then
newdate=$(date -v-$1d +%F)

fi
if [ "$2" = "week" ]
then
newdate=$(date -v-$1w +%F )


fi
if [ "$2" = "month" ]
then
newdate=$(date -v-$1m +%F )


fi
a=`echo $newdate`

commands=`grep $a history_output.txt >a.txt`

awk '{ if (++words[$3] > max) { max = words[$3]; maxW=$0 } } END { print maxW }' a.txt
