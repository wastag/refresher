avrdude.exe -p m32u4 -P usb -c usbasp  -U flash:w:dbg/refresher-m32u4.hex:i
@echo %~dp0
PAUSE