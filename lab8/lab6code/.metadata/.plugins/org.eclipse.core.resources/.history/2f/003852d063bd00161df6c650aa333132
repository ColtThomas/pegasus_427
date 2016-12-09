################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/alienHandler.c \
../src/aliens.c \
../src/arduino.c \
../src/bulletHandler.c \
../src/bullets.c \
../src/bunkers.c \
../src/core.c \
../src/dma_magic.c \
../src/globals.c \
../src/helloworld.c \
../src/pitiful.c \
../src/platform.c \
../src/saucer.c \
../src/screen.c \
../src/sound.c \
../src/tank.c \
../src/text.c \
../src/xac97_l.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/alienHandler.o \
./src/aliens.o \
./src/arduino.o \
./src/bulletHandler.o \
./src/bullets.o \
./src/bunkers.o \
./src/core.o \
./src/dma_magic.o \
./src/globals.o \
./src/helloworld.o \
./src/pitiful.o \
./src/platform.o \
./src/saucer.o \
./src/screen.o \
./src/sound.o \
./src/tank.o \
./src/text.o \
./src/xac97_l.o 

C_DEPS += \
./src/alienHandler.d \
./src/aliens.d \
./src/arduino.d \
./src/bulletHandler.d \
./src/bullets.d \
./src/bunkers.d \
./src/core.d \
./src/dma_magic.d \
./src/globals.d \
./src/helloworld.d \
./src/pitiful.d \
./src/platform.d \
./src/saucer.d \
./src/screen.d \
./src/sound.d \
./src/tank.d \
./src/text.d \
./src/xac97_l.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../hello_world_bsp_0/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


