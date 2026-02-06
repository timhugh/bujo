vcpkg_toolchain ?= "$$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

.PHONY: configure build clean

configure:
	@cmake -B build -S . \
		-DCMAKE_TOOLCHAIN_FILE=$(vcpkg_toolchain)

build:
	@cmake --build build

clean:
	@rm -rf build
