all:
	@cmake --build build

run:
	@cmake --build build --target run

clean:
	@cmake --build build --target clean

.PHONY: all run clean
