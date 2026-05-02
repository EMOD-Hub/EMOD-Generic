# Install EMOD on Windows

To install EMOD on Windows computers, follow the steps below. You will install the pre-built
Eradication.exe and all software necessary to run simulations locally.  Optionally, you can
install Python virtual environments, software to plot the output of simulations, and EMOD
**input files** for various regions.

The EMOD executable (Eradication.exe) is tested using Windows 10, Windows Server 12, and Windows HPC Server 12 (64-bit). Windows HPC Server is used for testing remote
simulations on a **high-performance computing (HPC)** cluster and the other Windows
operating systems are used to test local simulations.

!!! note

        If you want to download and modify the EMOD source code and build the Eradication.exe
        yourself, see [dev-install-overview](../md_dev/dev-install-overview.md).

## Install EMOD

1. Install the Microsoft HPC Pack 2019 Client Utilities Redistributable Package (64-bit).
1. Install the Microsoft MPI v10.
1. Install the Microsoft Visual C++ 2022 Redistributable (64-bit).

### Python and Python packages

EMOD is built with an embedded Python interpreter. Having a local installation of Python is a requirement for many of the features to work correctly.
