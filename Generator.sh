#!/bin/bash
#SBATCH --job-name=urqmd
#SBATCH --cpus-per-task=1
#SBATCH --ntasks=1
#SBATCH --error=.urqmd.err
#SBATCH --output=.urqmd.out
#SBATCH --partition=debug

###### Main script for generating UrQMD data

qmdExec="urqmd.x86_64"
cvtExec="cvt"
export ftn09=qmd_input.txt
export ftn13=qmd_output.txt

if [ -x $qmdExec ]; then
    echo "[LOG] - Generator.sh: Running UrQMD main"
    ./$qmdExec
else 
    ehco "[LOG] - Generator.sh: UrQMD executable ($qmdExec) not found"
    exit 701
fi

###### Convert

cvtExec="cvt"
if [ ! -f $ftn13 ]; then
    echo "[LOG] - Generator.sh: Output doesn't exist"
    exit 702
fi
if [ -x $cvtExec ]; then
    echo "[LOG] - Generator.sh: Running formatting convert program"
    ./$cvtExec __JID__
else 
    echo "[LOG] - Generator.sh: convert executable ($cvtExec) not found"
    exit 703
fi

###### Remove raw output
rm $ftn13