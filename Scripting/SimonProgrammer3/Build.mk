# SimonProgrammer 3 is a docker-compose challenge (Flask app speaking HTTP over
# its raw TCP port). No PwnableHarness TARGET; this only registers the check.
#
# The original solver (solver_folder/wav_file_solver.sh) needs aubio-tools + jq +
# curl, which are not installed in the `pwnmake check` image, so the registered
# check runs solver_folder/solver.py, a stdlib-only Python port. Downloading and
# analyzing 100 wav files takes a while, so bump the per-solver timeout.
#
# `pwnmake check` starts it, runs the solver, and tears it down.
CHECK_TIMEOUT := 400
$(call ctf_check_tcp_slow,$(DIR),23203,python3 solver_folder/solver.py)
