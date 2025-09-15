cmake --build build

rm -f ./build/image.ppm
./build/first_image >> ./build/image.ppm