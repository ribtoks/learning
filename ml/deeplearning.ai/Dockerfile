FROM ubuntu:18.04

ARG ML_USER="ktar"

USER root

# Run a system update to get it up to speed
# Then install python3 and pip3
RUN apt-get update && apt-get install -yq --no-install-recommends \
    python3 \
    python3-pip \
    python3-setuptools \
    git \
    git-extras \
    wget \
    bzip2 \
    ca-certificates \
    sudo \
    nodejs \
    locales

RUN echo "en_US.UTF-8 UTF-8" > /etc/locale.gen && locale-gen

# Configure environment
ENV SHELL=/bin/bash \
    LC_ALL=en_US.UTF-8 \
    LANG=en_US.UTF-8 \
    LANGUAGE=en_US.UTF-8

# Install jupyter & co
RUN pip3 install wheel
RUN pip3 install jupyter -U && pip3 install jupyterlab
RUN pip3 install numpy scipy pandas sklearn matplotlib seaborn tensorflow

RUN adduser --disabled-password --gecos '' --uid 1000 $ML_USER
RUN echo "$ML_UESR ALL=(ALL) NOPASSWD:ALL" > "/etc/sudoers.d/$ML_USER"

# Fix permissions
RUN mkdir /notebooks && chown -R $ML_USER /notebooks

COPY .jupyter /home/$ML_USER/.jupyter
RUN chown -R $ML_USER /home/$ML_USER/.jupyter

USER $ML_USER
WORKDIR /home/$ML_USER

# Jupyter and Tensorboard ports
EXPOSE 8888 6006

VOLUME /notebooks

# Start the jupyter notebook with config from jupyter_notebook_config.py
ENTRYPOINT ["jupyter", "lab"]

#RUN sudo chown -R $ML_USER /home/$ML_USER/.local/share/jupyter
