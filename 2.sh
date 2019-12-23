#!/bin/bash
`echo diff $1 $2`
`diff $1 $2 >  difference.txt`
line=`sed 's/^[^0-9]*//g' difference.txt`
echo "Please enter which change you would like to do"
read index
stringarray=($line)
change=${stringarray[$((index-1))]}

declare -a name;
filename="difference.txt"
r=0
while IFS= read -r value; do
    name[r]="$value"

    r=$((r+1))
done < $filename
declare -a ffile;
declare -a sfile;
h=0
while IFS= read -r one; do
    ffile[h]="$one"
   
    h=$((h+1))
done < $1
q=0
while IFS= read -r two; do
    sfile[q]="$two"

    q=$((q+1))
done < $2



  `cat $1 >  out.txt`
  



for i in "${!name[@]}"; do
   if [[ ${name[$i]} = ${change} ]]; then
       
       m=${i}
   fi
done
declare -a ar;
declare -a new;

change_index=`echo $change | grep -o "[0-9]*[,]*[0-9]$"`

    while [ "${name[m]}" !=  "${stringarray[$((index))]}" ]
    do
        
       ar[m]=`echo ${name[m]} | grep "^[>|<]"`
       
     m=$((m+1))
    
    done

    bar=()
    for f in "${ar[@]}" ; do
        bar+=( ${f:0:1} )
    done
   
    index=`echo $change | grep -o "^[0-9][,]*[0-9]*"`
    
   if [[ $change == *'c'* ]]; then
    IFS=', ' read -r -a array <<< "$change_index"
 
    IFS=', ' read -r -a array2 <<< "$index"
    temp=("${ffile[@]}")

    
         k=0
          j=${array[0]}
          j=$(($j-1))
          d=${array[1]}
          if [ -z "${array[1]}" ];then
            d=$(($j-1))
            else
            d=$(($d-$j))
           
            fi

        if [ ${bar[k]} == "<" ];then
         insert=${sfile[@]:$j:$d}
          i=${array2[0]}
          i=$(($i-1))
          echo $insert
           if [ -z "${array2[1]}" ];then
            k=$(($i+1))
            else
            k=${array2[1]}
           
            fi
 
     
          ffile=( "${ffile[@]:0:$i}" "$insert" "${ffile[@]:$k}")
         
         
    
         fi


 
fi
if [[ $change == *'a'* ]]; then
 IFS=', ' read -r -a array <<< "$change_index"
 k=0
 for j in "${array[@]}"
 do


 if [ ${bar[k]} == ">" ];then
 insert=${sfile[j-1]}
 
 ffile=( "${ffile[@]:0:$index}" "$insert" "${ffile[@]:$(($index+1))}" )
 fi
 done
 
 index=$(($index+1))

 fi


if [[ $change == *'d'* ]]; then

    IFS=', ' read -r -a array <<< "$change_index"
    IFS=', ' read -r -a array2 <<< "$index"
    temp=("${ffile[@]}")
    k=0

    for j in "${array2[@]}"
    do

    ffile=( "${ffile[@]:0:$(($j-1))}" "${ffile[@]:$(($j))}" )
done
    fi




echo ${sfile[@]}
echo ${ffile[@]}
  `echo ${ffile[@]} >  out.txt`
