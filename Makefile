.PHONY: fmt
fmt: fmt-cc

.PHONY: fmt-cc
fmt-cc:
	@echo "Formatting C/C++ files..."
	@find . -type f \
		\( -name '*.c' -o -name '*.cpp' -o -name '*.h' \) \
		-not -path '*lib*' \
		-not -path '*.git*' \
		-exec clang-format -i {} '+'
