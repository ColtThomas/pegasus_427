################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/aliens.c \
../src/bullets.c \
../src/bunkers.c \
../src/globals.c \
../src/helloworld.c \
../src/platform.c \
../src/screen.c \
../src/tank.c \
../src/text.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/aliens.o \
./src/bullets.o \
./src/bunkers.o \
./src/globals.o \
./src/helloworld.o \
./src/platform.o \
./src/screen.o \
./src/tank.o \
./src/text.o 

C_DEPS += \
./src/aliens.d \
./src/bullets.d \
./src/bunkers.d \
./src/globals.d \
./src/helloworld.d \
./src/platform.d \
./src/screen.d \
./src/tank.d \
./src/text.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../lab3_bsp/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


