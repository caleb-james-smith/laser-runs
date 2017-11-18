#!/bin/bash
# rsync root file

# run script: ./giveMeTheData.sh -f caleb

dir=""
if [ "$1" = "-f" ]; then
    dir="Nov17-18_Final_CU_Data"
else if [ "$1" = "-p" ]; then
    dir="passed_cu_data"
else if [ "$1" = "-r" ]; then
    dir="rework_cu_data"
else if [ "$1" = "-s" ]; then
    dir="stability_cu_data"
else
    echo "Use these options for getting the root file for different data sets."
    echo "  -f Nov17-18_Final_CU_Data"
    echo "  -p passed_cu_data"
    echo "  -r rework_cu_data"
    echo "  -s stability_cu_data"
    exit 1
fi fi fi fi 
echo "You have selected: $dir"

user=""
if [ -z "$2" ]; then
    echo "Please enter your CERN username as the second argument."
    exit 1
fi
user="$2"

data_path="/nfshome0/caleb/laser-runs"

rsync -avz -e "ssh $user@lxplus.cern.ch ssh $user@cms904usr" :"$data_path/$dir/*.root" $dir
