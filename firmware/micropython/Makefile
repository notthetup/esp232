DEVICE ?= /dev/ttyUSB0

flash:
	ampy -p ${DEVICE} put us2n.py
	ampy -p ${DEVICE} put main.py
	ampy -p ${DEVICE} put us2n.json
	ampy -p ${DEVICE} reset