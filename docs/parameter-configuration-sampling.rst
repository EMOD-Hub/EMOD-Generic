========
Sampling
========

The following parameters determine how a population is sampled in the simulation. While you may
want every agent (individual object) to represent a single person, you can often optimize CPU
time with without degrading the accuracy of the simulation but having an agent represent
multiple people. The sampling rate may be adapted to have a higher or lower sampling rate for
particular regions or age groups.


.. include:: reuse/warning-case.txt

.. csv-table::
    :header: Parameter, Data type, Minimum, Maximum, Default, Description, Example
    :widths: 10, 5, 5, 5, 5, 20, 5
    :file: csv/config-sampling-generic.csv