Port Assignments
-----

Port assignment convention:

* SunshineCTF challenges: port = `yycnn`
  * `yy` = last 2 digits of competition year, so SunshineCTF 2023 would be `23cnn`
  * `c` = category (0 = pwn, 1 = reversing, 2 = scripting, 3 = web, 4 = crypto, 5 = misc, 6 = speedrun, 7 = pegasus)
  * `nn` = challenge index, starting at 0

Only challenges with a hosted, server-side component are listed. HTTP(S) web
challenges are reverse-proxied by nginx (see `nginx/`) on their subdomain;
raw-TCP / port-based challenges are reached with `nc ctf.hackucf.org <port>`
(the Scripting challenges speak HTTP over their raw port).

| Challenge Name | Category | Author | Directory | Host | Port | Connection |
|----------------|----------|--------|-----------|------|-----:|------------|
| Robot Assembly Line | Pwn | kcolley | Pwn/RobotAssemblyLine | ctf.hackucf.org | 23000 | `nc ctf.hackucf.org 23000` |
| House of Sus | Pwn | Oreomeister | Pwn/House_of_Sus | ctf.hackucf.org | 23001 | `nc ctf.hackucf.org 23001` |
| flock | Pwn | tj_oconnor | Pwn/flock | ctf.hackucf.org | 23002 | `nc ctf.hackucf.org 23002` |
| Array of Sunshine | Pwn | tj_oconnor | Pwn/array_of_sunshine | ctf.hackucf.org | 23003 | `nc ctf.hackucf.org 23003` |
| Bugspray | Pwn | tj_oconnor | Pwn/bugspray | ctf.hackucf.org | 23004 | `nc ctf.hackucf.org 23004` |
| DDR | Scripting | shuga__ | Scripting/ddr | ctf.hackucf.org | 23200 | `nc ctf.hackucf.org 23200` |
| Simon Programmer 1 | Scripting | solarbonite | Scripting/SimonProgrammer1 | simon-programmer-1.ctf.hackucf.org | 23201 | https://simon-programmer-1.ctf.hackucf.org |
| Simon Programmer 2 | Scripting | solarbonite | Scripting/SimonProgrammer2 | simon-programmer-2.ctf.hackucf.org | 23202 | https://simon-programmer-2.ctf.hackucf.org |
| Simon Programmer 3 | Scripting | solarbonite | Scripting/SimonProgrammer3 | simon-programmer-3.ctf.hackucf.org | 23203 | https://simon-programmer-3.ctf.hackucf.org |
| BeepBoop Blog | Web | Oreomeister | Web/beepboop_blog | beepboop-blog.ctf.hackucf.org | 23300 | https://beepboop-blog.ctf.hackucf.org |
| Hotdog Stand | Web | jck | Web/hotdog_stand | hotdog-stand.ctf.hackucf.org | 23301 | https://hotdog-stand.ctf.hackucf.org |

Not hosted here (no reproducible server-side component):

* `Reversing/Dill` — downloadable `.pyc` reversing challenge; its Dockerfile only
  compiles the distributable attachment (no server).
