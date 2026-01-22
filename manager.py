import os
import sys
import random
from conf import Args
from yLog import yLog

l = yLog('.Manager.log')

if len(sys.argv) != 2:
    raise Exception('Usage: python3 manager.py submit')

mode = sys.argv[1]
assert(mode == 'submit')

l.log(f'[LOG] - Manager System: Submit mode')
nJobs = Args.nJobs
nEventsPerJob = Args.nEventsPerJob
l.log(f'[LOG] - Manager System: Number of Events {nJobs} x {nEventsPerJob} = {nJobs*nEventsPerJob:.2e}')

rdsList = Args.rdsList
rds = None
if os.path.exists(rdsList):
    with open(rdsList, 'r') as f:
        rds = f.readlines()
    rds = [(int)(item.strip()) for item in rds]
    l.log(f'[LOG] - Manager System: {len(rds)} seeds used according to {rdsList}')
else:
    rds = []
    l.log(f'[LOG] - Manager System: The random seed list is currently empty')

existing_set = set(rds)

UrQMDMain = Args.UrQMDMain
UrQMDTable = Args.UrQMDTable
assert(os.path.exists(UrQMDMain))
assert(os.path.exists(UrQMDTable))
mainBaseName = os.path.basename(UrQMDMain)
tableBaseName = os.path.basename(UrQMDTable)

targetPath = Args.targetPath
assert(os.path.exists(targetPath))
l.log(f'[LOG] - Manager System: Target path: {targetPath}')

max_attempts = 1000 # in principle we won't reach this maximum
for idx in range(nJobs):
    thisSeed = None
    for _ in range(max_attempts):
        cdd = random.randint(1, 2**31 - 1)
        if cdd not in existing_set:
            thisSeed = cdd
            break
    rds.append(thisSeed)
    existing_set.add(thisSeed)

    # prepare the folder
    os.mkdir(f'{targetPath}/job{idx}')
    # and link executbale + table
    os.system(f'ln -s {UrQMDMain} {targetPath}/job{idx}/{mainBaseName}')
    os.system(f'ln -s {UrQMDTable} {targetPath}/job{idx}/{tableBaseName}')
    os.system(f'ln -s {os.getcwd()}/cvt {targetPath}/job{idx}/cvt')
    # then copy the configuration files
    os.system(f'cp {os.getcwd()}/input.txt {targetPath}/job{idx}/qmd_input.txt')
    os.system(f'cp {os.getcwd()}/generator.job {targetPath}/job{idx}/generator.job')
    os.system(f'cp {os.getcwd()}/Generator.sh {targetPath}/job{idx}/Generator.sh')
    # set job id in script (just for changing the output root file name)
    os.system(f'sed -i "s|__JID__|{idx}|g" {targetPath}/job{idx}/Generator.sh')
    os.system(f'chmod +x {targetPath}/job{idx}/Generator.sh')
    # replace key words in the input file
    os.system(f'sed -i "s|__NEV__|{nEventsPerJob}|g" {targetPath}/job{idx}/qmd_input.txt')
    os.system(f'sed -i "s|__RSD__|{thisSeed}|g" {targetPath}/job{idx}/qmd_input.txt')

    os.system(f'cd {targetPath}/job{idx} && condor_submit generator.job')
    l.log(f'[LOG] - Manager System:Job {idx} submitted, random seed: {thisSeed}')

l.log('[LOG] - Manager System: All jobs submitted.')

with open(rdsList, 'w') as f:
    f.write("\n".join(str(item) for item in rds))


l.log(f'[LOG] - Manager System: In total {len(rds)} random seeds used.')

        