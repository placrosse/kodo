"""
Copyright Steinwurf ApS 2011-2013.
Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
See accompanying file LICENSE.rst or
http://www.steinwurf.com/licensing

Various helper functions for database query, pandas data processing and
plotting
"""

import os
import scipy as sp
import argparse

import pymongo
assert pymongo.version_tuple[:2] >= (2,5), "You need a newer version of pymongo"
from pymongo import MongoClient

from matplotlib import pyplot as pl
from matplotlib.backends.backend_pdf import PdfPages as pp

import pandas as pd
assert sp.any(sp.array(pd.version.version.split(".")) >= ['1','12',sp.inf]), \
    "You need a newer version of pandas"

import datetime

def now():
    return datetime.datetime.utcnow()

def today():
    TODAY = now().date()
    return datetime.datetime(TODAY.year, TODAY.month, TODAY.day)

def yesterday():
    return today() - datetime.timedelta(1)

def timedelta(arg):
    return datetime.timedelta(arg)

def connect_database():
    """
    Connect to the benchmark database
    """

    address = "176.28.49.184"
    username = "guest"
    password = "none"

    client = MongoClient(address)
    db = client["benchmark"]
    db.authenticate(username, password)
    return db

def markers(label):
        if "Binary8" in label:
            return "v"
        if "Binary16" in label:
            return"^"
        if "Binary" in label:
            return "o"
        if "Prime2325" in label:
            return "*"

def set_legend():
    pl.legend(loc="upper left", fontsize="x-small", ncol=4, mode="expand")

def fields(string):
        if "Binary8" in string:
            return "$2^8$"
        if "Binary16" in string:
            return "$2^{16}$"
        if "Binary" in string:
            return "$2$"
        if "Prime2325" in string:
            return "$2^{32}-5$"

def algorithms(string):
        if "BackwardFullRLNC" in string:
            return "Backwards"
        if "FullDelayedRLNC" in string:
            return "Delayed"
        if "FullRLNC" in string:
            return "Standard"

def set_common_params():
        pl.rcParams.update(
            {'figure.autolayout' : True,
            'text.usetex' : True,
            })

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        pass

class plotter:
    """
    for convenient plotting
    """

    def __init__(self, args):
        self.args = args
        self.type = False
        self.branch = False
        pl.close('all')
        self.pdf = False

    def __del__(self):
        self.pdf.close()

    def get_dataframe(self, query, collection="none"):
        if self.args.json:
            df = pd.read_json(self.args.json)
            df['buildername'] = "local"
        else:
            db = connect_database()
            if collection == "kodo_throughput":
                mc = db.kodo_throughput.find(query)
            elif collection == "kodo_decoding_probability":
                mc = db.kodo_decoding_probability.find(query)
            elif collection == "kodo_overhead":
                mc = db.kodo_overhead.find(query)
            df = pd.DataFrame.from_records( list(mc) )

        return df

    def get_base_path(self):
        if hasattr(self.args, "json"):
            path = os.path.basename("local")
        else:
            path = os.path.basename("database")

        if hasattr(self.args, "coder"):
            path = os.path.join(path, self.args.coder)

        if self.branch:
            path = os.path.join(path, self.branch)

        mkdir_p(path)
        return path

    def get_full_path(self, filename):
        path = os.path.join(self.get_base_path(), self.type)
        mkdir_p(path)
        return os.path.join(path, filename + "." + self.args.output_format)

    def set_type(self, type):
        set_common_params()
        self.type = type

    def set_branch(self, branch):
        self.branch = branch.replace("-","_")

    def set_title(self, title):
        pl.title(title, ha = "left", position = (.0,1.03), fontsize = "medium")

    def set_markers(self, plot):
        """
        Set markers depending on the field
        @param plot a pylab plot
        """
        for l in plot.lines:
            field = l.get_label()
            l.set_marker(markers(field))

    def write(self, filename):
        set_legend()
        if not self.pdf:
            self.pdf = pp(os.path.join(self.get_base_path(), "all.pdf"))
        self.pdf.savefig(transparent=True)

        pl.savefig(self.get_full_path(filename))
        pl.close('all')

def add_arguments(argument_list):
    """
    add arguments to the runtime
    """

    parser = argparse.ArgumentParser()

    arguments = {
        "json" : add_argument_json,
        "coder" : add_argument_coder,
        "output-format" : add_argument_output_format,
        "date" : add_argument_date,
        "days" : add_argument_days,
        }

    for argument in argument_list:
        arguments[argument](parser)

    args = parser.parse_args()
    return args

def add_argument_json(parser):
    parser.add_argument(
    '--json', dest='json', action='store',
    help='the .json file written by gauge benchmark, if non provided\
    plots from the database', default="")

def add_argument_coder(parser):
    parser.add_argument(
    '--coder', dest='coder', action='store', choices=['encoder','decoder'],
    default='decoder',
    help='Whether to consider the encoding or decoding performance')

def add_argument_output_format(parser):
    parser.add_argument(
    '--output-format', dest='output_format', action='store', default='eps',
    help='The format of the generated figures, e.g. eps, pdf')

def add_argument_date(parser):
    parser.add_argument(
    '--date', dest='date', type=int, action='store', default=today(),
    help='What data to use when accessing the database')

def add_argument_days(parser):
    parser.add_argument(
    '--days', dest='days', type=int, action='store', default=3,
    help='How many days to look back in time when comparing')
