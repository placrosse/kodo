Benchmarks
----------
This folder and it's sub-folders contain all necessary binaries and scripts to

* Run and plot benchmarks of Kodo on your local machine
* Plot benchmarks from Steinwurf's benchmark database

Scripts for plotting database results are located in their respective benchmark
folders. E.g., to see plots related to throughput, go to ``throughput`` folder
and execute either ``plot.py`` or ``plot_comparison.py``.

When you have successfully executed a plotting script, the resulting plots will
be generated in the folder specified using the ``--output-dir`` or in a default
folder called plot_database.

Installing with Pip
-------------------
By using pip and virtualenv the plots dependencies can easily be handled.
Please note, the installation instructions found here has only been tested on
Ubuntu/Debian.

First, if you haven't already, install the prerequisites::

    sudo apt-get update
    sudo apt-get install python python-pip libfreetype6-dev libpng12-dev
    sudo apt-get build-dep python-scipy

Install the virtualenv wrapper::

    sudo pip install virtualenvwrapper

Add the virtualenvwrapper functions and variables to your path::

    printf "export WORKON_HOME=$HOME/.virtualenvs\n" >> ~/.bashrc
    printf "source /usr/local/bin/virtualenvwrapper.sh\n" >> ~/.bashrc
    source ~/.bashrc

Create a virtual environment::

    mkvirtualenv kodo_plot

mkvirtualenv will automatically activate the virtualenv. If you need to activate
it again at a later point you can use the following command::

    workon kodo_plot

For more information on virtualenvwrappers, click
`here <http://virtualenvwrapper.readthedocs.org/en/latest/>`_.

To install the requirements, run the following command and grab a cup of coffee
(while having the kodo_plot virtualenv activated)::

    pip install -Ur requirements.txt
