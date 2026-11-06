.PHONY: debug release configure clean 

debug:
	cmake --build build/debug

release:
	cmake --build build/release

configure:
	cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
	cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release

clean:
	rm -rf build