# Install EMOD on Linux

To install EMOD on Linux computers, follow the steps below. You will install the pre-built
Eradication binary for Linux and all supporting software needed to run simulations locally. Optionally, you can
install Python virtual environments, software to plot the output of simulations, and EMOD
**input files** for various regions.

The Eradication binary for Linux is tested and supported on a CentOS 7.1 virtual machine on Azure.
It has also been successfully built and run on Ubuntu, SUSE, and Arch, but has not been tested
and is not supported on those Linux distributions.

!!! note

        If you want to download and modify the EMOD source code and build the Eradication binary for Linux
        yourself, see [dev-install-overview](dev-install-overview.md).

## Install EMOD using a script

The setup script installs most prerequisite software, including Python and the Python packages
dateutil, six, pyparsing, NumPy, and matplotlib. Other prerequisites, such as Boost 1.77.0 and
Microsoft MPI v10, are declared by the script as required. Because the installation instructions for
these packages will vary based on the particular Linux distribution you are running, installation
instructions are not included here.

!!! note

        IDM does not provide support or guarantees for any third-party software, even software
        that we recommend you install. Send feedback if you encounter any issues, but any support must come
        from the makers of those software packages and their user communities.

The script provides the option to install input files that describe the demographics, migration patterns, and
climate of many different locations across the world. While the script installs a pre-built version
of the Eradication binary for Linux, it also provides the option of installing the EMOD source code. For
information on building the Eradication binary for Linux from source code, see [dev-install-overview](dev-install-overview.md).

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

2.  Download the Eradication binary for Linux for CentOS 7.1 (not Eradication.exe for Windows). See on `EMOD releases`_
    on GitHub. Save to a local drive, such as the desktop.

## (Optional) Install Python virtual environments

Python virtual environments enable you to isolate your Python environments from one
another and give you the option to run multiple versions of Python on the same computer. When using a
virtual environment, you can indicate the version of Python you want to use and the packages you
want to install, which will remain separate from other Python environments. You may use
`virtualenv`, but `venv` is recommended and included with Python 3.3+.

1. Navigate to the location where you want to create the virtual environment directory and enter the
    following to create a environment called "idm", or other desired name:

```
        python -m venv idm
```

1. To activate the virtual environment, enter the following::

        source idm/bin/activate

1. Verify that the command prompt displays "idm" at the beginning, which indicates that you are in
    the idm virtual environment. For example:

```
        (idm) my-computer:idm
```

1. To verify you are using Python 3.9 64-bit, enter the following::

        python --version

1. When you are done working in this virtual environment, deactivate the environment. The
    environment will be saved and you can reactivate it at any time. To deactivate it, enter the following:

```
        deactivate
```

## (Optional) Install plotting software

None of the following plotting software is required to run simulations with EMOD, but they are
useful for creating graphs from and evaluating the model output. In addition, EMOD provides many
Python scripts for analyzing data.

R
-

R is a free software environment for statistical computing and graphics.

*  Go to https://www.r-project.org/ and install R 4.1.2 (64-bit).

### MATLAB

MATLAB is a high-level language and interactive environment for numerical computation,
visualization, and programming. The MATLAB Statistics and Machine Learning Toolbox™ provides
functions and applications to describe, analyze and model data using statistics and machine learning
algorithms.

1. Go to http://www.mathworks.com/products/matlab/ and install MATLAB R2021b.
1. If desired, go to https://www.mathworks.com/products/statistics.html and install the MATLAB Statistics and Machine Learning Toolbox™ R2021b.

# (Optional) Download input files

IDM provides **input files** that describe the demographics, migration patterns, and climate of
many different locations across the world. You can download these files from the EMOD-InputData repository, which uses large file storage (LFS) to manage the binaries and large
**JSON (JavaScript Object Notation)** files. A standard Git clone of the repository will only
retrieve the metadata for these files managed with LFS. To retrieve the actual data, follow the
steps below.

1. Install the Git LFS plugin, if it is not already installed.

    * For Windows users, download the plugin from https://git-lfs.github.com.
    * For CentOS users, the plugin is included with the PrepareLinuxEnvironment.sh script.

1. Using a Git client or Command Prompt window, clone the input data repository to retrieve
    the metadata:

```
        git clone https://github.com/InstituteforDiseaseModeling/EMOD-InputData.git
```

1. Navigate to the directory where you downloaded the metadata for the
    input files.

1. Download the actual data on your local machine::

        git lfs fetch

1. Replace the metadata in the files with the actual data::

        git lfs checkout
