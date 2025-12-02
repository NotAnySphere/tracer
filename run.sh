WRITE_TO_FILE=$1

# cmake --fresh -DSDL_X11_XSCRNSAVER=OFF -DCMAKE_BUILD_TYPE=Debug -B build

cmake --build build -j=10 

./build/first_image "$WRITE_TO_FILE"

if test -n "$WRITE_TO_FILE" ; then
    xviewer ./build/image.bmp
fi