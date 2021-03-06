VUSB_DIR = protocol/vusb

OPT_DEFS += -DPROTOCOL_VUSB

SRC +=	$(VUSB_DIR)/main.c \
	$(VUSB_DIR)/vusb.c \
	$(VUSB_DIR)/usbdrv/usbdrv.c \
	$(VUSB_DIR)/usbdrv/usbdrvasm.S \
	$(VUSB_DIR)/usbdrv/oddebug.c


ifeq (yes,$(strip $(NO_UART)))
OPT_DEFS += -DNO_UART
SRC +=	$(COMMON_DIR)/sendchar_null.c
else
SRC +=	$(COMMON_DIR)/sendchar_uart.c \
	$(COMMON_DIR)/avr/uart.c
endif


# Search Path
VPATH += $(TMK_DIR)/protocol/vusb:$(TMK_DIR)/protocol/vusb/usbdrv
