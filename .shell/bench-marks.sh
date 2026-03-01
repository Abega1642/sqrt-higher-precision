#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

# ---------------------------------------------------------------------------
# bench-marks.sh
# Locks CPU environment for reproducible benchmarking, runs the benchmark
# binary, then restores all system settings unconditionally via a trap.
#
# Usage:
#   sudo ./.shell/bench-marks.sh <path-to-built-test-binary>
#
# Example:
#   sudo ./.shell/bench-marks.sh ./build/tests
# ---------------------------------------------------------------------------

readonly BENCHMARK_CORE=2
readonly BENCHMARK_FILTER="SquareRootBenchmark*"

if [[ $EUID -ne 0 ]]; then
	echo "Error: this script must be run as root." >&2
	echo "Usage: sudo ${0} <path-to-test-binary>" >&2
	exit 1
fi

if [[ $# -ne 1 ]]; then
	echo "Error: exactly one argument required." >&2
	echo "Usage: sudo ${0} <path-to-test-binary>" >&2
	exit 1
fi

readonly BINARY="${1}"

if [[ ! -f "${BINARY}" ]]; then
	echo "Error: binary not found: ${BINARY}" >&2
	exit 1
fi

if [[ ! -x "${BINARY}" ]]; then
	echo "Error: binary is not executable: ${BINARY}" >&2
	exit 1
fi

readonly CPU_COUNT
CPU_COUNT=$(nproc)

if [[ "${BENCHMARK_CORE}" -ge "${CPU_COUNT}" ]]; then
	echo "Error: BENCHMARK_CORE=${BENCHMARK_CORE} but only ${CPU_COUNT} CPUs available." >&2
	exit 1
fi

declare -a ORIGINAL_GOVERNORS=()
declare -a ORIGINAL_MIN_FREQS=()
declare -a ORIGINAL_MAX_FREQS=()

for ((i = 0; i < CPU_COUNT; i++)); do
	ORIGINAL_GOVERNORS+=("$(cat "/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_governor" 2>/dev/null || echo "schedutil")")
	ORIGINAL_MIN_FREQS+=("$(cat "/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_min_freq" 2>/dev/null || echo "")")
	ORIGINAL_MAX_FREQS+=("$(cat "/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_max_freq" 2>/dev/null || echo "")")
done

readonly ORIGINAL_ASLR
ORIGINAL_ASLR=$(cat /proc/sys/kernel/randomize_va_space)

readonly ORIGINAL_INTEL_TURBO
ORIGINAL_INTEL_TURBO=$(cat /sys/devices/system/cpu/intel_pstate/no_turbo 2>/dev/null || echo "")

readonly ORIGINAL_AMD_BOOST
ORIGINAL_AMD_BOOST=$(cat /sys/devices/system/cpu/cpufreq/boost 2>/dev/null || echo "")

restore_system() {
	echo ""
	echo "--- Restoring system defaults ---"

	echo "${ORIGINAL_ASLR}" >/proc/sys/kernel/randomize_va_space

	for ((i = 0; i < CPU_COUNT; i++)); do
		local gov_path="/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_governor"
		if [[ -f "${gov_path}" ]]; then
			echo "${ORIGINAL_GOVERNORS[${i}]}" >"${gov_path}"
		fi
		local min_path="/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_min_freq"
		local max_path="/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_max_freq"
		if [[ -n "${ORIGINAL_MIN_FREQS[${i}]}" && -f "${min_path}" ]]; then
			echo "${ORIGINAL_MIN_FREQS[${i}]}" >"${min_path}"
		fi
		if [[ -n "${ORIGINAL_MAX_FREQS[${i}]}" && -f "${max_path}" ]]; then
			echo "${ORIGINAL_MAX_FREQS[${i}]}" >"${max_path}"
		fi
	done

	if [[ -n "${ORIGINAL_INTEL_TURBO}" ]]; then
		echo "${ORIGINAL_INTEL_TURBO}" >/sys/devices/system/cpu/intel_pstate/no_turbo
	fi

	if [[ -n "${ORIGINAL_AMD_BOOST}" ]]; then
		echo "${ORIGINAL_AMD_BOOST}" >/sys/devices/system/cpu/cpufreq/boost
	fi

	echo "System restored."
}

trap restore_system EXIT

echo "--- Locking CPU environment for benchmarking ---"

for ((i = 0; i < CPU_COUNT; i++)); do
	local_gov="/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_governor"
	if [[ -f "${local_gov}" ]]; then
		echo "performance" >"${local_gov}"
	fi
done
echo "Governor: performance"

if [[ -f /sys/devices/system/cpu/intel_pstate/no_turbo ]]; then
	echo 1 >/sys/devices/system/cpu/intel_pstate/no_turbo
	echo "Intel turbo boost: disabled"
fi

if [[ -f /sys/devices/system/cpu/cpufreq/boost ]]; then
	echo 0 >/sys/devices/system/cpu/cpufreq/boost
	echo "AMD boost: disabled"
fi

for ((i = 0; i < CPU_COUNT; i++)); do
	local_max_freq_src="/sys/devices/system/cpu/cpu${i}/cpufreq/cpuinfo_max_freq"
	local_min_path="/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_min_freq"
	local_max_path="/sys/devices/system/cpu/cpu${i}/cpufreq/scaling_max_freq"
	if [[ -f "${local_max_freq_src}" && -f "${local_min_path}" && -f "${local_max_path}" ]]; then
		local_max_freq=$(cat "${local_max_freq_src}")
		echo "${local_max_freq}" >"${local_min_path}"
		echo "${local_max_freq}" >"${local_max_path}"
	fi
done
echo "CPU frequency: locked to hardware maximum"

echo 0 >/proc/sys/kernel/randomize_va_space
echo "ASLR: disabled"

echo ""
echo "--- Running benchmarks on core ${BENCHMARK_CORE} ---"
echo "Binary : ${BINARY}"
echo "Filter : ${BENCHMARK_FILTER}"
echo ""

# chrt -f 50 : FIFO real-time scheduler, priority 50
#   Prevents OS preemption mid-computation.
#   Priority 50 is above normal tasks but below kernel threads (99).
# taskset -c  : Pin to a single core, avoiding migration and cache thrash.
#   Core 2 is chosen to avoid core 0 which handles hardware interrupts.
chrt -f 50 taskset -c "${BENCHMARK_CORE}" \
	"${BINARY}" \
	--gtest_filter="${BENCHMARK_FILTER}" \
	--gtest_color=yes
