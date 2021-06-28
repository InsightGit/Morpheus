# Modified version of devkitPro's docker containers

FROM ubuntu:20.04

MAINTAINER Bobby Youstra <bobbyyoustra@protonmail.com>

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y --no-install-recommends python3 python3-pip python3-pil gpg curl pkg-config ca-certificates  \
    cmake zip unzip build-essential doxygen wget sudo libraqm-dev

RUN pip3 install myst-parser breathe sphinx==3.5.4 sphinx-rtd-theme

RUN wget https://github.com/devkitPro/pacman/releases/latest/download/devkitpro-pacman.amd64.deb && \
    dpkg -i devkitpro-pacman.amd64.deb && \
    rm devkitpro-pacman.amd64.deb && \
    dkp-pacman -Scc --noconfirm

ENV DEVKITPRO=/opt/devkitpro
ENV PATH=${DEVKITPRO}/tools/bin:$PATH

RUN dkp-pacman -Syyu --noconfirm nds-dev gba-dev nds-portlibs

ENV DEVKITARM=${DEVKITPRO}/devkitARM
