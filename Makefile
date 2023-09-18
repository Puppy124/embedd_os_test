#--------------------------------------Makefile-------------------------------------
CFILES = $(wildcard ./src/kernel/*.c)
OFILES = $(CFILES:./src/kernel/%.c=./object/%.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib
LDFLAGS = -nostdlib -nostartfiles

all: clean kernel8.img run


./object/boot.o: ./src/kernel/boot.S
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./src/kernel/boot.S -o ./object/boot.o

./object/uart.o: ./src/uart/uart.c
	@echo building UART
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./object/%.o: ./src/kernel/%.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./object/mailbox.o: ./src/mailbox/mbox.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./object/custom.o: ./src/custom/custom.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

./object/printf.o: ./src/printf/printf.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

#frame buffer
./object/framebf.o : ./src/frameBuffer/framebf.c
	aarch64-none-elf-gcc $(GCCFLAGS) -c ./src/frameBuffer/framebf.c -o ./object/framebf.o

kernel8.img: ./object/boot.o ./object/uart.o ./object/mailbox.o ./object/custom.o ./object/printf.o ./object/framebf.o $(OFILES)
	aarch64-none-elf-gcc $(LDFLAGS) ./object/boot.o ./object/uart.o ./object/mailbox.o ./object/custom.o ./object/printf.o ./object/framebf.o $(OFILES) -T ./src/kernel/link.ld -o ./object/kernel8.elf
	aarch64-none-elf-objcopy -O binary ./object/kernel8.elf kernel8.img

clean:
	del .\object\kernel8.elf .\object\*.o *.img

run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -serial null -serial stdio
