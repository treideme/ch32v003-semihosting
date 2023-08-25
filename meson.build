project('CH32V003 Semihosting Sample', 
	'c',
	version : '0.1',
	default_options : [ 
		'buildtype=minsize', 
		'c_std=gnu99',
	],
)

assert(meson.is_cross_build(), 'Use cross-build for CH32V003 Target')
objcopy = find_program('riscv-none-embed-objcopy', required : true)
objdump = find_program('riscv-none-embed-objdump', required : true)

incdir = [
    include_directories('Core'),
    include_directories('Debug'),
    include_directories('Peripheral/inc'),
    include_directories('Startup'),
    include_directories('User'),
]

t_executable = '@0@.elf'
t_bin = '@0@.bin'
t_hex = '@0@.hex'
t_asm = '@0@.S'
t_startup = 'Startup/startup_@0@.S'
t_flash = 'flash_@0@'

t_mapfile = '-Wl,-Map,@0@.map'
t_ldflag = '-Wl,-T'+meson.source_root()+'/Ld/Link.ld'

progs = [
	['hello',
	    ['Core/core_riscv.c',
	     
	     'Peripheral/src/ch32v00x_gpio.c',
	     'Peripheral/src/ch32v00x_rcc.c',
	     'Peripheral/src/ch32v00x_misc.c',
	     'Peripheral/src/ch32v00x_dbgmcu.c',
	     
	     'User/ch32v00x_it.c',
	     'User/main.c',
	     'User/semihosting.c',
	     'User/system_ch32v00x.c',
	     ],
    'Hello World Example',
    'ch32v00x'],
]

foreach p : progs

	exe = executable(t_executable.format(p[0]), [p[1], t_startup.format(p[3])],
		link_args : [t_mapfile.format(p[0]), t_ldflag],
		include_directories : [incdir],
		build_by_default : true,
	)
	bin = custom_target(t_bin.format(p[0]),
		output : [t_bin.format(p[0])],
		build_by_default : true,
		command : [objcopy, '-O', 'binary', t_executable.format(p[0]), t_bin.format(p[0])],
        depends : exe)
	hex = custom_target(t_hex.format(p[0]),
		output : [t_hex.format(p[0]),],
        build_by_default : true,
        command : [objcopy, '-O', 'ihex', t_executable.format(p[0]), t_hex.format(p[0])],
        depends : exe)
	asm = custom_target(t_asm.format(p[0]),
		output : t_asm.format(p[0]),
        build_by_default : true,
        command : [objdump, '-d', '-S', t_executable.format(p[0])],
		capture : true,
        depends : exe)
endforeach
