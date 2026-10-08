
# docker-compose web challenge; `pwnmake check` starts it, runs the solver, and
# tears it down.
$(call ctf_check_web,$(DIR),23301,hotdog-stand.ctf.hackucf.org,python3 solver.py)
