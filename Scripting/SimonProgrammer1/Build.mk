# SimonProgrammer 1 is a docker-compose web challenge (Flask app) served
# behind the reverse proxy at its subdomain. No PwnableHarness TARGET; this
# only registers the check. `pwnmake check` starts it, runs the solver, and
# tears it down.
$(call ctf_check_web,$(DIR),23201,simon-programmer-1.ctf.hackucf.org,python3 solver.py)
