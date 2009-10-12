 ___      _      _            _   _ 
/ __|_ __(_)_ _ | |   ___  __| |_| |
\__ \ '_ \ | ' \| |__/ _ \/ _| / /_|
|___/ .__/_|_||_|____\___/\__|_\_(_)
    |_|                             
        CHALLENGE #7
            700pts

...oooOOO Backstory OOOooo...

    You were contacted by a worried system administrator.
He noticed about 2 weeks ago that someone had compromised
one of his domain controllers. He began monitoring suspicious
traffic going in and out of the machine. He noted that
all the suspicious traffic was on TCP port 7777. When
he attempted to connect back to the attackers machine
late one night, he noticed a flurry of activity on the 
domain controller and then the attacker was gone.
All that is left is a excerpt of traffic captures which 
he has provided to you, and a single dll which you suspect
contains the encryption and decryption routines for 
the network communication of the trojan.

When viewing the suspicious dll in a tool like PEView
or Depends.exe you notice that it exports 5 functions:
    Tumbler1
    Tumbler2
    Tumbler3
    Tumbler4 
    and 
    Tumbler5
You suspect that (like a lock) these Tumblers must be strung together
to properly encrypt and decrypt data.


...oooOOO CTF Task OOOooo...
Using the dll provided, and the traffic provided in the packet capture,
decrypt the "ciphertext" to determine what happened on the wire when
the trojan was active. Who knows, maybe there is even a flag in there! :-)

*HINT*: 
    One of your co-workers reversed enough of the dll to determine
that each tumbler took exactly one argument, a pointer to a character buffer.
"With that info" (he said before leaving for vacation) "you can probably just
write a loader or something..."

