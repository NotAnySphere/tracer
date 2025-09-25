cmake --build build

rm -f ./build/image.png
./build/first_image >> ./build/image.ppm
convert ./build/image.ppm ./build/image.png
rm -f ./build/image.ppm
xviewer ./build/image.png &