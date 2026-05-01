# Run a simulation using mpiexec

The application mpiexec is used to run multi-node simulations in parallel. Eradication.exe is "single
threaded", so it uses only one core for processing. If you run a simulation with multiple geographic
nodes using mpiexec instead of invoking Eradication.exe directly, multiple copies of Eradication.exe will be
running, with one copy per core processing data for a single node at a time. Message Passing
Interface (MPI) communicates between the cores when handling the migration of individuals from one
**node** to another.

Although mpiexec can be used to run a simulation in parallel on your local computer, it is  more
often used to run complex simulations in parallel on an HPC cluster or several linked computers.
Mpiexec is part of the Microsoft HPC Pack 2019 SDK (64-bit) installed as a prerequisite for building Eradication.exe from the
EMOD source code. See [dev-install-overview](dev-install-overview.md) for more information.

!!! note

        If you get an error that the mpiexec command cannot be found, you must add the path to mpiexec to
        the PATH environment variable. For example, open Control Panel and add the path C:\\Program
        Files\\Microsoft HPC Pack 2012\\Bin to PATH.

## Usage

1. Take note of the number of cores on your computer or cluster.

    If running locally, we recommend running mpiexec with one fewer cores than are available, so one
    core is reserved for the operating system. The simulation can be run on all available cores and
    will complete faster, but the desktop will not be responsive.

1. Open a Command Prompt window and navigate to the directory that contains the configuration and
    campaign files for the simulation.

1. Invoke Eradication.exe using mpiexec as follows, replacing the number of cores, paths, and command options
    as necessary for your environment. See [software-simulation-cli](software-simulation-cli.md) for more information about
    the command options available for use with Eradication.exe.

```none

        mpiexec -n 3 ..\Eradication.exe --config config.json --input-path ..\InputDirectory\Garki --output-path OutputGarki
```

Mpiexec will start multiple copies of Eradication.exe as specified by `-n`. Those instances will
communicate with each other via MPI. If all cores are on a single computational node or host, they
will use internal networking to carry the MPI packets.

You can also link together several computers with MPI using the mpiexec `-host` option. For
example, if you were using six cores on two computers, you could run three copies of Eradication.exe
on the first computer, and three more could be run on the second computer. Again, this assumes that
each computer has at least three cores.

For more information about mpiexec, see MSDN_.
