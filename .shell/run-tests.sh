#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

declare SCRIPT_DIR
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly SCRIPT_DIR

declare PROJECT_DIR
PROJECT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
readonly PROJECT_DIR

readonly BUILD_DIR="${PROJECT_DIR}/build"
readonly BINARY="${BUILD_DIR}/tests"

readonly COLOR_RESET='\033[0m'
readonly COLOR_GREEN='\033[0;32m'
readonly COLOR_RED='\033[0;31m'
readonly COLOR_CYAN='\033[0;36m'
readonly COLOR_YELLOW='\033[0;33m'
readonly COLOR_BOLD='\033[1m'

print_header() {
	echo -e "\n${COLOR_BOLD}${COLOR_CYAN}=== ${1} ===${COLOR_RESET}\n"
}

print_success() {
	echo -e "${COLOR_GREEN}[PASS]${COLOR_RESET} ${1}"
}

print_failure() {
	echo -e "${COLOR_RED}[FAIL]${COLOR_RESET} ${1}"
}

print_info() {
	echo -e "${COLOR_YELLOW}[INFO]${COLOR_RESET} ${1}"
}

validate_environment() {
	if [[ ! -d "${BUILD_DIR}" ]]; then
		echo -e "${COLOR_RED}Error:${COLOR_RESET} build directory not found: ${BUILD_DIR}" >&2
		echo "Run cmake -B build first." >&2
		exit 1
	fi

	if ! command -v cmake &>/dev/null; then
		echo -e "${COLOR_RED}Error:${COLOR_RESET} cmake not found in PATH." >&2
		exit 1
	fi
}

build_project() {
	print_header "Build"
	print_info "Building project in ${BUILD_DIR}"

	if cmake --build "${BUILD_DIR}" --parallel; then
		print_success "Build completed."
	else
		print_failure "Build failed. Aborting."
		exit 1
	fi
}

validate_binary() {
	if [[ ! -f "${BINARY}" ]]; then
		echo -e "${COLOR_RED}Error:${COLOR_RESET} test binary not found: ${BINARY}" >&2
		exit 1
	fi

	if [[ ! -x "${BINARY}" ]]; then
		echo -e "${COLOR_RED}Error:${COLOR_RESET} test binary is not executable: ${BINARY}" >&2
		exit 1
	fi
}

run_tests() {
	print_header "Test Suites"

	local -r suites=(
		"FractionCorrectnessTest"
		"FractionPerformanceTest"
		"SquareRootTest"
		"SquareRootBenchmark"
	)

	local passed=0
	local failed=0
	local failed_suites=()

	for suite in "${suites[@]}"; do
		echo -e "${COLOR_BOLD}Running: ${suite}${COLOR_RESET}"

		if "${BINARY}" \
			--gtest_filter="${suite}.*" \
			--gtest_color=yes \
			--gtest_print_time=1; then
			print_success "${suite}"
			((passed++)) || true
		else
			print_failure "${suite}"
			((failed++)) || true
			failed_suites+=("${suite}")
		fi

		echo ""
	done

	print_header "Summary"
	echo -e "  Suites run    : ${#suites[@]}"
	echo -e "  ${COLOR_GREEN}Passed${COLOR_RESET}        : ${passed}"

	if [[ ${failed} -gt 0 ]]; then
		echo -e "  ${COLOR_RED}Failed${COLOR_RESET}        : ${failed}"
		echo ""
		echo -e "${COLOR_RED}Failed suites:${COLOR_RESET}"
		for s in "${failed_suites[@]}"; do
			echo -e "  - ${s}"
		done
		echo ""
		exit 1
	else
		echo ""
		echo -e "${COLOR_GREEN}${COLOR_BOLD}All suites passed.${COLOR_RESET}"
	fi
}

main() {
	validate_environment
	build_project
	validate_binary
	run_tests
}

main
