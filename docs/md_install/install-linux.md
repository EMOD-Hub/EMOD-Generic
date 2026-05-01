# Install EMOD on Linux

To install EMOD on Linux computers, follow the steps below. You will install the pre-built
Eradication binary for Linux and all supporting software needed to run simulations locally. Optionally, you can
install Python virtual environments, software to plot the output of simulations, and EMOD
**input files** for various regions.

The Eradication binary for Linux is tested and supported on a CentOS 7.1 virtual machine on Azure.
It has also been successfully built and run on Ubuntu, SUSE, and Arch, but has not been tested
and is not supported on those Linux distributions.

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
of the Eradication binary for Linux, it also provides the option of installing the EMOD source code.

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
