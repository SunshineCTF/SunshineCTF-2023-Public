# SimonProgrammer 3 is a docker-compose web challenge (Flask app) served
# behind the reverse proxy at its subdomain. No PwnableHarness TARGET; this
# only registers the check.
#
# The original solver (solver_folder/wav_file_solver.sh) needs aubio-tools + jq +
# curl, which are not installed in the `pwnmake check` image, so the registered
# check runs solver_folder/solver.py, a stdlib-only Python port. Downloading and
# analyzing 100 wav files takes a while, so bump the per-solver timeout.
#
# `pwnmake check` starts it, runs the solver, and tears it down.
CHECK_TIMEOUT := 400
$(call ctf_check_web_slow,$(DIR),23203,simon-programmer-3.ctf.hackucf.org,python3 solver_folder/solver.py)
