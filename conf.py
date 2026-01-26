class Args:
    nJobs = 10
    nEventsPerJob = 2000
    # path to urqmd executable
    UrQMDMain = '/somewhere/urqmd.x86_64'
    UrQMDTable = '/somewhere/tables.dat'
    # path to random seed list -> to ensure all the output events are unique
    rdsList = '/somewhere/rds.list'
    # target path
    targetPath = '/somewhere/to_save'
    # output format
    outputFormat = 'r'
