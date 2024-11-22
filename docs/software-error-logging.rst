=======================
Error and logging files
=======================

When you run a simulation, |EMOD_s| will output basic error and logging information to help track
the progress and help debug any issues that may occur. If you run the simulation on an HPC
cluster, the cluster will generate additional logging and error files. See :doc:`troubleshooting`
if you need help resolving an error.

Status
======

A status.txt file will be saved to the :term:`working directory` that provides one
output line per time step and includes the total run time of the simulation. A simulation
with 50 time steps will look something like this::

    Beginning Simulation...
    1 of 50 steps complete.
    2 of 50 steps complete.
    3 of 50 steps complete.
    4 of 50 steps complete.
    5 of 50 steps complete.
    6 of 50 steps complete.
    7 of 50 steps complete.
    8 of 50 steps complete.
    9 of 50 steps complete.
    10 of 50 steps complete.
    11 of 50 steps complete.
    12 of 50 steps complete.
    13 of 50 steps complete.
    14 of 50 steps complete.
    15 of 50 steps complete.
    16 of 50 steps complete.
    17 of 50 steps complete.
    18 of 50 steps complete.
    19 of 50 steps complete.
    20 of 50 steps complete.
    21 of 50 steps complete.
    22 of 50 steps complete.
    23 of 50 steps complete.
    24 of 50 steps complete.
    25 of 50 steps complete.
    26 of 50 steps complete.
    27 of 50 steps complete.
    28 of 50 steps complete.
    29 of 50 steps complete.
    30 of 50 steps complete.
    31 of 50 steps complete.
    32 of 50 steps complete.
    33 of 50 steps complete.
    34 of 50 steps complete.
    35 of 50 steps complete.
    36 of 50 steps complete.
    37 of 50 steps complete.
    38 of 50 steps complete.
    39 of 50 steps complete.
    40 of 50 steps complete.
    41 of 50 steps complete.
    42 of 50 steps complete.
    43 of 50 steps complete.
    44 of 50 steps complete.
    45 of 50 steps complete.
    46 of 50 steps complete.
    47 of 50 steps complete.
    48 of 50 steps complete.
    49 of 50 steps complete.
    50 of 50 steps complete.
    Done - 0:00:02

Standard output
===============

When you run a simulation on an HPC cluster, it will generate a standard output logging file
(StdOut.txt) in the working directory that captures all standard output messages. When you run a
simulation locally, the Command Prompt window will display the same information. If you want to save
this information to a text file instead, you can append ``> stdout.txt`` to the command used to
run the local |EMOD_s| simulation to redirect the console output to the file specified.

The file contains information about a particular simulation, such as the |EMOD_s| version used and
the files in use, as well as other initialization information, including the default logging level
and the logging levels set for particular modules. The file follows that information
with log output using the following format: <timestep><HPC rank><log level><module><message>.

By default, the logging level is set to "INFO". If you want to change the logging level,
see :doc:`dev-logging`.

For example::


    ..\..\..\Eradication.exe --config config.json --input-path ..\..\..\Demographics_Files
    Intellectual Ventures(R)/EMOD Disease Transmission Kernel 2.18.16.0
    Built on Jul  5 2018 15:32:59 by SYSTEM from master (e98fbf4) checked in on 2018-06-12 11:32:40 -0700
    Supports sim_types: GENERIC, VECTOR, MALARIA, AIRBORNE, TBHIV, STI, HIV, PY.
    Using config file: config.json
    Using input path: ..\..\..\Demographics_Files
    Using output path: output
    Using dll path:
    Python not initialized because --python-script-path (-P) not set.
    Initializing environment...
    Log-levels:
            Default -> INFO
            Eradication -> INFO
    00:00:00 [0] [I] [Eradication] Loaded Configuration...
    00:00:00 [0] [I] [Eradication] 56 parameters found.
    00:00:00 [0] [I] [Eradication] Initializing Controller...
    00:00:00 [0] [I] [Controller] DefaultController::execute_internal()...
    00:00:00 [0] [I] [Simulation] Using PSEUDO_DES random number generator.
    00:00:00 [0] [I] [DllLoader] ReadEmodulesJson: no file, returning.
    00:00:00 [0] [I] [DllLoader] dllPath not passed in, getting from EnvPtr
    00:00:00 [0] [I] [DllLoader] Trying to copy from string to wstring.
    00:00:00 [0] [I] [DllLoader] DLL ws root path:
    00:00:00 [0] [W] [Simulation] 00:00:00 [0] [W] [Simulation] Failed to load reporter emodules for SimType: GENERIC_SIM from path: reporter_plugins
    Failed to load reporter emodules for SimType: GENERIC_SIM from path: reporter_plugins
    00:00:00 [0] [I] [Simulation] Found 0 Custom Report DLL's to consider loading, load_all_reports=1
    00:00:00 [0] [I] [Controller] DefaultController::execute_internal() populate simulation...
    00:00:00 [0] [I] [Simulation] Campaign file name identified as: campaign.json
    00:00:00 [0] [I] [Climate] Initialize
    00:00:00 [0] [I] [LoadBalanceScheme] Using Checkerboard Load Balance Scheme.
    00:00:00 [0] [I] [Simulation] Looking for campaign file campaign.json
    00:00:00 [0] [I] [Simulation] Found campaign file successfully.
    00:00:00 [0] [I] [DllLoader] ReadEmodulesJson: no file, returning.
    00:00:00 [0] [I] [DllLoader] dllPath not passed in, getting from EnvPtr
    00:00:00 [0] [I] [DllLoader] Trying to copy from string to wstring.
    00:00:00 [0] [I] [DllLoader] DLL ws root path:
    00:00:00 [0] [W] [Simulation] 00:00:00 [0] [W] [Simulation] Failed to load intervention emodules for SimType: GENERIC_SIM from path: interventions
    Failed to load intervention emodules for SimType: GENERIC_SIM from path: interventions
    00:00:01 [0] [I] [JsonConfigurable] Using the default value ( "Number_Repetitions" : 1 ) for unspecified parameter.
    00:00:01 [0] [I] [JsonConfigurable] Using the default value ( "Timesteps_Between_Repetitions" : -1 ) for unspecified parameter.
    00:00:01 [0] [I] [JsonConfigurable] Using the default value ( "Incubation_Period_Override" : -1 ) for unspecified parameter.
    00:00:01 [0] [I] [Simulation] populateFromDemographics() created 1 nodes
    00:00:01 [0] [I] [Simulation] populateFromDemographics() generated 1 nodes.
    00:00:01 [0] [I] [Simulation] Rank 0 contributes 1 nodes...
    00:00:01 [0] [I] [Simulation] Merging node rank maps...
    00:00:01 [0] [I] [Simulation] Merged rank 0 map now has 1 nodes.
    00:00:01 [0] [I] [Simulation] Rank map contents not displayed until NodeRankMap::ToString() (re)implemented.
    00:00:01 [0] [I] [Simulation] Initialized 'InsetChart.json' reporter
    00:00:01 [0] [I] [Simulation] Initialized 'BinnedReport.json' reporter
    00:00:01 [0] [I] [Simulation] Initialized 'DemographicsSummary.json' reporter
    00:00:01 [0] [I] [Simulation] Update(): Time: 1.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:01 [0] [I] [SimulationEventContext] Time for campaign event. Calling Dispatch...
    00:00:01 [0] [I] [SimulationEventContext] 1 node(s) visited.
    00:00:01 [0] [I] [JsonConfigurable] Using the default value ( "Incubation_Period_Override" : -1 ) for unspecified parameter.
    00:00:01 [0] [I] [StandardEventCoordinator] UpdateNodes() gave out 4 'OutbreakIndividual' interventions at node 1
    00:00:01 [0] [I] [Simulation] Update(): Time: 2.0 Rank: 0 StatPop: 10000 Infected: 4
    00:00:01 [0] [I] [Simulation] Update(): Time: 3.0 Rank: 0 StatPop: 10000 Infected: 13
    00:00:01 [0] [I] [Simulation] Update(): Time: 4.0 Rank: 0 StatPop: 10000 Infected: 65
    00:00:01 [0] [I] [Simulation] Update(): Time: 5.0 Rank: 0 StatPop: 10000 Infected: 283
    00:00:01 [0] [I] [Simulation] Update(): Time: 6.0 Rank: 0 StatPop: 10000 Infected: 1149
    00:00:01 [0] [I] [Simulation] Update(): Time: 7.0 Rank: 0 StatPop: 10000 Infected: 3777
    00:00:01 [0] [I] [Simulation] Update(): Time: 8.0 Rank: 0 StatPop: 10000 Infected: 7268
    00:00:01 [0] [I] [Simulation] Update(): Time: 9.0 Rank: 0 StatPop: 10000 Infected: 7065
    00:00:01 [0] [I] [Simulation] Update(): Time: 10.0 Rank: 0 StatPop: 10000 Infected: 5578
    00:00:01 [0] [I] [Simulation] Update(): Time: 11.0 Rank: 0 StatPop: 10000 Infected: 4377
    00:00:01 [0] [I] [Simulation] Update(): Time: 12.0 Rank: 0 StatPop: 10000 Infected: 3392
    00:00:01 [0] [I] [Simulation] Update(): Time: 13.0 Rank: 0 StatPop: 10000 Infected: 2640
    00:00:01 [0] [I] [Simulation] Update(): Time: 14.0 Rank: 0 StatPop: 10000 Infected: 2054
    00:00:01 [0] [I] [Simulation] Update(): Time: 15.0 Rank: 0 StatPop: 10000 Infected: 1624
    00:00:01 [0] [I] [Simulation] Update(): Time: 16.0 Rank: 0 StatPop: 10000 Infected: 1247
    00:00:01 [0] [I] [Simulation] Update(): Time: 17.0 Rank: 0 StatPop: 10000 Infected: 975
    00:00:01 [0] [I] [Simulation] Update(): Time: 18.0 Rank: 0 StatPop: 10000 Infected: 742
    00:00:01 [0] [I] [Simulation] Update(): Time: 19.0 Rank: 0 StatPop: 10000 Infected: 605
    00:00:01 [0] [I] [Simulation] Update(): Time: 20.0 Rank: 0 StatPop: 10000 Infected: 469
    00:00:01 [0] [I] [Simulation] Update(): Time: 21.0 Rank: 0 StatPop: 10000 Infected: 355
    00:00:01 [0] [I] [Simulation] Update(): Time: 22.0 Rank: 0 StatPop: 10000 Infected: 267
    00:00:01 [0] [I] [Simulation] Update(): Time: 23.0 Rank: 0 StatPop: 10000 Infected: 206
    00:00:01 [0] [I] [Simulation] Update(): Time: 24.0 Rank: 0 StatPop: 10000 Infected: 164
    00:00:01 [0] [I] [Simulation] Update(): Time: 25.0 Rank: 0 StatPop: 10000 Infected: 122
    00:00:01 [0] [I] [Simulation] Update(): Time: 26.0 Rank: 0 StatPop: 10000 Infected: 89
    00:00:01 [0] [I] [Simulation] Update(): Time: 27.0 Rank: 0 StatPop: 10000 Infected: 73
    00:00:01 [0] [I] [Simulation] Update(): Time: 28.0 Rank: 0 StatPop: 10000 Infected: 57
    00:00:01 [0] [I] [Simulation] Update(): Time: 29.0 Rank: 0 StatPop: 10000 Infected: 46
    00:00:01 [0] [I] [Simulation] Update(): Time: 30.0 Rank: 0 StatPop: 10000 Infected: 32
    00:00:01 [0] [I] [Simulation] Update(): Time: 31.0 Rank: 0 StatPop: 10000 Infected: 22
    00:00:01 [0] [I] [Simulation] Update(): Time: 32.0 Rank: 0 StatPop: 10000 Infected: 17
    00:00:01 [0] [I] [Simulation] Update(): Time: 33.0 Rank: 0 StatPop: 10000 Infected: 16
    00:00:01 [0] [I] [Simulation] Update(): Time: 34.0 Rank: 0 StatPop: 10000 Infected: 15
    00:00:01 [0] [I] [Simulation] Update(): Time: 35.0 Rank: 0 StatPop: 10000 Infected: 10
    00:00:01 [0] [I] [Simulation] Update(): Time: 36.0 Rank: 0 StatPop: 10000 Infected: 6
    00:00:01 [0] [I] [Simulation] Update(): Time: 37.0 Rank: 0 StatPop: 10000 Infected: 4
    00:00:01 [0] [I] [Simulation] Update(): Time: 38.0 Rank: 0 StatPop: 10000 Infected: 3
    00:00:01 [0] [I] [Simulation] Update(): Time: 39.0 Rank: 0 StatPop: 10000 Infected: 3
    00:00:01 [0] [I] [Simulation] Update(): Time: 40.0 Rank: 0 StatPop: 10000 Infected: 2
    00:00:01 [0] [I] [Simulation] Update(): Time: 41.0 Rank: 0 StatPop: 10000 Infected: 1
    00:00:01 [0] [I] [Simulation] Update(): Time: 42.0 Rank: 0 StatPop: 10000 Infected: 1
    00:00:01 [0] [I] [Simulation] Update(): Time: 43.0 Rank: 0 StatPop: 10000 Infected: 1
    00:00:01 [0] [I] [Simulation] Update(): Time: 44.0 Rank: 0 StatPop: 10000 Infected: 1
    00:00:01 [0] [I] [Simulation] Update(): Time: 45.0 Rank: 0 StatPop: 10000 Infected: 1
    00:00:01 [0] [I] [Simulation] Update(): Time: 46.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:01 [0] [I] [Simulation] Update(): Time: 47.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:01 [0] [I] [Simulation] Update(): Time: 48.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:01 [0] [I] [Simulation] Update(): Time: 49.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:01 [0] [I] [Simulation] Update(): Time: 50.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:01 [0] [I] [Simulation] Update(): Time: 51.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:01 [0] [I] [Simulation] Update(): Time: 52.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:01 [0] [I] [Simulation] Update(): Time: 53.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 54.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 55.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 56.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 57.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 58.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 59.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 60.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 61.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 62.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 63.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 64.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 65.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 66.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 67.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 68.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 69.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 70.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 71.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 72.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 73.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 74.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 75.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 76.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 77.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 78.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 79.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 80.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 81.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 82.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 83.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 84.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 85.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 86.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 87.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 88.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 89.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Update(): Time: 90.0 Rank: 0 StatPop: 10000 Infected: 0
    00:00:02 [0] [I] [Simulation] Finalizing 'InsetChart.json' reporter.
    00:00:02 [0] [I] [Simulation] Finalized  'InsetChart.json' reporter.
    00:00:02 [0] [I] [Simulation] Finalizing 'BinnedReport.json' reporter.
    00:00:02 [0] [I] [Simulation] Finalized  'BinnedReport.json' reporter.
    00:00:02 [0] [I] [Simulation] Finalizing 'DemographicsSummary.json' reporter.
    00:00:02 [0] [I] [Simulation] Finalized  'DemographicsSummary.json' reporter.
    00:00:02 [0] [I] [Controller] Exiting DefaultController::execute_internal
    00:00:02 [0] [I] [Eradication] Controller executed successfully.


Standard error
==============

When you run a simulation on an HPC cluster, it will also generate a standard error logging file
(StdErr.txt) in the working directory that captures all standard error messages.
