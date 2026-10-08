# BeepBoop Blog is a docker-compose web challenge. No PwnableHarness TARGET; this
# only registers the check. `pwnmake check` starts it, runs the solver, and tears
# it down.
$(call ctf_check_web,$(DIR),23300,beepboop-blog.ctf.hackucf.org,python3 solver.py)
