# SimonProgrammer 2 is a docker-compose challenge (Flask app speaking HTTP over
# its raw TCP port). No PwnableHarness TARGET; this only registers the check.
# `pwnmake check` starts it, runs the solver, and tears it down.
$(call ctf_check_tcp,$(DIR),23202,python3 solver.py)
