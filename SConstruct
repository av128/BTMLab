import os
import sys

def isWindows():
    if os.name == 'nt':
        return True
    else:
        return False

def setLibs(env):
    if not isWindows():
        env.AppendUnique(LIBS = ['stdc++fs'])
        env.AppendUnique(LIBS = ['gomp'])

    # else need to be specified

def setDebugFlags(env):
    if isWindows():
        env.AppendUnique(CPPFLAGS = ['/FS', '/Zi'])
        env.AppendUnique(CPPFLAGS = ['/MTd'])
        env.AppendUnique(CPPFLAGS = ['/JMC'])
        env.AppendUnique(CPPFLAGS = ['/GS'])
        env.AppendUnique(CPPFLAGS = ['/DEBUG'])
        # env.AppendUnique(CPPFLAGS = ['/MACHINE:X64'])
        # env.AppendUnique(LINKFLAGS = ['/MACHINE:X64'])
        env.AppendUnique(LINKFLAGS = ['/DEBUG:FASTLINK'])

def setCppFlags(env):
    if isWindows():
        env.AppendUnique(CPPFLAGS = ['/EHsc', '/std:c++17', '/MTd', '/openmp'])
    else:
        env.AppendUnique(CPPFLAGS = ['-std=c++17'])
        env.AppendUnique(CPPFLAGS = ['-w'])
        env.AppendUnique(CPPFLAGS = ['-fopenmp'])

def setOptimizeFlags(env):
    if isWindows():
        env.AppendUnique(CPPFLAGS = ['/Ox'])
        env.AppendUnique(CPPFLAGS = ['/DEBUG'])
        env.AppendUnique(LINKFLAGS = ['/DEBUG:FASTLINK'])
    else:
        env.AppendUnique(CPPFLAGS = ['-O3'])

# ================== READ CMD OPTIONS ===================
debug = ARGUMENTS.get('debug', False)

if debug: print("================ DEBUG =================")
else: print("================= OPTIMIZE ================")
# ====================================================================

if isWindows():
    env = Environment(TARGET_ARCH='x86_64')
else:
    env = Environment()

setLibs(env)
setCppFlags(env)

if debug:
    setDebugFlags(env)
else:
    setOptimizeFlags(env)

env['CCCOMSTR'] = ''

# the environment is ready, export it and build
Export('env')

buildDir = 'debug' if debug else 'release'

SConscript('src/SConscript', variant_dir=buildDir, duplicate = 0)