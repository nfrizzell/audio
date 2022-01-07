TARGET = make

# debug build?
DEBUG = 1
# optimization
OPT = -Og

BUILD_DIR = build

C_SOURCES =  \
firmware/main.c \
firmware/stm32f3xx_it.c \
firmware/stm32f3xx_hal_msp.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_ll_usb.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_i2c_ex.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_rcc_ex.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_gpio.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_dma.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_cortex.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pwr_ex.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_flash_ex.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_exti.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_spi_ex.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_tim_ex.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pcd.c \
firmware/drivers/STM32F3xx_HAL_Driver/Src/stm32f3xx_hal_pcd_ex.c \
firmware/system_stm32f3xx.c  

ASM_SOURCES = firmware/asm/startup_stm32f303xc.s

PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F303xC

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-Ifirmware \
-Ifirmware/drivers/STM32F3xx_HAL_Driver/Inc \
-Ifirmware/drivers/STM32F3xx_HAL_Driver/Inc/Legacy \
-Ifirmware/drivers/CMSIS/Device/ST/STM32F3xx/Include \
-Ifirmware/drivers/CMSIS/Include

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# link script
LDSCRIPT = firmware/asm/STM32F303VCTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

clean:
	-rm -rf $(BUILD_DIR)
  
# dependencies
-include $(wildcard $(BUILD_DIR)/*.d)
