#!/bin/bash

printf "Please enter RBX number: "
read rbx
printf "Please enter CU number: "
read cu
printf "Please enter Iteration number: "
read iteration

shunt=0
host=hcal904daq04
commands=../uhtrRun.txt
init=../uhtrInit.txt
crate2=63;

rm_pdf=CU_"$cu"/rbx"$rbx"-rm1_"$iteration".pdf
cu_pdf=CU_"$cu"/rbx"$rbx"-cu-pd0_"$iteration".pdf
declare -a pdf_files=($rm_pdf $cu_pdf) 
for f in "${pdf_files[@]}"
do
  if [ -e "$f" ]; then
    printf "Are you sure you want to overwirte this file (y/n)? : "$f" : "
    read answer
    if [ $answer != "y" ]; then
      exit 1
    fi
  else
    echo "This file does not exist: "$f""
  fi 
done

if [ $rbx = "0" ]; then
    crate1=63; uhtr1=1; uhtr2=2; uhtr3=8; pd_ch="h0";
elif [ $rbx = "1" ]; then
    crate1=61; uhtr1=1; uhtr2=2; uhtr3=8; pd_ch="h72";
else if [ $rbx = "2" ]; then
    crate1=61; uhtr1=2; uhtr2=3; uhtr3=8; pd_ch="h84";
else if [ $rbx = "3" ]; then
    crate1=61; uhtr1=3; uhtr2=4; uhtr3=8; pd_ch="h96";
else if [ $rbx = "4" ]; then
    crate1=61; uhtr1=5; uhtr2=6; uhtr3=8; pd_ch="h108";
else if [ $rbx = "5" ]; then
    crate1=61; uhtr1=6; uhtr2=7; uhtr3=8; pd_ch="h120";
else if [ $rbx = "6" ]; then
    crate1=61; uhtr1=7; uhtr2=8; uhtr3=8; pd_ch="h132";
else if [ $rbx = "7" ]; then
    crate1=61; uhtr1=9; uhtr2=10; uhtr3=9; pd_ch="h0";
else if [ $rbx = "8" ]; then
    crate1=61; uhtr1=10; uhtr2=11; uhtr3=9; pd_ch="h12";
else if [ $rbx = "9" ]; then
    crate1=61; uhtr1=11; uhtr2=12; uhtr3=9; pd_ch="h24";
else if [ $rbx = "10" ]; then
    crate1=62; uhtr1=1; uhtr2=2; uhtr3=9; pd_ch="h36";
else if [ $rbx = "11" ]; then
    crate1=62; uhtr1=2; uhtr2=3; uhtr3=9; pd_ch="h48";
else if [ $rbx = "12" ]; then
    crate1=62; uhtr1=3; uhtr2=4; uhtr3=9; pd_ch="h60";
else if [ $rbx = "13" ]; then
    crate1=62; uhtr1=5; uhtr2=6; uhtr3=9; pd_ch="h72";
else if [ $rbx = "14" ]; then
    crate1=62; uhtr1=6; uhtr2=7; uhtr3=9; pd_ch="h84";
else if [ $rbx = "15" ]; then
    crate1=62; uhtr1=7; uhtr2=8; uhtr3=9; pd_ch="h96";
else if [ $rbx = "16" ]; then
    crate1=62; uhtr1=9; uhtr2=10; uhtr3=9; pd_ch="h108";
else if [ $rbx = "17" ]; then
    crate1=62; uhtr1=10; uhtr2=11; uhtr3=9; pd_ch="h120";
else if [ $rbx = "18" ]; then
    crate1=62; uhtr1=11; uhtr2=12; uhtr3=9; pd_ch="h132";
fi fi fi fi fi fi
fi fi fi fi fi fi
fi fi fi fi fi fi
# Fe, fi, fo, fum!
# I hear a giant beating his drum!

echo "RBX: $rbx"
echo "uHTR 1: $crate1:$uhtr1"
echo "uHTR 2: $crate1:$uhtr2"
echo "uHTR 3: $crate2:$uhtr3"

if [ ! -d CU_$cu ]; then
    mkdir CU_$cu
fi

cd CU_$cu

# write macro for CU
declare -a pd_channels=("h0" "h1" "h2" "h3" "h4" "h5")
pd=0
for ch in "${pd_channels[@]}"
do
  cu_file=macro_pd"$pd".C
  echo "{" > $cu_file
  echo ""$ch"->Draw();" >> $cu_file
  echo ""$ch"->SetTitle(\"CU "$cu" Pindiode Ch. "$pd", Shunt "$shunt", Iteration "$iteration";ADC;counts\");" >> $cu_file
  echo "c1->SetLogy();" >> $cu_file
  echo "c1->SaveAs(\"CU_pd"$pd".pdf\");" >> $cu_file
  echo "}" >> $cu_file
  pd=$((pd+1))
done

# write macros for RMs
declare -a rm_channels=("h12" "h84" "h12" "h84")
rm=1
for ch in "${rm_channels[@]}"
do
  rm_file=macro_rm"$rm".C
  echo "{" > $rm_file
  echo ""$ch"->Draw();" >> $rm_file
  echo ""$ch"->SetTitle(\"CU "$cu" RM "$rm" Fib. 0 Ch. 0, Shunt "$shunt", Iteration "$iteration";ADC;counts\");" >> $rm_file
  echo "c1->SetLogy();" >> $rm_file
  echo "c1->SaveAs(\"RM"$rm".pdf\");" >> $rm_file
  echo "}" >> $rm_file
  rm=$((rm+1))
done

# initialize links for calibration unit uHTR
uHTRtool.exe -o $host -c $crate2:$uhtr3 -s $init

# take data from 3 uHTRs
uHTRtool.exe -o $host -c $crate1:$uhtr1 -s $commands
mv data.root rbx"$rbx"_shunt"$shunt"_uhtr1_"$iteration".root
uHTRtool.exe -o $host -c $crate1:$uhtr2 -s $commands 
mv data.root rbx"$rbx"_shunt"$shunt"_uhtr2_"$iteration".root
uHTRtool.exe -o $host -c $crate2:$uhtr3 -s $commands 
mv data.root rbx"$rbx"_shunt"$shunt"_pd_"$iteration".root

# Use rm macros
root -b -q rbx"$rbx"_shunt"$shunt"_uhtr1_"$iteration".root macro_rm1.C
root -b -q rbx"$rbx"_shunt"$shunt"_uhtr1_"$iteration".root macro_rm2.C
root -b -q rbx"$rbx"_shunt"$shunt"_uhtr2_"$iteration".root macro_rm3.C
root -b -q rbx"$rbx"_shunt"$shunt"_uhtr2_"$iteration".root macro_rm4.C

for rm in `seq 1 4`;
do
  mv RM"$rm".pdf rbx"$rbx"-rm"$rm"_"$iteration".pdf
done
# Use cu macros
for pd in `seq 0 5`;
do
  root -b -q rbx"$rbx"_shunt"$shunt"_pd_"$iteration".root macro_pd"$pd".C
  mv CU_pd"$pd".pdf rbx"$rbx"-cu-pd"$pd"_"$iteration".pdf
done
