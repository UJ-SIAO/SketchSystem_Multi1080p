connect -url tcp:127.0.0.1:3121
source D:/Project/SketchSystem_Multi1080p/SketchSystem.sdk/system_wrapper_hw_platform_0/ps7_init.tcl
targets -set -filter {jtag_cable_name =~ "Digilent Zed 210248585775" && level==0} -index 1
fpga -file D:/Project/SketchSystem_Multi1080p/SketchSystem.sdk/system_wrapper_hw_platform_0/system_wrapper.bit
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zed 210248585775"} -index 0
loadhw -hw D:/Project/SketchSystem_Multi1080p/SketchSystem.sdk/system_wrapper_hw_platform_0/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zed 210248585775"} -index 0
stop
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zed 210248585775"} -index 0
rst -processor
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zed 210248585775"} -index 0
dow D:/Project/SketchSystem_Multi1080p/SketchSystem.sdk/videoImageTest/Debug/videoImageTest.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zed 210248585775"} -index 0
con
