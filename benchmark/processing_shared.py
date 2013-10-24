"""
Copyright Steinwurf ApS 2011-2013.
Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
See accompanying file LICENSE.rst or
http://www.steinwurf.com/licensing

Various shared functionality for database query, pandas data processing and plotting
"""

import os
import scipy as sp

import pymongo
assert pymongo.version_tuple[:2] >= (2,5), "You need a newer version of pymongo"
from pymongo import MongoClient

import pandas as pd
assert sp.any(sp.array(pd.version.version.split(".")) >= ['1','12',sp.inf]),  "You need a newer version of pandas"

address = "176.28.49.184"
username = "guest"
password = "none"

def connect_database():
    """
    Connect to the benchmark database
    """
    client = MongoClient(address)
    db = client["benchmark"]
    db.authenticate(username, password)
    return db

def query_database(query):
    """
    Query the database
    @param query a dict containing the query
    """
    
    assert( isinstance(query, dict) )
    
    db = connect_database()
    assert( isinstance(db, pymongo.database.Database) )

    mongo_cursor = db.kodo_throughput.find(query)
    return mongo_cursor

def calculate(df):
    """
    Calculate mean and variance on a dataframe
    @param df a pandas dataframe
    """

    df['mean'] = df['throughput'].apply(sp.mean)
    df['std'] = df['throughput'].apply(sp.std)

def set_markers(plot):
    """
    Set markers depending on the field 
    @param plot a pylab plot
    """
    for l in plot.lines:
        field = l.get_label()
        if "Binary" in field:
            l.set_marker("o")
        if "Binary8" in field:
            l.set_marker("v")
        if "Binary16" in field:
            l.set_marker("^")
        if "Prime2325" in field:
            l.set_marker("*")

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        pass

from matplotlib import pyplot as pl

pl.rcParams['legend.frameon'] = False
pl.rcParams['legend.loc'] = 'center right'

def plot_sparse_group(group, y_value="mean", title=""):
    """
    Plots a sparse group of benchmark results group 
    @param group is the pandas groupby dataframe
    @ param y_value is the value to plot on the y-axis as a string
    @title the title for the figure
    """

    pl.rcParams.update(
    {'figure.subplot.right': .7 ,
    'figure.subplot.left': .1
    })

    p = group.pivot_table(y_value,  rows='symbols', cols=['benchmark','average_nonzero_symbols']).plot()
    set_markers(p)
    p.set_title(title, ha = "left", position = (.0,1.03), fontsize = "medium")
    pl.xticks(list(sp.unique(group['symbols'])))
    pl.ylabel("Throughput [" + list(group['unit'])[0] + "]")
    pl.legend(bbox_to_anchor=(1., -0.01), loc=3, ncol=1)
    return p


def plot_dense_group(group, y_value="mean", title=""):
    """
    Plots a dense group of benchmark results group
    @param group is the pandas groupby dataframe
    @ param y_value is the value to plot on the y-axis as a string
    @title the title for the figure
    """

    pl.rcParams.update(
        {'figure.subplot.right': .48 ,
        'figure.subplot.left': .1
        })

    p = group.pivot_table(y_value,  rows='symbols', cols=['benchmark','testcase']).plot()
    set_markers(p)
    p.set_title(title, ha = "left", position = (.0,1.03), fontsize = "medium")
    pl.xticks(list(sp.unique(group['symbols'])))
    pl.ylabel("Throughput [" + list(group['unit'])[0] + "]")
    pl.legend(bbox_to_anchor=(1., -.01), loc=3, ncol=1)
    return p
