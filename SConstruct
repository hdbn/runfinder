# -*- encoding: utf-8 -*-

import os, sys, glob

env = Environment(CC="gcc",CXX="g++",
                  CCFLAGS="-fast -Wall", LINKFLAGS="-fast -Wall",
                  CPPPATH = ["/opt/local/include"])

envDebug = Environment(CC="gcc",CXX="g++",
                       CCFLAGS="-g -Wall", LINKFLAGS="-g -Wall",
                       CPPPATH = ["/opt/local/include"])

# use to force 32 bit compile
# env = Environment(CC="gcc",CXX="g++", CCFLAGS="-fast -Wall -m32", LINKFLAGS="-fast -Wall -m32")

# use to force 64 bit compile
# env = Environment(CC="gcc",CXX="g++", CCFLAGS="-fast -Wall -m64", LINKFLAGS="-fast -Wall -m64")

sources_common = ["divsufsort.c", "bits.c", "lz77.cpp", "suffixArray.cpp", "runFinder.cpp" ]
sources_main = ["runFinderMain.cpp"]

objects_common = env.Object(sources_common)
objects_main = env.Object(sources_main)

debug_objects_common = [envDebug.Object(src+'.debug.o', src) for src in sources_common]
debug_objects_main = [envDebug.Object(src+'.debug.o', src) for src in sources_main]

env.Program("runFinder", objects_common + ["runFinderMain.o"])
envDebug.Program("runFinder.debug", debug_objects_common + ["runFinderMain.cpp.debug.o"])

####################################################
# tests: uses google-test
####################################################
#
def runUnitTest(env,target,source):
    import subprocess
    app = str(source[0].abspath)
    if not subprocess.call(app):
        open(str(target[0]),'w').write("PASSED\n")

if os.path.exists('/opt/local/lib/libgtest.a') and os.path.exists('/opt/local/lib/libgtest_main.a'):
    testProg = 'runTests'
    envTEST = env.Clone(CPPPATH = ['./', '/opt/local/include'],
                        LIBPATH=['/opt/local/lib', './'],
                        LIBS=['gtest', 'gtest_main'])
    program = envTEST.Program(testProg, glob.glob('tests/*.cpp') + objects_common)
    debug_program = envTEST.Program(testProg+'.debug',  glob.glob('tests/*.cpp') + debug_objects_common)
    Command("tests.passed", testProg, runUnitTest)
    test_alias = Alias(testProg, [program], program[0].abspath)
    AlwaysBuild(test_alias)
else:
    print "Google test not found in /opt/local/lib/. Testing skipped."
