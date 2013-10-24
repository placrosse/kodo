"""
Copyright Steinwurf ApS 2011-2013.
Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
See accompanying file LICENSE.rst or
http://www.steinwurf.com/licensing

Plot the difference in throughput between all branches where the benchmarks have
been run within the last 72 hours for all supported Kodo platforms

In order for this script to output some comparison figures you should force the
relevant benchmark run on our buildslaves: http://176.28.49.184:12344/buildslaves
"""

import pandas as pd
import scipy as sp
import processing_shared as ps

from datetime import datetime, timedelta
now = datetime.utcnow()
today = now.date()
today = datetime(today.year, today.month, today.day)
yesterday = today - timedelta(1)

query_master = {
"type": "decoder",
"branch" : "master",
"scheduler": "kodo-nightly-benchmark",
"utc_date" : {"$gte": yesterday, "$lt": today}
}

query_branches = {
"type": "decoder",
"scheduler": "kodo-force-benchmark",
"utc_date" : {"$gte": now - timedelta(3)}
}

cursor_master = query_database(query_master)
cursor_branches = query_database(query_branches)

df_all = pd.DataFrame.from_records( sp.hstack( [list(cursor_master), list(cursor_branches)] ))
ps.calculate(df_all)
groups = df_all.groupby(['buildername'])

from matplotlib import pyplot as pl
from matplotlib.backends.backend_pdf import PdfPages as pp
pl.close('all')

branches = list(sp.unique(df_all['branch']))
pdf = {}
for branch in branches:
	if branch != "master":
		PATH  = ("figures/throughput_gain/" + branch ).replace("-","_")
		ps.mkdir_p(PATH + "/sparse")
		ps.mkdir_p(PATH + "/dense")
		pdf[branch] = pp(PATH + "/all.pdf")

for buildername, group in groups:

    branches_group = group[group['branch'] != "master"].groupby(by = ['branch'])
    master_group = group[sp.array(group['branch'] == "master")]
    master_group = master_group[master_group['buildnumber'] == max(master_group['buildnumber'])]

    group[group['branch'] == "master"]
    if len(master_group) == 0:
        print "Skipping " + buildername + " as no nightly benchmark resutls exists for the master for this buider yet"
        continue

    for branch, branch_group in branches_group:
        PATH  = ("figures/throughput_gain/" + branch ).replace("-","_")

        branch_group = branch_group[branch_group["buildnumber"] == max(branch_group['buildnumber'])]
        branch_group['gain'] = (sp.array(branch_group['mean']) - sp.array(master_group['mean']) ) / sp.array(master_group['mean']) * 100

        sparse = branch_group[branch_group['testcase'] == "SparseFullRLNC"].groupby(by= ['symbol_size'])
        dense = branch_group[branch_group['testcase'] != "SparseFullRLNC"].groupby(by= ['symbol_size'])

        for key, g in sparse:
            p = ps.plot_sparse_group(g, "gain", buildername)
            pl.ylabel("Throughput gain [%]")
            pl.savefig(PATH + "/sparse/" + buildername + '.eps')
            pdf[branch].savefig(transparent=True)

        for key, g in dense:
            p = ps.plot_dense_group(g, "gain", buildername)
            pl.ylabel("Throughput gain [%]")
            pl.savefig(PATH + "/dense/" + buildername + '.eps')
            pdf[branch].savefig(transparent=True)

for p in pdf:
	pdf[p].close()
