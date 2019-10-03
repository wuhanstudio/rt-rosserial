from building import *
import rtconfig

# get current directory
cwd     = GetCurrentDir()
# The set of source files associated with this SConscript file.
src     = Glob('src/*.cpp')

path    = [cwd + '/']
path   += [cwd + '/port']
path   += [cwd + '/src']

LOCAL_CCFLAGS = ''

if GetDepend('ROSSERIAL_USING_HELLO_WORLD_UART'):
    src    += Glob('examples/hello_world_serial.cpp')

if GetDepend('ROSSERIAL_USING_BLINK_UART'):
    src    += Glob('examples/blink.cpp')

if GetDepend('ROSSERIAL_USING_BLINK_CLASS_UART'):
    src    += Glob('examples/blink_class.cpp')

if GetDepend('ROSSERIAL_USING_BUTTON_UART'):
    src    += Glob('examples/button.cpp')

if GetDepend('ROSSERIAL_USING_HELLO_WORLD_TCP'):
    src    += Glob('examples/hello_world_tcp.cpp')

if rtconfig.CROSS_TOOL == 'gcc':
    LOCAL_CCFLAGS += ' -std=c99'

group = DefineGroup('rosserial', src, depend = ['PKG_USING_ROSSERIAL'], CPPPATH = path, LOCAL_CCFLAGS = LOCAL_CCFLAGS)

Return('group')
