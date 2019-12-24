#/bin/bash/!
#receives the first part of the permission that the user wants to be the same in the files

search=$1
apply=$2
`ls -l >  permissions.txt`

filename="permissions.txt"
r=0
while IFS= read -r value; do
    name[r]="$value"
    
    r=$((r+1))
done < $filename

IFS=',' read -ra array <<< "$search"
for i in "${array[@]}"; do
    IFS='+' read -ra array2 <<< "$i"
   

done



if [ "${array2[0]}" == "u" ];then
for i in "${name[@]}"
do

    if [[ "${i:1:3}" == *"${array2[1]}"* ]];then
  
            a="$(echo $i |awk '{ print $9 }' )"
            `chmod $2 $a`

        fi
    
    done
    


fi
if [ "${array2[0]}" == "g" ];then
for i in "${name[@]}"
do
    
    if [[ "${i:4:3}" == *"${array2[1]}"* ]];then
        echo $i
           `chmod $2 $a`
        fi
    done
    
    
fi
if [ "${array2[0]}" == "a" ];then
for i in "${name[@]}"
do
    
    if [[ "${i:7:3}" == *"${array2[1]}"* ]];then
           `chmod $2 $a`
       fi
    done
fi

