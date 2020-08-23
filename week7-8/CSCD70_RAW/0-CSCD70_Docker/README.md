# Docker Image

This folder contains the Docker image which you could use to run experiments or
build examples in almost any OS. You can skip this section if you are using the
UG workstations since they already have LLVM installed.

## Introduction

Docker runs our applications in a **lightweight** *container* by using the
*image* that has all the necessary software components installed. Think of it as
`conda` or `virtualenv` (both are used to create an isolated Python
environment), but at a broader scope for almost all common software libraries.
Compared with the virtual machine solution, Docker has very low performance
overhead ("*The general result is that Docker is **nearly identical to Native
performance** and faster than KVM in every category*"
[[1]](https://stackoverflow.com/a/26149994/6320608)), making itself an ideal
solution for software development nowadays.

<p align="middle">
  <img width="32.9%" src="https://docs.docker.com/images/Container%402x.png">
  <img width="32.9%" src="https://docs.docker.com/images/VM%402x.png">
  <a href="https://docs.docker.com/get-started/">[2]</a>
</p>

## Instructions

- **Download and Install Docker**: 
  [Windows](https://hub.docker.com/editions/community/docker-ce-desktop-windows), 
  [Mac](https://hub.docker.com/editions/community/docker-ce-desktop-mac),
  [Ubuntu](https://docs.docker.com/install/linux/docker-ce/ubuntu/)

- **Build the docker image**:
  - An *image* is an isolated environment that includes all necessary software
    components.

```bash
# Build the docker image from file (-f) `cscd70.Dockerfile` and tag (-t) it
# with name `cscd70:6.0`.
docker build . -t cscd70:6.0 -f cscd70.Dockerfile
```

- **Create a container**:
  - A *container* is an instantiation of an image. Apparantly multiple
    containers can be created out of an image (it is similar to the conda where
    we have multiple Python projects using the same conda environment).

```bash
# Create a container for the first assignment.
cd ../Assignment1-Introduction_to_LLVM/FunctionInfo
docker run -it --rm -v $(pwd):/mnt --name CSCD70_A1 cscd70:6.0 

# Options:
#  -it   : Allocate a pseudo tty (-t) and connect STDIN (-i). These options must
#          be used for interactive processes.
# --rm   : (Optional) Cleanup the container when we exit.
# --name : (Optional) Name of the Container
#  -v    : Mount the `FunctionInfo` directory to /mnt in the container. This way
#          any changes made natively will be reflected in the container as well
#          (and vice versa).

# Note:
# - The image name (cscd70:6.0) should always come last.
# - Directory path should be absolute path (`$(pwd)`).
# - Additiaonlly, you can append commands to the image name. E.g.,
# 
#       cd /mnt && make -f Optimize.mk all
# 
#   Otherwise the default command `/bin/bash` will run.
```

- **Run the experiments**:

```bash
cd /mnt && make -f Optimize.mk all
```

- **You can also combine the first two steps**:

```bash
cd ../Assignment1-Introduction_to_LLVM/FunctionInfo
docker run -it --rm -v $(pwd):/mnt --name CSCD70_A1 -w /mnt cscd70:6.0 \
    make -f Optimize.mk all
# Pay attention to the additional `-w /mnt` (set working directory as `/mnt`)
# and the appended build command `make -f Optimize.mk all`.
```

### Docker Compose

Docker Compose is a simple wrapper on top of the docker commands. There is a
docker-compose file provided in the project root folder. We can do the same
things as we have previously described, but with much simpler commands.

```bash
# Install docker-compose via python-pip.
pip3 install docker-compose

# Build the LLVM docker image and Create a container.
docker-compose build cscd70-dev
docker-compose run --rm cscd70-dev
```
