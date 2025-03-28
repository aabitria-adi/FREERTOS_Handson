PROJECT := freertos

FREERTOS := ThirdParty/FreeRTOS
SEGGER := ThirdParty/SEGGER

VPATH := \
	./Core/Src \
	./Core/Startup \
	./Drivers/STM32F1xx_HAL_Driver/Src \
	$(FREERTOS) \
	$(FREERTOS)/portable/GCC/ARM_CM3 \
	$(FREERTOS)/portable/MemMang \
	$(SEGGER)/Config \
	$(SEGGER)/OS \
	$(SEGGER)/SEGGER \
	$(SEGGER)/SEGGER/Syscalls \
	$(SEGGER)/Rec
	

INCLUDES  := \
	-ICore/Inc \
	-IDrivers/STM32F1xx_HAL_Driver/Inc \
	-IDrivers/STM32F1xx_HAL_Driver/Inc/Legacy \
	-IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
	-IDrivers/CMSIS/Include \
	-I$(FREERTOS)/include \
	-I$(FREERTOS)/portable/GCC/ARM_CM3 \
	-I$(SEGGER)/Config \
	-I$(SEGGER)/OS \
	-I$(SEGGER)/SEGGER


ASM_SRCS := \
	startup_stm32f103c8tx.s \

CORE_SRCS := \
	display.c \
	gpio.c \
	main.c \
	serial_print.c \
	stm32f1xx_hal_msp.c \
	stm32f1xx_hal_timebase_tim.c \
	stm32f1xx_it.c \
	syscalls.c \
	system_common.c \
	system_stm32f1xx.c \
	usart.c

HAL_DRIVER_SRCS := \
	stm32f1xx_hal_cortex.c \
	stm32f1xx_hal_dma.c \
	stm32f1xx_hal_exti.c \
	stm32f1xx_hal_flash_ex.c \
	stm32f1xx_hal_flash.c \
	stm32f1xx_hal_gpio_ex.c \
	stm32f1xx_hal_gpio.c \
	stm32f1xx_hal_pwr.c \
	stm32f1xx_hal_rcc_ex.c \
	stm32f1xx_hal_rcc.c \
	stm32f1xx_hal_tim_ex.c \
	stm32f1xx_hal_tim.c \
	stm32f1xx_hal_uart.c \
	stm32f1xx_hal.c

FREERTOS_SRCS := \
	croutine.c \
	event_groups.c \
	list.c \
	queue.c \
	stream_buffer.c \
	tasks.c \
	timers.c \
	port.c \
	heap_4.c

SEGGER_ASM_SRCS := \
	SEGGER_RTT_ASM_ARMv7M.S

SEGGER_SRCS := \
	SEGGER_SYSVIEW_Config_FreeRTOS.c \
	SEGGER_SYSVIEW_FreeRTOS.c \
	SEGGER_RTT_printf.c \
	SEGGER_RTT.c \
	SEGGER_SYSVIEW.c \
	segger_uart.c
	#SEGGER_RTT_Syscalls_GCC.c

ASM_DEFS := -DDEBUG 

C_DEFS := \
	-DDEBUG \
	-DUSE_HAL_DRIVER \
	-DSTM32F103xB

OUTPUT:=$(PROJECT)
LD_SCRIPT:=STM32F103C8TX_FLASH.ld

ARM_CPU:=-mcpu=cortex-m3

C_SRCS:= \
	$(CORE_SRCS) \
	$(HAL_DRIVER_SRCS) \
	$(FREERTOS_SRCS) \
	$(SEGGER_SRCS)

################################################################################
# Place the ARM GNU toolchain bin location to this variable
################################################################################
GNU_TOOLS_DIR := \
	C:/"Program Files (x86)"/"Arm GNU Toolchain arm-none-eabi/14.2 rel1"

CC    := $(GNU_TOOLS_DIR)/bin/arm-none-eabi-gcc
AS    := $(GNU_TOOLS_DIR)/bin/arm-none-eabi-gcc
LINK  := $(GNU_TOOLS_DIR)/bin/arm-none-eabi-gcc
BIN   := $(GNU_TOOLS_DIR)/bin/arm-none-eabi-objcopy


BIN_DIR := Debug

C_OBJS   := $(patsubst %.c,%.o,$(C_SRCS))
ASM_OBJS := $(patsubst %.s,%.o,$(ASM_SRCS))
SEGGER_ASM_OBJS := $(patsubst %.S,%.o,$(SEGGER_ASM_SRCS))

TARGET_BIN   := $(BIN_DIR)/$(OUTPUT).bin
TARGET_ELF   := $(BIN_DIR)/$(OUTPUT).elf
TARGET_MAP   := $(BIN_DIR)/$(OUTPUT).map
ASM_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(ASM_OBJS))
SEGGER_ASM_OBJS_EXT := $(addprefix $(BIN_DIR)/, $(SEGGER_ASM_OBJS))
C_OBJS_EXT   := $(addprefix $(BIN_DIR)/, $(C_OBJS))
C_DEPS_EXT   := $(patsubst %.o, %.d, $(C_OBJS_EXT))

ASFLAGS := $(ARM_CPU) -mfloat-abi=soft -mthumb -g3 -c -x assembler-with-cpp \
			--specs=nano.specs $(ASM_DEFS) -I$(SEGGER)/Config
	
CFLAGS := -g $(ARM_CPU) -mfloat-abi=soft -mthumb -std=gnu11 -g3 -O0 \
			-ffunction-sections -fdata-sections	-Wall -fstack-usage \
			$(INCLUDES) $(C_DEFS) 
	
LINKFLAGS := $(ARM_CPU) -T"./$(LD_SCRIPT)" -Wl,-Map="$(TARGET_MAP)" \
			-specs=nano.specs -specs=nosys.specs -Wl,--gc-sections -static \
			-mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group


ifeq ("$(wildcard $(BIN_DIR))","")
$(shell mkdir $(BIN_DIR))
endif

# rules

all: $(TARGET_BIN)

$(TARGET_BIN): $(TARGET_ELF)
	@echo OBJCOPY $(notdir $@)
	@$(BIN) -O binary $< $@

$(TARGET_ELF) : $(C_OBJS_EXT) $(ASM_OBJS_EXT) $(SEGGER_ASM_OBJS_EXT)
	@echo LD $(notdir $@)
	@$(CC) $(CFLAGS) $(LINKFLAGS) -o $@ $^
	
$(BIN_DIR)/%.d : %.c
	@$(CC) -MM -MT $(@:.d=.o) $(CFLAGS) $< > $@


$(BIN_DIR)/%.o : %.c
	@echo CC $(notdir $<)
	@$(CC) -c $(CFLAGS) $< -o $@


$(BIN_DIR)/%.o : %.s
	@echo AS $(notdir $<)
	@$(AS) $(ASFLAGS) $< -o $@

$(BIN_DIR)/%.o : %.S
	@echo AS $(notdir $<)
	@$(AS) $(ASFLAGS) $< -o $@

clean:
	@echo "Cleaning Build..."
	@rm -Rf $(BIN_DIR)
	@echo Done.


