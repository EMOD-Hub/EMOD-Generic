# Install CentOS prerequisites for EMOD source code

This section describes the software packages or utilities must be installed on computers running
CentOS 7.1 to build the  Eradication binary for Linux from source code and run regression tests.

If additional software is needed for the prerequisite software due to your specific environment, the
installer for the prerequisite software should provide instructions. For example, if Microsoft MPI v10
requires additional Visual C++ redistributable packages, the installer will display that
information.

!!! note

        IDM does not provide support or guarantees for any third-party software, even software
        that we recommend you install. Send feedback if you encounter any issues, but any support must come
        from the makers of those software packages and their user communities.

## Install prerequisites for running simulations

The following software packages are required to run simulations using the Eradication binary for Linux. If you already
installed the pre-built Eradication.exe using the instructions in [install-overview](install-overview.md), you can skip
this section.

Before you begin, you must have the following:

* **sudo** privileges to install packages
* 15 GB free in your home directory (if you install the EMOD source code and input files)
* An Internet connection

1. On GitHub on the `EMOD releases`_ page, download and run the PrepareLinuxEnvironment.sh script.

    Respond to the prompts for information while the script is running.

    a.  Set the EMOD_ROOT environment variable to the path to the EMOD source path:

```
            EMOD_ROOT=~/IDM/EMOD
```

    b.  Include Scripts and . in the path:

```
            export PATH=$PATH:.:$EMOD_ROOT/Scripts
```

    c.  If you want to run simulations in the same session that you updated EMOD_ROOT and
        the Scripts path, reload the .bashrc file using `source .bashrc`.

## Install prerequisites for compiling from source code

For CentOS 7.1, all prerequisites for building the Eradication binary for Linux are installed by the setup
script described above. However, if you originally installed EMOD without including
the source code and input files that are optional for running simulations using a pre-built
Eradication binary for Linux, rerun the script and install those.

## Install prerequisites for running regression tests

The setup script includes most plotting software required for running regression tests, where graphs
of model output are created both before and after source code changes are made to see if those
changes created a discrepancy in the regression test output. For more information, see
[dev-regression](dev-regression.md). You may want to install R or MATLAB, but both are optional.

We recommended that you download some of the NumPy Python package from
http://www.lfd.uci.edu/~gohlke/pythonlibs, a page compiled by Christoph Gohlke, University of
California, Irvine. The libraries there include linear algebra packages that are not included by default with the standard Windows packages. They are compiled Windows binaries, including the 64-bit versions
required by EMOD. The naming convention used lists the Python version after "cp", for example "cp36-cp36m", and the Windows bit version after "win", for example "win_amd64".

### (Optional) R

The IDM test team uses R 4.1.2 (64-bit) for regression testing, but it is considered optional.

R is a free software environment for statistical computing and graphics.

*  Go to https://www.r-project.org/ and install R 4.1.2 (64-bit).

### (Optional) MATLAB

The IDM test team uses MATLAB R2021b and the MATLAB Statistics and Machine Learning Toolbox™ R2021b for regression testing, but they are both
considered optional.

MATLAB is a high-level language and interactive environment for numerical computation,
visualization, and programming. The MATLAB Statistics and Machine Learning Toolbox™ provides
functions and applications to describe, analyze and model data using statistics and machine learning
algorithms.

1. Go to http://www.mathworks.com/products/matlab/ and install MATLAB R2021b.
1. If desired, go to https://www.mathworks.com/products/statistics.html and install the MATLAB Statistics and Machine Learning Toolbox™ R2021b.
