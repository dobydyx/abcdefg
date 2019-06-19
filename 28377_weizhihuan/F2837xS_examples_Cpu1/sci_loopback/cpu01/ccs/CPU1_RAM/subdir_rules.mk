################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
AD2S1210.obj: ../AD2S1210.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="AD2S1210.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_Adc.obj: ../F2837xS_Adc.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_Adc.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_CodeStartBranch.obj: ../F2837xS_CodeStartBranch.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_CodeStartBranch.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_CpuTimers.obj: ../F2837xS_CpuTimers.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_CpuTimers.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_DefaultISR.obj: ../F2837xS_DefaultISR.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_DefaultISR.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_EPwm.obj: ../F2837xS_EPwm.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_EPwm.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_GlobalVariableDefs.obj: ../F2837xS_GlobalVariableDefs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_GlobalVariableDefs.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_Gpio.obj: ../F2837xS_Gpio.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_Gpio.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_PieCtrl.obj: ../F2837xS_PieCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_PieCtrl.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_PieVect.obj: ../F2837xS_PieVect.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_PieVect.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_Sci.obj: ../F2837xS_Sci.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_Sci.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_Spi.obj: ../F2837xS_Spi.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_Spi.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_SysCtrl.obj: ../F2837xS_SysCtrl.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_SysCtrl.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_struct.obj: ../F2837xS_struct.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_struct.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

F2837xS_usDelay.obj: ../F2837xS_usDelay.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="F2837xS_usDelay.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

absolute_encoder.obj: ../absolute_encoder.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="absolute_encoder.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 --cla_support=cla1 -O4 --include_path="C:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.1.LTS/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_headers/include" --include_path="G:/CCSV8_WorkSpace/28377_weizhihuan/F2837xS_common/include" -g --define=CPU1 --diag_suppress=10063 --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="main.d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


