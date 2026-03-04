#!/usr/bin/env bash
set -euo pipefail

declare SCRIPT_DIR
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly SCRIPT_DIR

declare -r STYLE="Google"
declare -r EXTENSIONS=("*.cpp" "*.hpp" "*.h")
declare -r EXCLUDE_DIRS=("build" "cmake-build-debug-test" ".git")

if ! command -v clang-format &>/dev/null; then
	echo "Error: clang-format not found in PATH." >&2
	exit 1
fi

build_find_args() {
	local args=("${SCRIPT_DIR}")

	for dir in "${EXCLUDE_DIRS[@]}"; do
		args+=(-path "${SCRIPT_DIR}/${dir}" -prune -o)
	done

	args+=(-type f)

	local first=true
	for ext in "${EXTENSIONS[@]}"; do
		if [[ "${first}" == true ]]; then
			args+=(\( -name "${ext}")
			first=false
		else
			args+=(-o -name "${ext}")
		fi
	done
	args+=(\) -print0)

	printf '%s\0' "${args[@]}"
}

main() {
	local count=0

	while IFS= read -r -d '' file; do
		clang-format -i -style="${STYLE}" "${file}"
		echo "Formatted: ${file}"
		((count++)) || true
	done < <(find "${SCRIPT_DIR}" \
		-path "${SCRIPT_DIR}/build" -prune -o \
		-path "${SCRIPT_DIR}/cmake-build-debug-test" -prune -o \
		-path "${SCRIPT_DIR}/.git" -prune -o \
		-type f \( \
		-name "*.cpp" -o \
		-name "*.hpp" -o \
		-name "*.h" \
		\) -print0)

	echo ""
	echo "Done: ${count} file(s) formatted."
}

main
