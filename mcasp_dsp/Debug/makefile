################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/MyFolder/Apps/Engineering/TI/ccs/tools/compiler/ti-cgt-c6000_8.5.0.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./aic31.obj" \
"./codecif.obj" \
"./dsp_filter.obj" \
"./mcaspPlayBk_prerecorded.obj" \
"../C6748.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \
-l"C:/Program Files (x86)/Texas Instruments/pdk_C6748_2_0_0_0/C6748_StarterWare_1_20_03_03/binary/c674x/cgt_ccs/c6748/drivers/Debug/drivers.lib" \
-l"C:/Program Files (x86)/Texas Instruments/pdk_C6748_2_0_0_0/C6748_StarterWare_1_20_03_03/binary/c674x/cgt_ccs/c6748/system_config/Debug/system_config.lib" \
-l"C:/Program Files (x86)/Texas Instruments/pdk_C6748_2_0_0_0/C6748_StarterWare_1_20_03_03/binary/c674x/cgt_ccs/c6748/lcdkC6748/platform/Debug/platform.lib" \
-l"C:/Program Files (x86)/Texas Instruments/pdk_C6748_2_0_0_0/C6748_StarterWare_1_20_03_03/binary/c674x/cgt_ccs/c6748/drivers/Debug/drivers.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
mcasp_dsp.out 

EXE_OUTPUTS__QUOTED += \
"mcasp_dsp.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "mcasp_dsp.out"

# Tool invocations
mcasp_dsp.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'C6000 Linker - building target: "$@"'
	"D:/MyFolder/Apps/Engineering/TI/ccs/tools/compiler/ti-cgt-c6000_8.5.0.LTS/bin/cl6x" -mv6740 --define=c6748 -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"mcasp_dsp.map" --heap_size=0x800 --stack_size=0x800 -i"D:/MyFolder/Apps/Engineering/TI/ccs/tools/compiler/ti-cgt-c6000_8.5.0.LTS/lib" -i"D:/MyFolder/Apps/Engineering/TI/ccs/tools/compiler/ti-cgt-c6000_8.5.0.LTS/include" --reread_libs --diag_wrap=off --display_error_number --warn_sections --xml_link_info="mcasp_dsp_linkInfo.xml" --rom_model -o "mcasp_dsp.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "aic31.obj" "codecif.obj" "dsp_filter.obj" "mcaspPlayBk_prerecorded.obj" 
	-$(RM) "aic31.d" "codecif.d" "dsp_filter.d" "mcaspPlayBk_prerecorded.d" 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

