# Getting started with building EMOD source code from CentOS Docker images

The EMOD source code can be built from the following CentOS Docker images available on JFrog Artifactory:

| Image name | Repository path | Description |
| --- | --- | --- |
| dtk-build | docker-production.packages.idmod.org/idm/centos:dtk-build | EMOD source code to build and run the EMOD executable (Eradication.exe). |
| dtk-runtime | docker-production.packages.idmod.org/idm/centos:dtk-runtime | EMOD files needed for running a pre-built CentOS EMOD executable (Eradication.exe). |
| dtk-sfts | docker-production.packages.idmod.org/idm/centos:dtk-sfts | EMOD source code to build test and run the EMOD executable (Eradication.exe). |

You can build these images on Windows and Linux machines. The following steps were tested and documented from machines running Linux for CentOS 7.7 and Windows 10.

## Prerequisites

* privileges to install packages (**sudo** on Linux, admin on Windows)
* An Internet connection
* Git client
* Docker client

To verify whether you have Git and Docker clients installed you can type the following at a command line prompt:

```
    git --version
    docker --version
```

## Download and install prerequisites

Git version 1.8.3.1 and Docker CE version 19.03.3 were used for creating this documentation.

To download and install a Git client, see https://git-scm.com/download.

To download and install a Docker client for CentOS, see https://docs.docker.com/install/linux/docker-ce/centos/
To download and install a Docker client for Windows, see https://docs.docker.com/docker-for-windows/install/

## Clone EMOD source code

To clone EMOD source code, type the following at a command line prompt:

```
    git clone https://github.com/InstituteForDiseaseModeling/EMOD.git
```

The next steps are to run the Docker container and build the EMOD source code from the CentOS Docker images. If your host machine is running Linux, see [dev-install-centos-docker-linux](dev-install-centos-docker-linux.md). If your host machine is running Windows, see [dev-install-centos-docker-win](dev-install-centos-docker-win.md).

