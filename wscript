#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'kodo'
VERSION = '18.0.0'

def recurse_helper(ctx, name):
    if not ctx.has_dependency_path(name):
        ctx.fatal('Load a tool to find %s as system dependency' % name)
    else:
        p = ctx.dependency_path(name)
        ctx.recurse([p])


def options(opt):

    import waflib.extras.wurf_dependency_bundle as bundle
    import waflib.extras.wurf_dependency_resolve as resolve

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='boost',
        git_repository='github.com/steinwurf/external-boost-light.git',
        major_version=1))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='cpuid',
        git_repository='github.com/steinwurf/cpuid.git',
        major_version=3))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='fifi',
        git_repository='github.com/steinwurf/fifi.git',
        major_version=13))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='gauge',
        git_repository='github.com/steinwurf/cxx-gauge.git',
        major_version=7))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='gtest',
        git_repository='github.com/steinwurf/external-gtest.git',
        major_version=2))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='platform',
        git_repository='github.com/steinwurf/platform.git',
        major_version=1))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='sak',
        git_repository='github.com/steinwurf/sak.git',
        major_version=12))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='stub',
        git_repository='github.com/steinwurf/stub.git',
        major_version=2))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='tables',
        git_repository='github.com/steinwurf/tables.git',
        major_version=5))

    bundle.add_dependency(opt, resolve.ResolveGitMajorVersion(
        name='waf-tools',
        git_repository='github.com/steinwurf/external-waf-tools.git',
        major_version=2))

    opt.load("wurf_configure_output")
    opt.load('wurf_dependency_bundle')
    opt.load('wurf_dependency_resolve')
    opt.load('wurf_tools')


def configure(conf):

    if conf.is_toplevel():

        conf.load('wurf_dependency_bundle')
        conf.load('wurf_tools')

        conf.load_external_tool('install_path', 'wurf_install_path')
        conf.load_external_tool('mkspec', 'wurf_cxx_mkspec_tool')
        conf.load_external_tool('project_gen', 'wurf_project_generator')
        conf.load_external_tool('runners', 'wurf_runner')

        recurse_helper(conf, 'boost')
        recurse_helper(conf, 'cpuid')
        recurse_helper(conf, 'fifi')
        recurse_helper(conf, 'gauge')
        recurse_helper(conf, 'gtest')
        recurse_helper(conf, 'platform')
        recurse_helper(conf, 'sak')
        recurse_helper(conf, 'stub')
        recurse_helper(conf, 'tables')

        # Make is not supported on Windows
        if not conf.is_mkspec_platform('windows'):
            conf.recurse('examples/sample_makefile')


def build(bld):

    if bld.is_toplevel():

        bld.load('wurf_dependency_bundle')

        recurse_helper(bld, 'boost')
        recurse_helper(bld, 'cpuid')
        recurse_helper(bld, 'fifi')
        recurse_helper(bld, 'gauge')
        recurse_helper(bld, 'gtest')
        recurse_helper(bld, 'platform')
        recurse_helper(bld, 'sak')
        recurse_helper(bld, 'stub')
        recurse_helper(bld, 'tables')

        # Only build test when executed from the
        # top-level wscript i.e. not when included as a dependency
        # in a recurse call

        bld.recurse('test')
        bld.recurse('examples/decode_simple')
        bld.recurse('examples/encode_decode_file')
        bld.recurse('examples/encode_decode_simple')
        bld.recurse('examples/encode_decode_storage')
        bld.recurse('examples/encode_on_the_fly')
        bld.recurse('examples/encode_recode_decode_simple')
        bld.recurse('examples/rank_callback')
        bld.recurse('examples/sample_makefile')
        bld.recurse('examples/sliding_window')
        bld.recurse('examples/switch_systematic_on_off')
        bld.recurse('examples/tutorial')
        bld.recurse('examples/use_cache_decode_symbol')
        bld.recurse('examples/use_trace_layers')
        bld.recurse('examples/customize_partitioning_scheme')

        bld.recurse('benchmark/count_operations')
        bld.recurse('benchmark/decoding_probability')
        bld.recurse('benchmark/overhead')
        bld.recurse('benchmark/throughput')

    # Export own includes
    bld(name='kodo_includes',
        includes='./src',
        export_includes='./src',
        use=['cpuid', 'fifi'])

    # Export unit test includes, these files are useful for testing
    # the correctness of encoders and decoders. Since users of kodo
    # may define custom encoders and decoders in their own libraries
    # or applications we expose the unittest headers here to make them
    # easily available
    bld(export_includes='./test/src',
        name='kodo_unit_test_includes')
