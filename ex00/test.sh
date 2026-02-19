#!/usr/bin/env bash
set -e

bin="./convert"

if [ ! -x "$bin" ]; then
  echo "❌ Binary '$bin' not found. Run: make"
  exit 1
fi

runCase() {
  local literal="$1"

  echo "=============================="
  echo "Input: $literal"
  echo "------------------------------"
  "$bin" "$literal"
  echo
}

tests=(
  # char
  "a"
  "'a'"
  "0"
  "'0'"

  # int
  "42"
  "-42"
  "+42"
  "2147483647"
  "-2147483648"
  "2147483648"
  "-2147483649"

  # double
  "42.0"
  "-42.5"
  "0.0"
  "127.0"
  "128.0"

  # float
  "42.0f"
  "-42.5f"
  "0.0f"

  # pseudo literals
  "nan"
  "nanf"
  "+inf"
  "-inf"
  "+inff"
  "-inff"

  # invalid
  "abc"
  "42ff"
  "++1"
  "--1"
  "1.2.3"
)

echo "🧪 Running ${#tests[@]} tests..."
for literal in "${tests[@]}"; do
  runCase "$literal"
done

echo "✅ Done."
