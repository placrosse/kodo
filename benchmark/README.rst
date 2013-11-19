Benchmarks
----------
This folder and it's sub-folders contain all necessary binaries and scripts to

* run and plot benchmarks of Kodo on your local machine
* plot benchmarks from Steinwurf's benchmark database

Scripts for plotting database results are located in their respective benchmark
folders. E.g., to see plots related to throughput, go to ``throughput`` folder
and execute either ``plot.py`` or ``plot_comparison.py``.

When you have successfully executed a plotting script, the resulting plots will be
generated in the folder called plot_database.

Installing with Pip
-------------------
Many ways exists to try out the plotting scripts. One is by using pip.
The following will guide you though such an installation.

First, if you haven't already, install the following requisites::

    sudo apt-get install python python-pip
    sudo apt-get build-dep python-scipy

Install the virtualenv wrapper::

    sudo pip install virtualenvwrapper

Add the virtualenvwrapper functions and variables to your path::

    printf export WORKON_HOME=$HOME/.virtualenvs\n >> ~/.bashrc
    printf source /usr/local/bin/virtualenvwrapper.sh\n >> ~/.bashrc
    source ~/.bashrc

Create a virtual environment::

    mkvirtualenv kodo_plot

mkvirtualenv will automatically activate the virtualenv. If you need to activate it at a later point you can use the following command::

    workon kodo_plot

For a full list of virtualenvwrappers function, click `here <http://virtualenvwrapper.readthedocs.org/en/latest/>`_.

To install the kodo_plot requirements, run the following command (while having the kodo_plot virtualenv activated)::

    pip install -Ur requirements.txt
