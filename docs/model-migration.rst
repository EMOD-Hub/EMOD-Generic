====================
Geographic migration
====================

Human migration is a important factor in the spread of disease across a geographic region. |EMOD_s|
represents geography using nodes. Migration occurs when individuals move from one :term:`node` to
another; disease transmission occurs *within* nodes. Therefore, infected individuals can migrate to
nodes without disease and introduce disease transmission into that node. Nodes are very flexible and
can represent everything from individual households to entire countries or anything in between.
Therefore, to include migration in a simulation, you must define multiple nodes.

At each time step, individuals in each node have a defined probability of migrating out of their
current node to another. You can also define the average length of time individuals will stay in
their destination node before migrating again. If you are using timesteps longer than one day and
the time to next migration falls between timesteps, individuals will migrate at the following
timestep. For example, if you use 7-day timesteps and an individual draws a 12-day trip duration,
they won't migrate until day 14.

The mode of migration can be local (foot travel), regional (by roadway or rail), by air, or by sea.
You can also define different migration patterns, such as one-way or roundtrip. Individuals have a
"home node" that is relevant for some types of migration, such as migrating an entire family unit
only when all members are home or returning home after passing through several waypoints. For more
detailed information, see :doc:`parameter-configuration-migration` parameters.


You must include a separate migration file for each mode of travel that describes the migration
patterns for each node. It lists the migration rate for each node. Migration rate is defined as the
fraction of the node’s population that is migrating *out* of the node per day. Units are per person
per day, meaning the number of people migrating per day divided by the total population of the node.
For more information on the structure of these files, see :doc:`software-migration`.

The Generic/Zoonosis scenario in the downloadable `EMOD scenarios`_ zip file includes daily migration.
Review the README files there for more information.

.. _EMOD scenarios: https://github.com/InstituteforDiseaseModeling/docs-emod-scenarios/releases


To simulate intra-day migration, such as daily commutes, you can instead use the "contact scaling"
method described in :doc:`model-hint`.
