# This file is only used for development for convinience functions as
# quick builds and tests

GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always --tags)
.PHONY: build

build:
	mkdir -p build
	cd build && cmake -DCMAKE_BUILD_TYPE=Debug ../src && make -j$(nproc)

# build-docker:
#	cd docker && make all

#coverage: test
#	mkdir -p reports
#	gcovr -r . -d -e "build" -e "src/test" -e "src/client" -e "src/server" -e "src/generator/generate.cpp" --html --html-details -o reports/coverage.html

format:
	find . -name "*.h" -o -name "*.cpp" -exec clang-format -style=LLVM -i {} \;

check-format: format
	git diff --exit-code

tarball:
	git archive --format=tar.gz --prefix=libjson-rpc-cpp-$(GIT_VERSION)/ -o libjson-rpc-cpp-$(GIT_VERSION).tar.gz HEAD

signed-tarball: tarball
	gpg --armor --detach-sign libjson-rpc-cpp-$(GIT_VERSION).tar.gz

test: build
	cd build &&	make -j$(nproc) && ./bin/unit_testsuite

clean:
	rm -rf build
	rm -rf reports
