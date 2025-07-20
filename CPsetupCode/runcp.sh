# Add this to ~/.bashrc or source it from a script
runcp() {
  local src="$1"
  local infile="$HOME/input_cp.txt"
  local outfile="$HOME/output_cp.txt"
  # derive binary name from source file (e.g. foo.cpp → foo)
  local bin="a.out"

  # compile
  g++ -std=c++17 -O2 "$src" -o "$bin"
  if [ $? -ne 0 ]; then
    echo "Compilation failed. Aborting."
    return 1
  fi

  # run with redirection
  ./"$bin" < "$infile" > "$outfile"
  local rc=$?

  if [ $rc -ne 0 ]; then
    echo "Program exited with code $rc."
  else
    echo "Success! Output written to $outfile."
  fi

  cat "$outfile"
  return $rc
}


