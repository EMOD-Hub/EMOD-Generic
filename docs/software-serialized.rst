==========================
Serialized population file
==========================

To model a population with endemic disease, you cannot use a common  modeling technique in which you
introduce a disease outbreak to a naive population and analyze the immediate aftermath. Instead, you
must run the simulation for a period of time until the disease dynamics reach an equilibrium (aside
from  :term:`stochastic` noise). This is known as steady state. The simulation output prior to that
point is disregarded.  This is a modeling concept known as :term:`simulation burn-in`, borrowed from
the electronics industry where the first items produced by a manufacturing process are discarded
before the process is applied.

However, the time necessary to run simulations until this point can be significant, especially for
large populations. Indeed, for endemic disease present at low absolute prevalence, you should
simulate a larger population size that allows a small number of infected individuals to be
represented.

|EMOD_s| avoids the need to run the burn-in period again and again with each simulation by using
serialization to save the population state after it reaches equilibrium. Then, when you  want to
begin a subsequent simulation investigating the outcome of a particular set of interventions, you
can begin the simulation at that point rather than needing to re-run the burn-in period. You can
serialize the population at multiple time steps during a simulation.

The serialized population files created are placed in the output directory and use the naming
convention  state-<timestep>.dtk. They are binary files that contain state information about every
agent in a simulation: their health status, age, property values, and more. These files can be consumed
by subsequent simulations to decrease run time. 

.. note::

    If you used repeating interventions during the burn-in period, those interventions will not continue
    based on the information in the serialized population file. Check your campaign file for repeating
    interventions and reconfigure them as needed for the period after burn-in.

See :doc:`parameter-configuration-setup` parameters for more information.
