vcpkg_toolchain ?= "$$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

.PHONY: all configure build test clean

all: build

build/CMakeCache.txt: CMakeLists.txt vcpkg.json vcpkg-configuration.json
	@cmake -B build -S . \
		-DCMAKE_TOOLCHAIN_FILE=$(vcpkg_toolchain)
configure: build/CMakeCache.txt

build: build/CMakeCache.txt
	@cmake --build build

test: build
	@ctest --test-dir build --output-on-failure $(TEST_ARGS)

clean:
	@rm -rf build
