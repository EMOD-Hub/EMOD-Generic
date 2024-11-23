=============================
Install |EMOD_s| on Linux
=============================

To install |EMOD_s| on Linux computers, follow the steps below. You will install the pre-built
|linux_binary| and all supporting software needed to run simulations locally. Optionally, you can
install Python virtual environments, software to plot the output of simulations, and |EMOD_s| 
:term:`input files` for various regions.

.. include:: reuse/testing-linux.txt

.. note::

    If you want to download and modify the |EMOD_s| source code and build the |linux_binary|
    yourself, see :doc:`dev-install-overview`.

.. contents::
   :local:


Install |EMOD_s| using a script
===============================

The setup script installs most prerequisite software, including Python and the Python packages
dateutil, six, pyparsing, NumPy, and matplotlib. Other prerequisites, such as |Boost_supp| and
|MSMPI_supp|, are declared by the script as required. Because the installation instructions for
these packages will vary based on the particular Linux distribution you are running, installation
instructions are not included here.

.. include:: reuse/third-party-note.txt

The script provides the option to install input files that describe the demographics, migration patterns, and
climate of many different locations across the world. While the script installs a pre-built version
of the |linux_binary|, it also provides the option of installing the |EMOD_s| source code. For
information on building the |linux_binary| from source code, see :doc:`dev-install-overview`.

.. include:: reuse/centos-install-to-run.txt

2.  Download the |linux_binary| for |Centos_supp| (not |exe_s| for Windows). See on `EMOD releases`_
    on GitHub. Save to a local drive, such as the desktop.

(Optional) Install Python virtual environments
==============================================

.. include:: reuse/virtualenv-linux.txt

(Optional) Install plotting software
====================================

None of the following plotting software is required to run simulations with |EMOD_s|, but they are
useful for creating graphs from and evaluating the model output. In addition, |EMOD_s| provides many
Python scripts for analyzing data.

R
-

.. include:: reuse/r-install.txt

MATLAB
------

.. include:: reuse/matlab-install.txt

.. include:: reuse/install-inputdata.txt