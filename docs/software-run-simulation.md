# Running a simulation

The EMOD executable (Eradication.exe) consumes the **input files**, **configuration file**, and, optionally, **campaign file** to run simulations that model disease dynamics and campaign efficacy. The
simulation type controls the transmission mechanism of the disease. Each agent, whether human
or vector, is simulated and follows a set of rules that govern their health and behavior.

You have a few different options for running simulations. The option you choose will depend upon
whether you want to run one or more simulations, to run simulations locally or on a remote cluster
(for large simulations or multiple simulations), or to run simulations for debugging the source
code. This topic briefly describes the different options you have for running simulations. Because
the model is **stochastic**, you must run simulations multiple times to produce scientifically
valid results.

## Run a single simulation

It is simplest to run a single simulation using the EMOD command-line options. This will
run a single simulation and put the output files in a local directory.

You must either download the latest version of Eradication.exe from GitHub or clone the EMOD source from
GitHub and build Eradication.exe yourself. This gives you access to the latest features and parameters for
EMOD. To learn EMOD, you can also download the [EMOD scenarios](https://github.com/InstituteforDiseaseModeling/docs-emod-scenarios/releases), which contains all input files
needed to run local simulations that model a variety of disease scenarios.

## Run multiple simulations

Because the EMOD model is **stochastic**, simulations must be run multiple times to return
scientifically valid results. Therefore, you have the following options to run multiple simulations
at a time, either locally or remotely on a **high-performance computing (HPC)** cluster.
Generally, only small simulations should be run locally.

Many of these options are scripting languages that you can also use to modify the files consumed by
EMOD, simplifying your workflow when running many simulations.

## Run simulations for debugging

If you are helping advance the EMOD model by contributing to source code, there are other
options for running simulations that provide debugging support. These options for running
simulations are not recommended if you are not modifying the source code.

You can run a simulation locally from Visual Studio using the built-in debugger. You will be able
to put in breakpoints and step through the code while inspecting the values of different state
variables throughout the simulation.

You can use regression_test.py in the GitHub Regression_ directory to run multiple simulations on a
cluster, including running the suite of regression tests run by the IDM testing team. For more
information, see [dev-regression](dev-regression.md).

## Directory structure

Although there are many ways you can structure the files needed to run a simulation, we recommend
the following to keep your files organized and simplify the file paths set in the configuration file
or passed as arguments to Eradication.exe.

*   Place the configuration and campaign files needed for a simulation in the same directory. This
    is also known as the **working directory**.

    However, if you are using overlay files, you may want the default configuration or campaign file
    in a separate directory so they can be used with different overlay files for other simulations.

*   Place all demographics and climate files for a given region in the same directory.
*   Place output for a simulation in a subdirectory of the directory containing configuration
    and campaign files.

It is not important where you install Eradication.exe or the Eradication binary for Linux.

- [software-simulation-cli](software-simulation-cli.md)
- [software-simulation-mpiexec](software-simulation-mpiexec.md)
- [software-simulation-python](software-simulation-python.md)
- [software-simulation-matlab](software-simulation-matlab.md)
- [software-simulation-performance](software-simulation-performance.md)
