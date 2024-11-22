================================================
Welcome to |EMOD_s| modeling for general disease
================================================


The |IDM_l| develops disease modeling software that is thoroughly tested and shared with the
research community to advance the understanding of disease dynamics. This software helps determine
the combination of health policies and intervention strategies that can lead to disease eradication.
If you encounter any issues while using the software, please contact idm@gatesfoundation.org.

|EMOD_s|, is an :term:`agent-based model` (ABM) that simulates the
simultaneous interactions of agents in an effort to recreate complex phenomena.  Each agent (such as
a human or vector) can be assigned a variety of "properties" (for example, age, gender, etc.), and
their behavior and interactions with one another are determined by using decision rules. These
models have strong predictive power and are able to leverage spatial and temporal dynamics.

|EMOD_s| is also :term:`stochastic`, meaning that there is randomness built into the model. Infection
and recovery processes are represented as probabilistic Bernoulli random draws. In other words, when
a susceptible person comes into contact with a pathogen, they are not guaranteed to become infected.
Instead, you can imagine flipping a coin that has a λ chance of coming up tails S(t) times, and for
every person who gets a "head" you say they are infected. This randomness better approximates what
happens in reality. It also means that you must run many simulations to determine the probability of particular outcomes. 

The |EMOD_s| documentation is broken up into disease-specific sets that provide
guidance for researchers modeling particular diseases. The documentation contains only the
parameters, output reports, and other components of the model that are available to use for malaria modeling.

For example, this documentation set includes installation and usage instructions for the Generic model, which can be used to simulate a variety of different low-complexity diseases, such as measles.

.. toctree::
   :maxdepth: 3
   :titlesonly:
   :caption: Using the model

   whats-new
   install-overview
   software-overview
   tutorials
   troubleshooting

.. toctree::
   :maxdepth: 3
   :titlesonly:
   :caption: Understanding the model

   model-overview
   parameter-overview
   faq
   glossary

.. toctree::
   :maxdepth: 3
   :titlesonly:
   :caption: Advancing the model

   dev-install-overview
   dev-architecture-overview
   dev-debugging-overview
   
.. toctree::
   :maxdepth: 3
   :titlesonly:
   :caption: Related documentation

   idmtools <https://docs.idmod.org/projects/idmtools/en/latest/>
   idmtools-calibra <https://docs.idmod.org/projects/idmtools_calibra/en/latest/>
   emodpy-generic <https://docs.idmod.org/projects/emodpy-generic/en/latest/>
   emodpy <https://docs.idmod.org/projects/emodpy/en/latest/>
   emod-api <https://docs.idmod.org/projects/emod-api/en/latest/>
   EMOD developer docs <https://docs.idmod.org/projects/emod/en/latest/>
