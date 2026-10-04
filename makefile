.PHONY: all configure build run clean

# Default target
all: build

# Configure CMake
configure:
	cmake -S . -B build

# Build the project
build: configure
	cmake --build build

# Run an executable
run: build
	@if [ -z "$(filter-out run,$(MAKECMDGOALS))" ]; then \
		echo "Usage: make run <executable>"; \
		exit 1; \
	fi
	./build/$(filter-out run,$(MAKECMDGOALS))


# Run examples
run-example-%: build
	./build/examples/$*

# Clean build directory
clean:
	rm -rf build

# Allow arbitrary executable names after "run"
%:
	@: