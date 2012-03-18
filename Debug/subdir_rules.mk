################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Main.obj: ../Main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/ti/ccsv4/tools/compiler/msp430/bin/cl430" -vmsp -g -O0 --define=__MSP430G2553__ --include_path="C:/ti/ccsv4/msp430/include" --include_path="C:/ti/ccsv4/tools/compiler/msp430/include" --diag_warning=225 --printf_support=minimal --preproc_with_compile --preproc_dependency="Main.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


