from building import *
import rtconfig

# get current directory
cwd     = GetCurrentDir()
# The set of source files associated with this SConscript file.
src     = Glob('inc/*.h')
src    += Glob('port/*.h')
src    += Glob('src/*.cpp')

path    = [cwd + '/']
path   += [cwd + '/port']
path   += [cwd + '/src']

LOCAL_CCFLAGS = ''

if rtconfig.CROSS_TOOL == 'gcc':
    LOCAL_CCFLAGS += ' -std=c99'
elif rtconfig.CROSS_TOOL == 'keil':
    LOCAL_CCFLAGS += '-O2 --c99 --gnu'

group = DefineGroup('ROSSERIAL', src, depend = ['PKG_USING_ROSSERIAL'], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
