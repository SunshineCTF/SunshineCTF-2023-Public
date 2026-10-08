# DDR is a docker-compose challenge (index.py served over a socat TCP socket).
# It has no PwnableHarness TARGET; this Build.mk only registers the check.
#
# `pwnmake check` starts it, runs the solver, and tears it down. The solver plays
# all 255 rounds of the game in real time, which takes a few minutes, so bump the
# per-solver timeout well above the default 120s.
CHECK_TIMEOUT := 400
$(call ctf_check_tcp_slow,$(DIR),23200,python3 src/solve.py)
