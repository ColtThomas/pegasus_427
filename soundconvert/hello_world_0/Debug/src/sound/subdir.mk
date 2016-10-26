################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/sound/fastinvader1.c \
../src/sound/fastinvader2.c \
../src/sound/fastinvader3.c \
../src/sound/fastinvader4.c \
../src/sound/invaderkilled.c \
../src/sound/shoot.c \
../src/sound/ufo_highpitch.c \
../src/sound/ufo_lowpitch.c 

OBJS += \
./src/sound/fastinvader1.o \
./src/sound/fastinvader2.o \
./src/sound/fastinvader3.o \
./src/sound/fastinvader4.o \
./src/sound/invaderkilled.o \
./src/sound/shoot.o \
./src/sound/ufo_highpitch.o \
./src/sound/ufo_lowpitch.o 

C_DEPS += \
./src/sound/fastinvader1.d \
./src/sound/fastinvader2.d \
./src/sound/fastinvader3.d \
./src/sound/fastinvader4.d \
./src/sound/invaderkilled.d \
./src/sound/shoot.d \
./src/sound/ufo_highpitch.d \
./src/sound/ufo_lowpitch.d 


# Each subdirectory must supply rules for building sources it contributes
src/sound/%.o: ../src/sound/%.c
	@echo Building file: $<
	@echo Invoking: MicroBlaze gcc compiler
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../hello_world_bsp_0/microblaze_0/include -mlittle-endian -mxl-barrel-shift -mxl-pattern-compare -mcpu=v8.20.b -mno-xl-soft-mul -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo Finished building: $<
	@echo ' '


