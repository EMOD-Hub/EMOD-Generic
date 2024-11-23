============================
Install |EMOD_s| on Windows
============================

To install |EMOD_s| on Windows computers, follow the steps below. You will install the pre-built
|exe_s| and all software necessary to run simulations locally.  Optionally, you can
install Python virtual environments, software to plot the output of simulations, and |EMOD_s| 
:term:`input files` for various regions.

.. include:: reuse/testing-windows.txt

.. note::

    If you want to download and modify the |EMOD_s| source code and build the |exe_s|
    yourself, see :doc:`dev-install-overview`.

.. contents::
   :local:

Install |EMOD_s|
================

.. include:: reuse/windows-install-to-run.txt

#.  On GitHub on the `EMOD releases`_ page, download the |exe_l|.  Save to a local drive, such as
    the desktop.

.. warning:: Double-clicking |exe_s| will not run the |EMOD_s| software or launch an installer. 
    You must run |EMOD_s| from the command line or a script. See :doc:`software-run-simulation` 
    for more information.

(Optional) Install plotting software
====================================

None of the following plotting software is required to run simulations with |EMOD_s|, but it
is useful for creating graphs from and evaluating the model output. In addition, |EMOD_s|
provides many Python scripts for analyzing data.

.. include:: reuse/third-party-note.txt

Python and Python packages
--------------------------

Python is required to install many of the software packages described below.

.. include:: reuse/python-install.txt

Python virtual environments
~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. include:: reuse/virtualenv-windows.txt

NumPy
~~~~~

.. include:: reuse/gohlke.txt

.. include:: reuse/numpy-install.txt


Python packages
~~~~~~~~~~~~~~~

.. include:: reuse/python-utility-install.txt


R
-

.. include:: reuse/r-install.txt

MATLAB
------

.. include:: reuse/matlab-install.txt

.. _EMOD releases: https://github.com/InstituteforDiseaseModeling/EMOD/releases

.. include:: reuse/install-inputdata.txt