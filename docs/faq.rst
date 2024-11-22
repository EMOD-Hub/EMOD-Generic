===========================
Frequently asked questions
===========================


Why doesn't anything happen when I double-click |exe_s|?
    Unlike many executable files, |exe_s| does not open a software installer when double-clicked.
    Instead, you must provide |exe_s| with various input files used to run a simulation and create
    outputs. You can run simulations in a number of ways. The simplest way to learn about the model
    is to run individual simulations at the command line. However, because |EMOD_s| is a stochastic
    model, this is not feasible when you must run many simulations to create useful output for
    statistical analysis. Once you advance to that stage, you will want to use other software tools
    to run multiple simulations either locally or, more often, on an HPC cluster. For more
    information, see :doc:`install-overview` and :doc:`software-run-simulation`.


Where do I get demographics and climate files? 
    |IDM_s| provides demographics and climate files for many different regions of the world in the
    EMOD-InputData GitHub repository. See :doc:`install-overview` for download instructions. If
    there are no files available for the region you are interested in, contact support@idmod.org.

How do I plot my output? 
    |EMOD_s| does not produce plots of the data by default. Instead, it produces 
    :term:`JSON (JavaScript Object Notation)` or CSV files that you can then parse and plot using
    Python, MATLAB, R, or another programming language. The `EMOD scenarios`_ zip file contains input 
    files and plotting scripts for many different disease modeling scenarios intended to teach 
    |EMOD_s|. You may want to review these files to see examples of how the output can be plotted.

Does your model include X?
    Many aspects of disease dynamics are explicitly modeled in |EMOD_s|. Review :doc:`parameter-overview` 
    for an exhaustive list of all parameters that can be used to enable functionality in
    |EMOD_s|. If you want to model something that isn't listed there, |EMOD_s| provides highly
    flexible functionality with the individual and node property feature. These properties label
    individuals or regional nodes in a simulation so you can change disease outbreaks, transmission
    dynamics, behavior, or treatment based on the assigned values. For example, you can add
    properties to represent vitamin deficiencies, comorbidities, and more. For more information, see
    :doc:`model-properties`.

    If individual and node properties cannot incorporate the functionality you need, |EMOD_s| is
    open-source software that can be extended to meet your needs. For more information, see 
    :doc:`dev-install-overview`.

Can I model regions, countries, provinces, or cities? 
    |EMOD_s| uses nodes to represent geographic areas. Each :term:`node` has a population and
    associated characteristics. Individuals and, if relevant, vectors can migrate between nodes.
    Nodes can represent areas of any scale you like, from entire countries to individual
    households. 

Do you have a model for disease X?
    The software architecture of |EMOD_s| has a "generic model" base upon which more specific
    transmission modes and diseases are built. This allows common functionality to be shared for
    modeling a variety of diseases. For example, the generic model can be used to model influenza,
    measles, and more. You can also use one of the transmission mode simulation types (airborne,
    STI, environmental, vector) to model diseases such as herpes, typhoid, dengue or others that we
    don't explicitly support. Often, this can be done without the need to make any source code
    changes.


.. _EMOD scenarios: https://github.com/InstituteforDiseaseModeling/docs-emod-scenarios/releases
