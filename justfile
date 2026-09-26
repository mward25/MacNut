build-dir:
		mkdir -p build

build: build-dir
		cmake -S . -B ./build -G Ninja && cmake --build ./build
