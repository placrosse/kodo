"""
Copyright Steinwurf ApS 2011-2013.
Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
See accompanying file LICENSE.rst or
http://www.steinwurf.com/licensing

Plot the number of extra symbols needed to decode
"""

import argparse
import pandas as pd
import scipy as sp

import sys
sys.path.insert(0, "../")
import processing_shared as ps

from datetime import datetime, timedelta
now = datetime.utcnow()
today = now.date()
today = datetime(today.year, today.month, today.day)
yesterday = today - timedelta(1)

def plot(args):

    if args.jsonfile:
        PATH  = ("figures_local/")
        df = pd.read_json(args.jsonfile)
        df['buildername'] = "local"

    else:
        PATH  = ("figures_database/")
        query = {
        "branch" : "master",
        "scheduler": "kodo-nightly-benchmark",
        "utc_date" : {"$gte": yesterday, "$lt": today}
        }

        db = ps.connect_database()
        mc = db.kodo_decoding_probability.find(query)
        df = pd.DataFrame.from_records( list(mc) )

    df['mean'] = df['used'].apply(sp.mean) -df['symbols']
    df['std'] = df['used'].apply(sp.std)

    sparse = df[df['testcase'] == "SparseFullRLNC"].groupby(by= ['buildername', 
        'symbol_size'])
    dense = df[df['testcase'] != "SparseFullRLNC"].groupby(by= ['buildername', 
        'symbol_size'])

    from matplotlib import pyplot as pl
    from matplotlib.backends.backend_pdf import PdfPages as pp
    pl.close('all')

    ps.mkdir_p(PATH + "sparse")
    ps.mkdir_p(PATH + "dense")
    pdf = pp(PATH + "all.pdf")

    for (buildername,symbols), group in sparse:
        ps.set_sparse_plot()
        p = group.pivot_table('mean',  rows='symbols', cols=['benchmark',
            'density']).plot()
        ps.set_plot_details(p, buildername)
        pl.ylabel("Extra symbols" + " [" + list(group['unit'])[0] + "]")
        pl.xticks(list(sp.unique(group['symbols'])))
        pl.savefig(PATH + "sparse/" + buildername + '.eps')
        pdf.savefig(transparent=True)

    for (buildername,symbols), group in dense:
        ps.set_dense_plot()
        p = group.pivot_table('mean',  rows='symbols', cols=['benchmark',
            'testcase']).plot()
        ps.set_plot_details(p, buildername)
        pl.ylabel("Extra symbols" + " [" + list(group['unit'])[0] + "]")
        pl.xticks(list(sp.unique(group['symbols'])))
        pl.savefig(PATH + "dense/"+ buildername + '.eps')
        pdf.savefig(transparent=True)

    pdf.close()


if __name__ == '__main__':

    parser = argparse.ArgumentParser()

    parser.add_argument(
        '--json', dest='jsonfile', action='store',
        help='the .json file written by gauge benchmark, if non provided plots \
        from the database',
        default="")

    args = parser.parse_args()

    plot(args)
