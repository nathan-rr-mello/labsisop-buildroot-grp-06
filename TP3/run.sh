if [ "$#" != "2" ];then 
    echo "Usage: ./run.sh <number_of_threads> <number_of_processors>"
    exit
fi
if [[ ! "$1" =~ ^[0-9]+$ ]] || [[ ! "$2" =~ ^[0-9]+$ ]];then
    echo "All the arguments must be an unsigned integer"
    exit
fi

gcc sched_profiler.c -o exe -lpthread

number_of_threads=$1
number_of_processors=$2

for ((i=1; i<=$number_of_processors; i++)); do
    processors=$(seq -s, 0 $((i - 1)))
    taskset -c $processors ./exe $number_of_threads $i
    echo taskset -c $processors ./exe $number_of_threads $i done
done