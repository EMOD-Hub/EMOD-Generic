========
Glossary
========

.. I think it's preferable to define terms as "long form (acronym)" rather than just the acronym.
.. We don't want to link to the glossary entry after the first use anyway. Additionally, it's better
.. to use a singular term in the glossary, unless that term is almost always used in its plural form.


.. contents:: Contents
   :local:


.. glossary::
    :sorted: 

    antibodies
        See :term:`antibody`.

    antibody
        A blood protein produced in response to and counteracting a specific antigen. Antibodies combine
        chemically with substances that the body recognizes as foreign (eg. bacteria, viruses, or other
        substances).

    antigen
        A substance that is capable of inducing a specific immune response and that evokes the
        production of one or more antibodies.

    antigens
        See antigen.

    Clausius-Clayperon relation
        A way of characterizing a transition between two phases of matter; provides a method to find
        a relationship between temperature and pressure along phase boundaries. Frequently used in
        meteorology and climatology to describe the behavior of water vapor. See `Wikipedia -
        Clausius-Clayperon relation <https://en.wikipedia.org/wiki/Clausius%E2%80%93Clapeyron_relation>`__
        for more information.

    compartmental model
        A disease model that divides the population into a number of compartments that represent
        different disease states, such as susceptible, infected, or recovered. Every person in a
        compartment is considered identical. Many compartmental models are :term:`deterministic`,
        but some are :term:`stochastic`.

    deterministic
        Characterized by the output being fully determined by the parameter values and the initial
        conditions. Given the same inputs, a deterministic model will always produce the same output.

    disability-adjusted life years (DALY)
        The number of years of life lost due to premature mortality
        plus the years lost due to disability while infected. Used to quantify the burden of disease.

    diffusive migration
        The diffusion of people in and out of nearby nodes by foot travel.

    epidemic
        An outbreak of an infectious disease, such that a greater number of individuals than normal
        has the disease. Epidemics have very high R\ :sub:`0`\  (Recall R\ :sub:`0`\ >1 for a disease to spread) and are often associated with acute, highly transmissible pathogens that can be directly transmitted. Further, pathogens with lower infectious periods create more explosive epidemics. To control epidemics, it is necessary to reduce R\ :sub:`0`\. This can be done by:

        * Reducing transmissibility.
        * Decreasing the number of susceptibles (by vaccination, for example).
        * Decreasing the mean number of contacts or the transmissibility, such as by improving
          sanitation, or limiting the number of interactions sick people have with healthy people.
        * Reducing the length of the infectious period.

    epitope
        The portion of an antigen that the immune system recognizes. An epitope is also called an
        antigenic determinant.

    Euler method
        Used in mathematics and computational science, this method is a first-order numerical
        procedure for solving ordinary differential equations with a given initial value.

    exp(
        The exponential function, :math:`e^x`, where :math:`e` is the number (approximately 2.718281828)
        such that the function :math:`e^x` is its own derivative. The exponential function is used to
        model a relationship in which a constant change in the independent variable gives the same
        proportional change (i.e. percentage increase or decrease) in the dependent variable. The
        function is often written as :math:`exp(x)`. The graph of :math:`y = exp(x)` is upward-sloping
        and increases faster as :math:`x` increases.

    exposed
        Individual who has been infected with a pathogen, but due to the pathogen’s incubation
        period, is not yet infectious.

    force of infection (FoI)
        A measure of the degree to which an infected individual can spread infection;
        the per-capita rate at which susceptibles contract infection. Typically increases with
        transmissibility and prevalence of infection.

    herd immunity
        The resistance to the spread of a contagious disease within a population that results if a
        sufficiently high proportion of individuals are immune to the disease, especially through
        vaccination. The portion of the population that needs to be immunized in
        order to achieve herd immunity is  P > 1 – (1/ R\ :sub:`0`\), where P = proportion
        vaccinated * vaccine efficacy.

    immune
        Unable to become infected/infectious, whether through vaccination or having the disease in the
        past.

    incidence
        The rate of new cases of a disease during a specified time period. This is a measure of
        the risk of contracting a disease.

    infectious
        Individual who is infected with a pathogen and is capable of transmitting the pathogen to others.

    Koppen-Geiger Climate Classification System
        A system based on the concept that native vegetation is a good expression of climate. Thus,
        climate zone boundaries have been selected with vegetation distribution in mind. It combines
        average annual and monthly temperatures and precipitation, and the seasonality of
        precipitation. |EMOD_s| has several options for configuring the climate, namely air
        temperature, rainfall, and humidity.

        One option utilizes input files that associate
        geographic nodes with Koppen climate indices. The modified Koppen classification uses three
        letters to divide the world into five major climate regions (A, B, C, D, and E) based on
        average annual precipitation, average monthly precipitation, and average monthly
        temperature. Each category is further divided into sub-categories based on temperature and
        precipitation. While the Koppen system does not take such things as temperature extremes,
        average cloud cover, number of days with sunshine, or wind into account, it is a good
        representation of our earth's climate.

    loss to follow-up (LTFU)
        Patients who at one point were actively participating in disease treatment or clinical
        research, but have become lost either by error or by becoming unreachable at the point of
        follow-up.

    LTFU
        See loss to follow-up.

    ordinary differential equation (ODE)
        A differential equation containing one or more functions of one independent variable and
        its derivatives.

    prevalence
        The rate of all cases of a disease during a specified time period. This is a measure of how
        widespread a disease is.

    recovered
        Individual who is either no longer infectious, or “removed” from the population.

    reproductive number
        In a fully susceptible population, the basic reproductive number R\ :sub:`0` \ is the number of
        secondary infections generated by the first infectious individual over the course of the infectious period. R\ :sub:`0`\=S*L* :math:`\beta` (where S = the number of susceptible hosts, L = length of infection, and :math:`\beta` = transmissibility). When R\ :sub:`0`\ > 1,
        disease will spread. It is essentially a measure of the expected or average outcome of transmission. The effective reproductive number takes into account non-susceptible individuals. This is the threshold parameter used to determine whether or not an epidemic will occur, and determines:

            * The initial rate of increase of an epidemic (the exponential growth phase).
            * The final size of an epidemic (what fraction of susceptibles will be infected).
            * The endemic equilibrium fraction of susceptibles in a population (1/ R\ :sub:`0`\).
            * The critical vaccination threshold, which is equal to 1-(1/ R\ :sub:`0`\), and
              determines the number of people that must be vaccinated to prevent the spread of a
              pathogen.

    routine immunization (RI)
        The standard practice of vaccinating the majority of susceptible people in a population against
        vaccine-preventable diseases.

    stochastic
        Characterized by having a random probability distribution that may be analyzed statistically
        but not predicted precisely.

    stochastic die-out
        When an disease outbreak ends, despite having an effective R\ :sub:`0` above 1, due to
        randomness. A :term:`deterministic` model cannot estimate the probability of stochastic die-
        out, but a stochastic model can.

    subpatent
        When an individual is infected but asymptomatic, so the infection is not readily detectable.

    SEIR model
        A generic epidemiological model that provides a simplified means of describing the transmission
        of an infectious disease through individuals where those individuals can pass through the
        following five states: susceptible, exposed, infectious, and recovered.

    SEIRS model
        A generic epidemiological model that provides a simplified means of describing the transmission
        of an infectious disease through individuals where those individuals can pass through the
        following five states: susceptible, exposed, infectious, recovered, and susceptible.

    SI model
        A generic epidemiological model that provides a simplified means of describing the transmission
        of an infectious disease through individuals where those individuals can pass through the
        following five states: susceptible and infectious.

    simulation burn-in
        A modeling concept in which a simulation runs for a period of time before reaching a steady
        state and the output during that period is not used for predictions. This concept is
        borrowed from the electronics industry where the first items produced by a manufacturing
        process are discarded.

    SIR model
        A generic epidemiological model that provides a simplified means of describing the transmission
        of an infectious disease through individuals where those individuals can pass through the
        following five states: susceptible, infectious, and recovered.

    SIRS model
        A generic epidemiological model that provides a simplified means of describing the transmission
        of an infectious disease through individuals where those individuals can pass through the
        following five states: susceptible, infectious, recovered, and susceptible.

    SIS model
        A generic epidemiological model that provides a simplified means of describing the transmission
        of an infectious disease through individuals where those individuals can pass through the
        following five states: susceptible, infectious, and susceptible.

    superinfection
        The simultaneous infection with multiple strains of the same pathogen.

    supplemental immunization activity (SIA)
        In contrast to :term:`routine immunization (RI)`, SIAs are large-scale operations with a
        goal of delivering vaccines to every household.

    susceptible
        Individual who is able to become infected.

    transmissibility (:math:`\beta`)
        Also known as the effective contact rate, is the product of the contact rate and the
        probability of transmission per contact.

    virulence
        The capacity of a pathogen to produce disease. It is proportional to parasitemia, or the
        number of circulating copies of the pathogen in the host. The higher the virulence (given
        contact between S and I individuals), the more likely transmission is to occur. However,
        higher virulence means contact may be less likely as infected hosts show more symptoms of
        the disease. There is a trade-off that occurs between high transmissibility and disease-
        induced mortality.

    WAIFW matrix
        A matrix of values that describes the rate of transmission between different population
        groups. WAIFW is an abbreviation for Who Acquires Infection From Whom.

    Weibull distribution
        A probability distribution often used in |EMOD_s| and that requires both a shape parameter
        and a scale parameter. The shape parameter governs the shape of the density function. When
        the shape parameter is equal to 1, it is an exponential distribution. For shape parameters
        above 1, it forms a unimodal (hump-shaped) density function. As the shape parameter becomes
        large, the function forms a sharp peak. The inverse of the shape parameter is sometimes
        referred to here as the “heterogeneity” of the distribution (heterogeneity = 1/shape),
        because it can be helpful to think about the degree of heterogeneity of draws from the
        distribution, especially for hump-shaped functions with heterogeneity values between 0 and 1
        (i.e., shape parameters greater than 1). The scale parameter shifts the distribution from
        left to right. When heterogeneity is small (i.e., the shape parameter is large), the scale
        parameter sets the location of the sharp peak.

    agent-based model
        A type of simulation that models the actions and interactions of autonomous agents
        (both individual and collective entities such as organizations or groups).

    Boost
        Free, peer-reviewed, portable C++ source libraries aimed at a wide range of uses including
        parallel processing applications (Boost.MPI). For more information, please see the Boost
        website, http://www.boost.org.

    boxcar function
        A mathematical function that is equal to zero over the entire real line except for a single
        interval where it is equal to a constant.

    campaign
        A collection of events that use interventions to modify a :term:`simulation`.

    campaign event
        A JSON object that determines when and where an intervention is distributed during a campaign.

    campaign file
        A :term:`JSON (JavaScript Object Notation)` formatted file that contains the parameters that
        specify the distribution instructions for all interventions used in a campaign, such as
        diagnostic tests, the target demographic, and the timing and cost of interventions. The
        location of this file is specified in the :term:`configuration file` with the
        **Campaign_Filename** parameter. Typically, the file name is campaign.json.

    channel
        A property of the simulation (for example, "Parasite Prevalence") that is accumulated once per
        simulated :term:`time step` and written to file, typically as an array of the accumulated values.

    class factory
        A function that instantiate objects at run-time and use information
        from JSON-based configuration information in the creation of these objects.

    configuration file
        A :term:`JSON (JavaScript Object Notation)` formatted file that contains the parameters
        sufficient for initiating a simulation. It controls many different aspects of the
        simulation, such as population size, disease dynamics, and length of the simulation.
        Typically, the file name is config.json.

    core
        In computing, a core refers to an independent central processing unit (CPU) in the computer.
        Multi-core computers have more than one CPU. However, through technologies such as Hyper-
        Threading Technology (HTT or HT), a single physical core can actually act like two virtual
        or logical cores, and appear to the operating system as two processors.

    demographics file
        A :term:`JSON (JavaScript Object Notation)` formatted file that contains the parameters that
        specify the demographics of a population, such as age distribution, risk, birthrate, and more.
        |IDM_s| provides demographics files for many geographic regions. This file is typically named <region>_demographics.json.

    disease-specific build
        A build of the |exe_l| built using SCons without any dynamic link libraries (DLLs).

    dynamic link library (DLL)
        Microsoft's implementation of a shared library, separate from the |exe_l|, that can be
        dynamically loaded (and unloaded when unneeded) at runtime. This loading can be explicit or
        implicit.

    EMODule
        A modular component of |EMOD_s| that are consumed and used by the |exe_l|.
        Under Windows, a |module| is implemented as a :term:`dynamic link library (DLL)` and,
        under |Centos|, |modules| are currently not supported. |modules| are primarily custom reporters.

    Epidemiological MODeling software (EMOD)
        The modeling software from the |IDM_l| for disease researchers and developers to investigate
        disease dynamics, and to assist in combating a host of infectious diseases. You may see
        this referred to as Disease Transmission Kernel (DTK) in the source code.

    Eradication.exe
        Typical (default) name for the |exe_l|, whether built using monolithic build or
        modular (|module|-enabled) build.

    event coordinator
        A JSON object that determines who will receive a particular intervention during a campaign.

    flattened file
        A single campaign or configuration file created by combining a default file with one or more
        overlay files. Multiple files must be flattened prior to running a simulation. Configuration
        files are flattened to a single-depth JSON file without nesting, the format required for
        consumption by the |exe_l|. Separating the parameters into multiple files is primarily
        used for testing and experimentation.

    Heterogeneous Intra-Node Transmission (HINT)
         A feature for modeling person-to-person transmission of diseases in heterogeneous population
         segments within a node for generic simulations.

    high-performance computing (HPC)
        The use of parallel processing for running advanced applications efficiently, reliably,
        and quickly.

    individual properties
        Labels that can be applied to individuals within a simulation and used to configure
        heterogeneous transmission, target interventions, and move individuals through a health care
        cascade.

    input files
        The JSON and binary files used as inputs to an |EMOD_s| simulation. The primary input files
        are the JSON-formatted configuration, demographics, and campaign files. They may also
        include the binary files for migration, climate, population serialization, or load-
        balancing.

    inset chart
        The default JSON output report for |EMOD_s| that includes multiple channels that contain
        data at each time step of the simulation. These channels include number of new infections,
        prevalence, number of recovered, and more.

    intervention
        An object aimed at reducing the spread of a disease that is distributed either to an
        individual; such as a vaccine, drug, or bednet; or to a node; such as a larvicide. Additionally,
        initial disease outbreaks and intermediate interventions that schedule another intervention
        are implemented as interventions in the :term:`campaign file`.

    JSON (JavaScript Object Notation)
        A human-readable, open standard, text-based file format for data interchange. It is
        typically used to represent simple data structures and associative arrays, and is
        language-independent. For more information, see https://www.json.org.

    JSON
        See JavaSCript Object Notation.

    Keyhole Markup Language (KML)
        A file format used to display geographic data in an Earth browser, for example, Google Maps.
        The format uses an XML-based structure (tag-based structure with nested elements and
        attributes). Tags are case-sensitive.

    Link-Time Code Generation (LTCG)
        A method for the linker to optimize code (for size and/or speed) after compilation has
        occurred. The compiled code is turned not into actual code, but instead into an intermediate
        language form (IL, but not to be confused with .NET IL which has a different purpose). The
        LTCG then, unlike the compiler, can see the whole body of code in all object files and be
        able to optimize the result more effectively.

    Message Passing Interface (MPI)
        An interface used to pass information between computing cores in parallel simulations. One
        example is the migration of individuals from one geographic location to another within |EMOD_s|
        simulations.

    microsolver
        A type of "miniature model" that operates within the framework of |EMOD_s|
        to compute a particular set of parameters. Each microsolver, in effect, is creating a
        microsimulation in order to accurately capture the dynamics of that particular aspect of the
        model.

    Monte Carlo method
        A class of algorithms using repeated random sampling to obtain numerical results. Monte
        Carlo simulations create probability distributions for possible outcomes, which provides a
        more realistic way of describing uncertainty.

    monolithic build
        A single |exe_l| with no DLLs that includes all components as part of |exe_s| itself. You
        can still use |modules| with the monolithic build; for example, a
        custom reporter is a common type of |module|. View the documentation on |modules| and
        emodules_map.json for more information about creation and use of |modules|.

    node
        A grid size that is used for modeling geographies. Within |EMOD_s|, a node is a geographic
        region containing simulated individuals. Individuals migrate between nodes either
        temporarily or permanently using mobility patterns driven by local, regional, and long-
        distance transportation.

    node properties
        Labels that can be applied to nodes within a simulation and used to target interventions based on geography.

    node-targeted intervention
        An intervention that is distributed to a geographical node rather than to a single
        individual. One example is larvicides, which affect all mosquitoes living and feeding within
        a given node.

    nodes
        See :term:`node`.

    output report
        A file that is the output from an |EMOD_s| simulation. Output reports are in JSON, CSV, or binary
        file format. You must pass the data from an output report to graphing software if you want to
        visualize the output of a simulation.

    overlay file
        An additional configuration, campaign, or demographic file that overrides the default
        parameter values in the primary file. Separating the parameters into multiple files is
        primarily used for testing a nd experimentation. In the case of configuration and campaign
        files, the files can use an arbitrary hierarchical structure to organize parameters into
        logical groups. Configuration and campaign files must be flattened into a single file before
        running a simulation.

    preview
        Software that undergoes a shorter testing cycle in order to make it available
        more quickly. Previews may contain software defects that could result in unexpected
        behavior. Use |EMOD_s| previews at your own discretion.

    regression test
        A test to verify that existing |EMOD_s| functionality works with new
        updates, located in the Regression subdirectory of the |EMOD_s| source code repository. Directory names of each
        subdirectory  in Regression describe the main regression attributes, for example,
        "1_Generic_Seattle_MultiNode". Also can refer to the process of regression testing of
        software.

    release
        Software that includes new functionality, scientific tutorials leveraging new or existing
        functionality, and/or bug fixes that have been thoroughly tested so that any defects have
        been fixed before release. |EMOD_s| releases undergo full regression testing.

    reporter
        Functionality that extracts simulation data, aggregates it, and saves it as an
        :term:`output report`. |EMOD_s| provides several built-in reporters for outputting data from
        simulations and you also have the ability to create a custom reporter.

    scenario
        A collection of input files that describes a real-world example of a disease outbreak and
        interventions. Many scenarios are included with |EMOD_s| source installations or are
        available to download at `EMOD scenarios`_ to learn more about epidemiology and disease
        modeling.

    schema
        A text or JSON file that can be generated from the |exe_l| that defines all
        configuration and campaign parameters.

    simulation
        An execution of the |EMOD_s| software using an associated set of input files.

    simulation type
        The disease or disease class to model.

        .. include:: reuse/sim-types.txt

    solvers
        Solvers are used to find computational solutions to problems. In simulations, they can be used,
        for example, to determine the time of the next simulation step, or to compute the states of
        a model at particular time steps.

    Standard Template Library (STL)
        A library that contains a set of common C++ classes (including generic algorithms and data
        structures) that are independent of container and implemented as templates, which enables
        compile-time polymorphism (often more efficient than run-time polymorphism). For more
        information and discussion of STL, see `Wikipedia -
        Standard Template Library <https://en.wikipedia.org/wiki/Standard_Template_Library>`__
        for more information.

    state transition event
        A change in state (e.g. healthy to infected, undiagnosed to positive diagnosis, or birth)
        that may trigger a subsequent action, often an intervention. "Campaign events" should not be
        confused with state transition events.

    time step
        A discrete number of hours or days in which the "simulation states" of all "simulation
        objects" (interventions, infections, immune systems, or individuals) are updated in a
        simulation. Each time step will complete processing before launching the next one. For
        example, a time step would process the migration data for populations moving between nodes
        via rail, airline, and road. The migration of individuals between nodes is the last step of
        the time step after updating states.

    tutorial
        A set of instructions in the documentation to learn more about epidemiology and
        disease modeling. Tutorials are based on real-world scenarios and demonstrate the mechanics
        of the the model. Each tutorial consists of one or more scenarios.

    working directory
        The directory that contains the configuration and campaign files for a simulation. You must
        be in this directory when you invoke |exe_s| at the command line to run a simulation.

.. _EMOD scenarios: https://github.com/InstituteforDiseaseModeling/docs-emod-scenarios/releases
