==============================================
Run a simulation using the command line
==============================================

Using command-line options is the simplest way to run an |EMOD_s| simulation. This topic describes
the commands available for running simulations.

The |exe_l| and |linux_binary| also provide commands that provide information about the version of
|EMOD_s| that is installed, such as available parameters and simulation types. For more information,
see :doc:`parameter-schema`. The examples below show the Windows |exe_s|, but the options are the same
for the |linux_binary| on |Centos|.

Command options
===============

The following command-line options are available for running |EMOD_s| simulations. Paths can be
absolute or relative to the directory you are in when running the commands, unless otherwise
specified.

.. csv-table:: Simulation options
   :header: "Long form", "Short form", "Description"
   :widths: 3, 1, 10

   ``--version``, ``-v``, "Get version information."
   ``--schema-path``,, "Path to write the schema (the complete list of all parameters and associated information). If not specified, the schema will be written to stdout."
   ``--config``, ``-C``, "Path to the configuration file. If not specified, |EMOD_s| will look for a file named config.json in the current directory."
   ``--input-path``, ``-I``, "Path to the directory containing other :term:`input files` (except the 
   campaign file, which must be in the current directory). If not specified, |EMOD_s| will look for 
   files in the current directory. The specific demographics, climate, etc. files to use must be
   listed in the configuration file."
   ``--output-path``, ``-O``, "Path to the directory where output files will be written. If not specified, |EMOD_s| will create an ""output"" directory and overwrite any previous output in that directory."
   ``--dll-path``, ``-D``, "Path to the |module| root directory. For more information, see :doc:`software-custom-reporter`."
   ``--python-script-path``, ``-P``, "The path to Python scripts to use for pre- or post-processing."


The following options are for monitoring the progress of simulations running on an
:term:`high-performance computing (HPC)` cluster. They are optional for any simulation, but they must
be used together. To monitor progress, listen for User Datagram Protocol (UDP) messages on the specified
host and port.

.. csv-table:: HPC cluster options
   :header: "Long form", "Description"
   :widths: 3, 10

   ``--monitor_host``, "The IP address of the commissioning/monitoring host. Set to ""none"" for no IP address."
   ``--monitor_port``, "The port of the commissioning/monitoring host. Set to ""0"" for no port."
   ``--sim_id``, "The unique ID for this simulation.  This ID is needed for self-identification to the UDP host. Set to ""none"" for no simulation ID."
   ``--progress``, "Send updates on the progress of the simulation to the HPC job scheduler."

Usage
=====

#.  Open a Command Prompt window and navigate to the :term:`working directory`, which contains the
    configuration and campaign files.
#.  Enter a command like the one illustrated below, substituting the appropriate paths and file
    names for your simulation::

        ../Eradication.exe -C config.json -I C:\EMOD\InputFiles -O Sim1Output

    If you do not specify anything when invoking |exe_s|, it will not execute with all defaults, but
    will instead tell you how to invoke the ``--help`` command.

#.  |EMOD_s| will display logging information, including an errors that occur, while running
    the simulation. See :doc:`software-error-logging` for more information.

#.  Output files will be placed in the directory specified. For more information on how to evaluate
    and analyze the output, see :doc:`software-outputs`.
