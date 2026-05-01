# Building EMOD source code from CentOS Docker images on Windows host machine

These steps walk you through building the EMOD source code from CentOS Docker images on a Windows host. Before following these steps you must meet the prerequisites in [dev-install-centos-docker](dev-install-centos-docker.md). Although these steps are specific to the "dtk-build" image, you can use them for the other images by replacing "dtk-build" with the desired image name, such as "dtk-runtime" or "dtk-sfts".

## Download Docker image to Windows host machine

To download Docker image to your Windows host machine type the following at command line prompt:

```
    docker pull docker-production.packages.idmod.org/idm/centos:dtk-build
```

## Run Docker container from Windows host machine

To run Docker image from your Windows host machine type the following at command line prompt:

```
    docker run -it -v C:\EMOD:/EMOD docker-production.packages.idmod.org/idm/centos:dtk-build
```

To see additional information about the options used with the "docker run" command, type "docker run --help" at a command line prompt.

Upon completion you will then see a new command prompt for the Docker image, which in this example is as follows:

```
    [root@c23ffe9cb1fb /]#
```

To then build the EMOD executable, Eradication.exe, move to the /EMOD directory:

```
    cd EMOD
```

This directories contains the necessary build script and files.

## Build binary executable from Docker image running Linux CentOS 7.7 within Windows host machine

To build a binary executable you run the "scons" script. For more information about the build script options, type "scons --help" from within the EMOD directory. To specify a specific user name associated with the built binary executable, you can enter a specific user name by setting the "USER" environment variable that the build script uses. For example:

```
    USER=johndoe
```

In this example the Generic disease model is built. Type the following at command line prompt:

```
    scons --Disease=Generic
```

Upon completion you will see the following line at the end of the build process:

```
    scons: done building targets.
```

For more information, see [dev-install-build-emod](dev-install-build-emod.md).

## Verify EMOD binary executable

You can verify the successful build of the EMOD binary executable by typing the following at command line prompt:

```
    /EMOD/build/x64/Release/Eradication/Eradication --version
```

You should see a response similar to the following:

```
    Intellectual Ventures(R)/EMOD Disease Transmission Kernel 2.20.17.0
    Built on Oct 25 2019 by johndoe from master(2d8a9f2) checked in on 2019-04-05 15:49:43 -0700
    Supports sim_types: GENERIC.
```

You can then use this executable for running simulations. For more information, see [software-simulation-cli](../md_software/software-simulation-cli.md).
