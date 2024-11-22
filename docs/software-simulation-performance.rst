=============================
Speed up |EMOD_s| simulations
=============================

While small simulations can be run quickly on a local computer, the time and memory needed to complete a
simulation can grow significantly when simulations become larger and more complex. For example, a
single geographical node simulation might use 3 GB with one core in order to run successfully in one
minute or less, while another simulation job may require 32 GB in a dual-core system in order to
complete in approximately the same amount of time.

As your simulations grow, you will likely want to run simulations on an HPC cluster or take other
steps to improve performance and reduce processing time. This topic describes many of the steps you
can take to speed up |EMOD_s| simulations.

Parallel processing
===================

The |exe_l| is "single threaded", meaning that for processing, it will use only one core. However,
you can use the Message Passing Interface (MPI) to run multiple copies of |exe_s| in parallel,
either locally or on an HPC cluster. To run a simulation in parallel, you must invoke |exe_s| with
the mpiexec command. For more information, see :doc:`software-simulation-mpiexec`.

Parameter settings
==================

You can also set various parameters in the :term:`configuration file` that will improve performance
by scaling down the amount of data used or optimizing data processing. See the parameters listed
in the sections below for guidance on making performance adjustments. See :doc:`parameter-configuration`
for more information about each of the parameters.

Scaling and sampling
--------------------

**Simulation_Duration**
    Obviously, simulating a shorter timespan will take less processing time. However, the
    processing time is often driven by the number of infections or immune updates, so running a
    simulation after all infections have cleared may not increase processing time much. For more
    information, see :doc:`parameter-configuration-setup` parameters.
**Individual_Sampling_Type**
    Instead of the default **Individual_Sampling_Type** setting of "TRACK_ALL", you can speed up
    performance by sampling such that each individual object represents more than one person. For
    example, a simulation with a population of 1 million and sample rate of 0.1 would simulate
    100,000 individuals, with each given a weight of 10. Sampling can be fixed at a particular rate
    or can adapt the rate based on certain criteria, such as age or immune state. However, you
    should be especially careful not to undersample simulations to the point where they are overly
    sensitive to rare stochastic events. For more information, see :doc:`parameter-configuration-sampling`
    parameters.
**Population_Scale_Type** and **x_Base_Population**
    Alternatively, you can simply reduce the total population of the simulation using
    **Population_Scale_Type** set to "FIXED_SCALING" and **x_Base_Population** set to
    less than one. For more information, see :doc:`parameter-configuration-scalars` parameters.

Processing
----------

**Num_Cores**
    For large, spatially distributed simulations, running the intra-node dynamics (for example,
    infection and immune dynamics) in parallel on multiple cores may be very advantageous. Ideally,
    the timing would be reduced inversely to the number of cores. However, there are costs to
    serializing individuals for migration over MPI, as well as considerations for balancing the CPU
    load on each core. These issues may be mitigated using a load balancing input file that is
    suitable to the geography being simulated. See :doc:`software-load-balancing` for more information.


