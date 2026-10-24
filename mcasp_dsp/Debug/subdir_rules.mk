################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'C6000 Compiler - building file: "$<"'
	"D:/MyFolder/Apps/Engineering/TI/ccs/tools/compiler/ti-cgt-c6000_8.5.0.LTS/bin/cl6x" -mv6740 --include_path="D:/ccs_workspace/mcasp/mcasp_dsp" --include_path="D:/MyFolder/Apps/Engineering/TI/ccs/tools/compiler/ti-cgt-c6000_8.5.0.LTS/include" --include_path="C:/Program Files (x86)/Texas Instruments/pdk_C6748_2_0_0_0/C6748_StarterWare_1_20_03_03/include/hw" --include_path="C:/Program Files (x86)/Texas Instruments/pdk_C6748_2_0_0_0/C6748_StarterWare_1_20_03_03/include" --include_path="C:/Program Files (x86)/Texas Instruments/pdk_C6748_2_0_0_0/C6748_StarterWare_1_20_03_03/include/" --include_path="C:/Program Files (x86)/Texas Instruments/pdk_C6748_2_0_0_0/C6748_StarterWare_1_20_03_03/include/c674x/c6748" --define=c6748 -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


