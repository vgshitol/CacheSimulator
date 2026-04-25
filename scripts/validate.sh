#!/usr/bin/env bash

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN_PATH="${REPO_ROOT}/code/sim_cache"
TRACE_DIR="${REPO_ROOT}/ValidationTraces"
TRACE_FILE="gcc_trace.txt"
EXPECTED_DIR="${REPO_ROOT}/ValidationResults"
ACTUAL_DIR="${REPO_ROOT}/OutputResults"

declare -a CONFIGS=(
  "16 1024 2 0 0 0"
  "16 1024 2 16 0 0"
  "16 1024 2 0 8192 4"
  "16 1024 2 16 8192 4"
  "16 1024 1 0 0 0"
  "16 1024 1 16 0 0"
  "16 1024 1 0 8192 4"
  "16 1024 1 16 8192 4"
)

echo "Building simulator..."
make -C "${REPO_ROOT}/code" all >/dev/null

mkdir -p "${ACTUAL_DIR}"

pass_count=0
for i in "${!CONFIGS[@]}"; do
  actual_file="${ACTUAL_DIR}/output${i}.txt"
  expected_file="${EXPECTED_DIR}/gcc.output${i}.txt"

  (
    cd "${TRACE_DIR}"
    "${BIN_PATH}" ${CONFIGS[$i]} "${TRACE_FILE}" > "${actual_file}"
  )

  expected_normalized="$(mktemp)"
  actual_normalized="$(mktemp)"

  sed -n '/^===== L1 contents =====/,$p' "${expected_file}" > "${expected_normalized}"
  sed -n '/^===== L1 contents =====/,$p' "${actual_file}" > "${actual_normalized}"

  if diff -u "${expected_normalized}" "${actual_normalized}" >/dev/null; then
    echo "[PASS] case ${i}"
    pass_count=$((pass_count + 1))
  else
    echo "[FAIL] case ${i} (see ${actual_file})"
    diff -u "${expected_normalized}" "${actual_normalized}" || true
    rm -f "${expected_normalized}" "${actual_normalized}"
    exit 1
  fi

  rm -f "${expected_normalized}" "${actual_normalized}"
done

echo "Validation complete: ${pass_count}/${#CONFIGS[@]} cases passed."
