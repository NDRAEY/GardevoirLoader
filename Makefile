# Use `chcp 1251` to decode errors

NDK ?= android-ndk-r25c
NDK_PATH ?= $(HOME)/$(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin

SRC = src/main.cpp \
      src/Video/BackFramebuffer.cpp \
      src/Video/Screen.cpp \
      src/Graphics/Pixel.cpp \
      src/Graphics/Rect.cpp \
      src/Graphics/PSFFont.cpp \
	  src/Input/Event.cpp \
	  src/UI/List.cpp \

OBJS = $(SRC:.cpp=.o)
PROJECT = gloader

RM ?= rm
CPP = $(NDK_PATH)/armv7a-linux-androideabi26-clang++
STRIP = $(NDK_PATH)/llvm-strip
CPPFLAGS ?= -Iinclude -static -O0 -Wall

PY ?= python3

all: $(PROJECT)

$(PROJECT): $(OBJS)
	$(CPP) $(OBJS) -static -o $(PROJECT)
	$(PY) tools/fix_tls_alignment.py $(PROJECT)
	$(STRIP) -S $(PROJECT)

$(OBJS): %.o : %.cpp
	$(CPP) $? -c $(CPPFLAGS) -o $@

clean:
	-$(RM) $(PROJECT)
	-$(RM) $(OBJS)

setup_device:
	@echo "Now, turn your phone into recovery mode."
	@echo -n "Press any key to continue..."
	@bash -c read -sn1

	adb shell mount /dev/block/platform/*/by-name/system /system

push:
	@echo "Now, connect your phone in pseudo-recovery mode."
	@echo -n "Press any key to continue..."
	@bash -c read -sn1
	
	adb push $(PROJECT) /
	adb push res/UniCyr8x8.psf /
	adb shell chmod +x /$(PROJECT)

run:
	adb shell /$(PROJECT)

everything:
	$(MAKE)
	$(MAKE) push
	$(MAKE) run
