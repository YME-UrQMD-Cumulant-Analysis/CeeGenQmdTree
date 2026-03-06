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
export ftn13=qmd_ftn13.txt
export ftn14=qmd_ftn14.txt
sw13=__SW13__
sw14=__SW14__
kp13=__KP13__
cvt13=__CVT13__

if [ -x $qmdExec ]; then
    echo "[LOG] - Generator.sh: Running UrQMD main"
    ./$qmdExec
else 
    echo "[LOG] - Generator.sh: UrQMD executable ($qmdExec) not found"
    exit 701
fi

if [ "$sw13" = "true" ]; then
    if [ ! -f $ftn13 ]; then
        echo "[LOG] - Generator.sh: Output for ROOT doesn't exist"
        exit 702
    fi
    
    if [ "$cvt13" = "true" ]; then 
        ###### Convert
        cvtExec="cvt"
        if [ -x $cvtExec ]; then
            echo "[LOG] - Generator.sh: Running formatting convert program"
            ./$cvtExec $ftn13 __JID__
        else 
            echo "[LOG] - Generator.sh: convert executable ($cvtExec) not found"
            exit 703
        fi
    fi

    if [ "$kp13" = "false" ]; then
        ###### Remove raw output (mode: ROOT)
        rm $ftn13
    else
        mv $ftn13 __JID__.dat
    fi
fi

if [ "$sw14" = "true" ]; then
    ###### Rename
    if [ ! -f $ftn14 ]; then
        echo "[LOG] - Generator.sh: Output for CEE doesn't exist"
        exit 704
    fi
    mv $ftn14 __JID__.f14.dat
fi