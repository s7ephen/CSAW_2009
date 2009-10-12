                        Challenge #3

...oooOOO Backstory OOOooo...
    This executable was set to you by the incident response division.
It was emailed to one of the incident response team members as an attachment
in a very targeted email. Your coworker (the recipient) wasn't foolish enough
to actually run the malware, but he was curious, and loaded it up in his
lab environment. He found that this unique piece of malware somehow finished
running "before" his debugger could even attach, even when started from
the debugger! He sent to you (the expert) for further analysis. 
Some preliminary poking around on the internet
pointed you at something called "TLS" (Thread Local Storage) callbacks.
A coworker then mentioned a program called PEView that he downloaded
that helped to view the sections of a Portable Exacutable. He mentioned
that he remembered seeing a weird .tls entry in the PE Headers.

...oooOOO CTF Task OOOooo...
    Find out what mechanism is making it possible for this malware to execute
before the debugger attaches. Get the process to the point where it will display the flag to you,
or simply reverse the flag out of the executable.


