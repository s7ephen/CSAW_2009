   _            __   __          __  __        __  __                ___ 
  /_\  _ _ ___  \ \ / /__ _  _  |  \/  |_  _  |  \/  |__ _ _ __  __ |__ \
 / _ \| '_/ -_)  \ V / _ \ || | | |\/| | || | | |\/| / _` | '  \/ _` |/_/
/_/ \_\_| \___|   |_|\___/\_,_| |_|  |_|\_, | |_|  |_\__,_|_|_|_\__,_(_) 
                                        |__/                             
                        Challenge #5
                          600 pts

...oooOOO Backstory OOOooo...
    This dll was found by a forensics team on a compromised computer.
You are fairly certain it is part of the attackers toolkit. The
forensics team resports that the attacker attempted to "clean his
tracks" by deleting a number of files from the filesystem. He forgot
this one however. The good thing is that all 'core functionality' of
the attacker's toolkit seems to be in this dll. A guy from the forensics
department said he wrote a small executable (loader) to LoadLibraryA() the dll.
He says he suspects the dll just needs to be LoadLibraryA() by a process
of the "correct" name.

...oooOOO CTF Task OOOooo...
    Get the process to the point where it will display the flag to you,
or simply reverse the flag out of the executable.
This may mean you need to wrte a loader (just something that does LoadLibraryA() of the dll) 
to see what the dll has to say....figure out what the filename of your loader should
be to "unlock" the functionality in the dll. 


