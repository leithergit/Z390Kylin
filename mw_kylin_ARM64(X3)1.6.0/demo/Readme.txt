g++ readerTestR6.cpp -o connect.out -I ./ -L ./ -L ./libusb-1.0.23/lib/ -lmwReader -Wl,-rpath=./ -lusb-1.0


g++ readerQR.cpp -o qrcode.out -I ./ -L ./ -L ./libusb-1.0.23/lib/ -lmwReader -Wl,-rpath=./ -lusb-1.0


g++ readerNFC.cpp -o nfc.out -I ./ -L ./ -L ./libusb-1.0.23/lib/ -lmwReader -Wl,-rpath=./ -lusb-1.0


g++ readerSScard.cpp -o sscard.out -I ./ -L ./ -L ./libusb-1.0.23/lib/ -lmwReader -Wl,-rpath=./ -lusb-1.0 


g++ readerBankNum.cpp -o banknum.out -I ./ -L ./ -L ./libusb-1.0.23/lib/ -lmwReader -Wl,-rpath=./ -lusb-1.0