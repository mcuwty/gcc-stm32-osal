#工程名称
TARGET 			= OSAL_example

#设置编译器
CC     			= arm-none-eabi-gcc
#CC     			= /opt/arm-none-gcc-5.4/bin/arm-none-eabi-gcc
OBJCOPY			= arm-none-eabi-objcopy
#OBJCOPY			= /opt/arm-none-gcc-5.4/bin/arm-none-eabi-objcopy
ARMSIZE			= arm-none-eabi-size

#获取当前工作目录
TOP=.

#设置源文件后缀，c或cpp
EXT				= c

#设置源文件搜索路径
VPATH			+= $(TOP)/APP:$(TOP)/CORE:$(TOP)/HARDWARE
VPATH			+= $(TOP)/OSAL/application:$(TOP)/OSAL/hal:$(TOP)/OSAL/osal
VPATH			+= $(TOP)/STM32F10x_FWLib/src

#设置自定义源文件目录
APP_DIR			= $(TOP)/APP
HARD_DIR		= $(TOP)/HARDWARE

#设置中间目标文件目录
OBJ_DIR			= $(TOP)/OBJ

#设定头文件包含目录
INC_FLAGS 		+= -I $(TOP)/APP
INC_FLAGS 		+= -I $(TOP)/CORE
INC_FLAGS 		+= -I $(TOP)/HARDWARE
INC_FLAGS 		+= -I $(TOP)/OSAL/include
INC_FLAGS 		+= -I $(TOP)/STM32F10x_FWLib/inc

#编译选项
CFLAGS 			+= -W -Wall -g -mcpu=cortex-m3 -mthumb -O2 -std=gnu11
CFLAGS 			+= -D STM32F10X_HD -D USE_STDPERIPH_DRIVER

#链接选项
LFLAGS 			+= -mthumb -mcpu=cortex-m3 -Wl,--start-group -lc -lm -Wl,--end-group -specs=nano.specs 
LFLAGS 			+= -specs=nosys.specs -static -Wl,-cref,-u,Reset_Handler -Wl,-Map=Project.map -Wl,--gc-sections 
LFLAGS 			+= -Wl,--defsym=malloc_getpagesize_P=0x80

#链接启动文件和链接脚本
LFLAGS			+= $(TOP)/CORE/startup_stm32f10x_hd.s
LFLAGS			+= -T $(TOP)/CORE/stm32_flash.ld

#固定源文件添加
C_SRC			+= $(shell find $(TOP)/CORE  -name '*.$(EXT)')
C_SRC			+= $(shell find $(TOP)/OSAL  -name '*.$(EXT)')
C_SRC			+= $(shell find $(TOP)/STM32F10x_FWLib/src  -name '*.$(EXT)')

#自定义源文件添加
C_SRC			+= $(HARD_DIR)/usart.c $(HARD_DIR)/led.c

#中间目标文件
#C_OBJ			+= $(C_SRC:%.$(EXT)=%.o)
C_SRC_NODIR		= $(notdir $(C_SRC))
C_OBJ 			= $(patsubst %.$(EXT), $(OBJ_DIR)/%.o,$(C_SRC_NODIR))

#依赖文件
C_DEP			= $(patsubst %.$(EXT), $(OBJ_DIR)/%.d,$(C_SRC_NODIR))

.PHONY: all clean rebuild upload test ctags

all:$(C_OBJ)
	@echo "linking object to $(TARGET).elf"
	@$(CC) $(C_OBJ) -o $(TARGET).elf $(LFLAGS)
	@$(ARMSIZE) $(TARGET).elf
	@echo "create $(TARGET).hex from $(TARGET).elf"
	@$(OBJCOPY) $(TARGET).elf $(TARGET).hex -Oihex
	@$(ARMSIZE) $(TARGET).hex
#$(OBJCOPY) $(TARGET).elf  $(TARGET).bin -Obinary 

#静态模式
#$(C_OBJ):%.o:%.$(EXT)
#	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o:%.$(EXT)
	@mkdir -p OBJ
	@echo "building $<"
	@$(CC) -c $(CFLAGS) $(INC_FLAGS) -o $@ $<

-include $(C_DEP)
$(OBJ_DIR)/%.d:%.$(EXT)
	@mkdir -p OBJ
	@echo "making $@"
	@set -e;rm -f $@;$(CC) -MM $(CFLAGS) $(INC_FLAGS) $< > $@.$$$$;sed 's,\($*\)\.o[ :]*,$(OBJ_DIR)/\1.o $(OBJ_DIR)/\1.d:,g' < $@.$$$$ > $@;rm -f $@.$$$$

clean:
	-rm -f $(OBJ_DIR)/*
	-rm -f $(shell find ./ -name '*.map')
	-rm -f $(shell find ./ -name '*.elf')
	-rm -f $(shell find ./ -name '*.bin')
	-rm -f $(shell find ./ -name '*.hex')

rebuild: clean all

test:
	@echo "Test !"

ctags:
	@ctags -R *

upload:
#openocd -f /usr/local/share/openocd/scripts/interface/jlink.cfg  -f /usr/local/share/openocd/scripts/target/stm32f1x.cfg -c init -c "reset halt" -c "flash write_image erase $(TOP)/$(TARGET).hex" -c reset -c shutdown
	openocd -f /usr/local/share/openocd/scripts/interface/stlink-v2.cfg  -f /usr/local/share/openocd/scripts/target/stm32f1x.cfg -c init -c "reset halt" -c "flash write_image erase $(TOP)/$(TARGET).hex" -c reset -c shutdown
