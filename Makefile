C_FILES = vpu25.c ascu2.c mc68230.c vbic.c
O_FILES = $(patsubst %.c,%.o,$(C_FILES))
D_FILES = $(patsubst %.c,%.d,$(C_FILES))

GCCFLAGS = -Wall -m68010 -mtune=68020 -g
GCCFLAGS += -MMD
LDFLAGS = -T vpu25.ld -Xlinker -Map=vpu25.map

all: vpu25.run

vpu25.run: vpu25.o ascu2.o mc68230.o vbic.o
	m68k-elf-gcc $(GCCFLAGS) -O $^ -o $@ $(LDFLAGS)

%.o: %.c
	m68k-elf-gcc $(GCCFLAGS) -c $< -o $@ 

clean:
	$(RM) $(O_FILES) vpu25.run vpu25.map
	$(RM) $(D_FILES)

distclean: clean

upload: vpu25.run
	/bin/echo -e "@,,0\r\n" >/dev/ttyUSB0
	/bin/echo -e "L\r\n" >/dev/ttyUSB0
	sleep 1
	ascii-xfr -s -l 100 $< >/dev/ttyUSB0
	sleep 1
	/bin/echo -e "J\r\n" >/dev/ttyUSB0

-include $(D_FILES)
