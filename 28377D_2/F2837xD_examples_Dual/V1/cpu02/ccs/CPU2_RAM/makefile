################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/sofeware/ccs6.0/ccsv6/tools/compiler/ti-cgt-c2000_6.4.6

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./AD2S1210.obj" \
"./F2837xD_Adc.obj" \
"./F2837xD_CodeStartBranch.obj" \
"./F2837xD_CpuTimers.obj" \
"./F2837xD_DefaultISR.obj" \
"./F2837xD_GlobalVariableDefs.obj" \
"./F2837xD_Gpio.obj" \
"./F2837xD_Ipc.obj" \
"./F2837xD_Ipc_Driver.obj" \
"./F2837xD_Ipc_Driver_Lite.obj" \
"./F2837xD_Ipc_Driver_Util.obj" \
"./F2837xD_PieCtrl.obj" \
"./F2837xD_PieVect.obj" \
"./F2837xD_Sci.obj" \
"./F2837xD_SysCtrl.obj" \
"./F2837xD_struct.obj" \
"./F2837xD_usDelay.obj" \
"./absolute_encoder.obj" \
"./main2.obj" \
$(GEN_CMDS__FLAG) \
-l"rts2800_fpu32.lib" \
-l"2837xD_RAM_lnk_cpu2.cmd" \
-l"F2837xD_Headers_nonBIOS_cpu2.cmd" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
13kW_normal_cpu02.out \

EXE_OUTPUTS__QUOTED += \
"13kW_normal_cpu02.out" \

BIN_OUTPUTS += \
13kW_normal_cpu02.hex \

BIN_OUTPUTS__QUOTED += \
"13kW_normal_cpu02.hex" \


# All Target
all: 13kW_normal_cpu02.out

# Tool invocations
13kW_normal_cpu02.out: $(OBJS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"D:/sofeware/ccs6.0/ccsv6/tools/compiler/ti-cgt-c2000_6.4.6/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --cla_support=cla1 --vcu_support=vcu2 -g --define=CPU2 --diag_warning=225 --display_error_number -z -m"13kW_normal_cpu02.map" --stack_size=0x400 --heap_size=0x400 --warn_sections -i"D:/sofeware/ccs6.0/ccsv6/tools/compiler/ti-cgt-c2000_6.4.6/lib" -i"D:/sofeware/ccs6.0/ccsv6/tools/compiler/ti-cgt-c2000_6.4.6/include" -i"C:/Users/chuang/Desktop/28377D_2/F2837xD_common/cmd" -i"C:/Users/chuang/Desktop/28377D_2/F2837xD_headers/cmd" --reread_libs --display_error_number --xml_link_info="13kW_normal_cpu02_linkInfo.xml" --rom_model -o "13kW_normal_cpu02.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

13kW_normal_cpu02.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: C2000 Hex Utility'
	"D:/sofeware/ccs6.0/ccsv6/tools/compiler/ti-cgt-c2000_6.4.6/bin/hex2000"  -o "13kW_normal_cpu02.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "AD2S1210.pp" "F2837xD_Adc.pp" "F2837xD_CpuTimers.pp" "F2837xD_DefaultISR.pp" "F2837xD_GlobalVariableDefs.pp" "F2837xD_Gpio.pp" "F2837xD_Ipc.pp" "F2837xD_Ipc_Driver.pp" "F2837xD_Ipc_Driver_Lite.pp" "F2837xD_Ipc_Driver_Util.pp" "F2837xD_PieCtrl.pp" "F2837xD_PieVect.pp" "F2837xD_Sci.pp" "F2837xD_SysCtrl.pp" "F2837xD_struct.pp" "absolute_encoder.pp" "main2.pp" 
	-$(RM) "AD2S1210.obj" "F2837xD_Adc.obj" "F2837xD_CodeStartBranch.obj" "F2837xD_CpuTimers.obj" "F2837xD_DefaultISR.obj" "F2837xD_GlobalVariableDefs.obj" "F2837xD_Gpio.obj" "F2837xD_Ipc.obj" "F2837xD_Ipc_Driver.obj" "F2837xD_Ipc_Driver_Lite.obj" "F2837xD_Ipc_Driver_Util.obj" "F2837xD_PieCtrl.obj" "F2837xD_PieVect.obj" "F2837xD_Sci.obj" "F2837xD_SysCtrl.obj" "F2837xD_struct.obj" "F2837xD_usDelay.obj" "absolute_encoder.obj" "main2.obj" 
	-$(RM) "F2837xD_CodeStartBranch.pp" "F2837xD_usDelay.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

