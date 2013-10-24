"""
Copyright Steinwurf ApS 2011-2013.
Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
See accompanying file LICENSE.rst or
http://www.steinwurf.com/licensing

Plot the throughput for all benchmarked Kodo platforms
"""

import pandas as pd
import scipy as sp
import processing_shared as ps

from datetime import datetime, timedelta
now = datetime.utcnow()
today = now.date()
today = datetime(today.year, today.month, today.day)
yesterday = today - timedelta(1)

query = {
"type": "decoder",
"branch" : "master",
"scheduler": "kodo-nightly-benchmark",
"utc_date" : {"$gte": yesterday, "$lt": today}
}

mc = query_database(query)
df = pd.DataFrame.from_records( list(mc) )
ps.calculate(df)

sparse = df[df['testcase'] == "SparseFullRLNC"].groupby(by= ['buildername', 'symbol_size'])
dense = df[df['testcase'] != "SparseFullRLNC"].groupby(by= ['buildername', 'symbol_size'])

from matplotlib import pyplot as pl
from matplotlib.backends.backend_pdf import PdfPages as pp
pl.close('all')

PATH  = ("figures/throughput")
ps.mkdir_p(PATH + "/sparse")
ps.mkdir_p(PATH + "/dense")
pdf = pp(PATH + "/all.pdf")

for (buildername,symbols), group in sparse:
    p = ps.plot_sparse_group(group, "mean", buildername)
    p.set_yscale('log')
    pl.savefig(PATH + "/sparse/" + buildername + '.eps')
    pdf.savefig(transparent=True)

for (buildername,symbols), group in dense:
    p = ps.plot_dense_group(group, "mean", buildername)
    p.set_yscale('log')
    pl.savefig(PATH + "/dense/"+ buildername + '.eps')
    pdf.savefig(transparent=True)

pdf.close()
