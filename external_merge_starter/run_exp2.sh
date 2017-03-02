# Run disk_sort with 200 MB, 5 times: run_exp2.sh
# Run disk_sort with specified total memory, 1 time: run_exp2.sh <total memory in bytes>
# Run Unix sort 5 times: run_exp2.sh sort

block=8192
if [[ $# -eq 0 ]];then
    i=0
    while [[ $i -lt 5 ]];do
        cp records.dat records.dat$i
        /usr/bin/time -v disk_sort records.dat$i 209715200 $block 
        rm -f records.dat$i
        rm -f sorted*
        rm -f entire_sorted.dat
        (( i += 1 ))
    done
else
    if [[ "$1" = "sort" ]];then
        i=0
        while [[ $i -lt 5 ]];do
            cp edges.csv edges.csv$i
            /usr/bin/time -v sort -t"," -n -k2 edges.csv$i > edges_sorted_uid2.csv
            rm -f edges.csv$i
            rm -f edges_sorted_uid2.csv
            (( i += 1 ))
        done
    else
        mem=$1
        cp records.dat records.dat$mem
        /usr/bin/time -v disk_sort records.dat$mem $mem $block
        rm -f records.dat$mem
        rm -f sorted*
        rm -f entire_sorted.dat
    fi      
fi 
