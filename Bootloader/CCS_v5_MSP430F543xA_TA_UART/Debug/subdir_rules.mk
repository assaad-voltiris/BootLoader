################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
BSL430_API.obj: H:/Projects/Voltiris/CCS_BSL_Source/API/BSL430_API.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --data_model=restricted -O4 --opt_for_speed=2 --use_hw_mpy=F5 --include_path="C:/ti/ccs1240/ccs/ccs_base/msp430/include" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/API" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/Peripheral_Interfaces" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/Command_Interpreter" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/CCS_v5_MSP430F543xA_TA_UART" --include_path="C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power="all" --define=__MSP430F5438A__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --single_inline --remove_hooks_when_inlining --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

BSL430_Command_Interpreter.obj: H:/Projects/Voltiris/CCS_BSL_Source/Command_Interpreter/BSL430_Command_Interpreter.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --data_model=restricted -O4 --opt_for_speed=2 --use_hw_mpy=F5 --include_path="C:/ti/ccs1240/ccs/ccs_base/msp430/include" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/API" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/Peripheral_Interfaces" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/Command_Interpreter" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/CCS_v5_MSP430F543xA_TA_UART" --include_path="C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power="all" --define=__MSP430F5438A__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --single_inline --remove_hooks_when_inlining --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --data_model=restricted -O4 --opt_for_speed=2 --use_hw_mpy=F5 --include_path="C:/ti/ccs1240/ccs/ccs_base/msp430/include" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/API" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/Peripheral_Interfaces" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/Command_Interpreter" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/CCS_v5_MSP430F543xA_TA_UART" --include_path="C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power="all" --define=__MSP430F5438A__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --single_inline --remove_hooks_when_inlining --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

BSL430_PI_TA.obj: H:/Projects/Voltiris/CCS_BSL_Source/Peripheral_Interfaces/BSL430_PI_TA.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --data_model=restricted -O4 --opt_for_speed=2 --use_hw_mpy=F5 --include_path="C:/ti/ccs1240/ccs/ccs_base/msp430/include" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/API" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/Peripheral_Interfaces" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/Command_Interpreter" --include_path="H:/Projects/Voltiris/CCS_BSL_Source/CCS_v5_MSP430F543xA_TA_UART" --include_path="C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power="all" --define=__MSP430F5438A__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --single_inline --remove_hooks_when_inlining --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


