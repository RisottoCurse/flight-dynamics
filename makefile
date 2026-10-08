.PHONY: all configure build run clean

# Default target
all: build

# Configure CMake
configure:
	cmake -S . -B build -G Ninja

# Build the project
build: configure
	cmake --build build

# Run an executable
run: build
	@if [ -z "$(filter-out run,$(MAKECMDGOALS))" ]; then \
		echo "Usage: make run <executable>"; \
		exit 1; \
	fi
	@if [ -f "./build/$(filter-out run,$(MAKECMDGOALS))" ]; then \
		./build/$(filter-out run,$(MAKECMDGOALS)); \
	elif [ -f "./build/examples/$(filter-out run,$(MAKECMDGOALS))" ]; then \
		./build/examples/$(filter-out run,$(MAKECMDGOALS)); \
	else \
		echo "Executable not found: $(filter-out run,$(MAKECMDGOALS))"; \
		exit 1; \
	fi


# Run examples
run-example-%: build
	./build/examples/$*

# Clean build directory
clean:
	rm -rf build

# Allow arbitrary executable names after "run"
%:
	@: