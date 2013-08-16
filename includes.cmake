set(INCLUDES ${INCLUDES}
	build_configs/
	Base/include
	RenderUnits/include
	RenderUnits/GL/include
	RenderUnits/GLSL/include
	RenderUnits/GLES/include
	SysDependent/include
	Plugins/AEVision/include
	Plugins/COLLADA/include
	/usr/include/collada-dom2.4/
	# /usr/include/collada-dom2.4/1.4/
	/usr/include/collada-dom2.4/1.5/)

if(TARGET_PLATFORM STREQUAL "RPI")
	set(INCLUDES ${INCLUDES}
		/opt/vc/include/
		/opt/vc/include/interface/vcos/pthreads
		/opt/vc/include/interface/vmcs_host/linux)
endif()
