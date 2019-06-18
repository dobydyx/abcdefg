################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
source/ePWM/InitEPWMx.obj: ../source/ePWM/InitEPWMx.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"D:/Program Files (x86)/CCS6.0/ccsv7/tools/compiler/ti-cgt-c2000_17.9.0.STS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --include_path="E:/DSP28377/workspace_v7/MotorCtrl_28377/include" --include_path="E:/DSP28377/workspace_v7/MotorCtrl_28377/include/SysHeader" --include_path="D:/Program Files (x86)/CCS6.0/ccsv7/tools/compiler/ti-cgt-c2000_17.9.0.STS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="source/ePWM/InitEPWMx.d" --obj_directory="source/ePWM" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


