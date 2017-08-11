cinst -y mingw
cinst -y make
cinst -y msys2
cinst -y 7zip.install
cinst -y wget
set "PATH=%PATH%;C:\tools\mingw64\bin\"

refreshenv

wget https://netcologne.dl.sourceforge.net/project/libusb/libusb-1.0/libusb-1.0.21/libusb-1.0.21.7z -OutFile libusb-1.0.21.7z
mkdir libusb-1.0.21
Get-ChildItem libusb-1.0.21.7z | % {& "C:\Program Files\7-Zip\7z.exe" "x" $_.fullname "-olibusb-1.0.21"}
Copy-Item libusb-1.0.21/MinGW64/static/libusb-1.0.a C:\tools\mingw64\x86_64-w64-mingw32\lib\
New-Item C:\tools\mingw64\x86_64-w64-mingw32\include\libusb-1.0 -type directory
Copy-Item libusb-1.0.21/include/libusb-1.0/libusb.h C:\tools\mingw64\x86_64-w64-mingw32\include\libusb-1.0\
rm libusb-1.0.21.7z
rm -R libusb-1.0.21
