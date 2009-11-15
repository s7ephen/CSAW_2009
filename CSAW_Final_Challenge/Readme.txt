
             _____ _          ___ _      
            |_   _| |_  ___  | __(_)_ _  
              | | | ' \/ -_) | _|| | ' \ 
              |_| |_||_\___| |_| |_|_||_|

                   The Final Challenge
                 (multiple point values)

...oooOOO Backstory OOOooo..

In the Immortal words of Liu Kang from Mortal Kombat:
"[You] have to accept the challenge, or there can be no final kombat."

In this final challenge, there is a malicious kernel driver running on 
one of your client's compromised systems. The driver contains
the core functionality of your attacker's rootkit. The attackers
use a series of "userland" applications to speak to the driver. A 
coworker of yours has "captured" the state of one of the comprimised machines
by building a virtual machine of it. The virtual machine has two snapshot
states: One before the driver is loaded, and another after. Using your keen
reversing skills, figure out where the malicious driver is, and how
to communicate with it. See if you can trick it into divulging a flag. 

...oooOOO CTF Task OOOooo...

    There are many ways to go about this task, all are equally as effective.
You can solve this with *just* your disassembler or you can solve this using your
kernel debugger. You can write your own application to speak to the driver,
or you can use one of the hints. (<-- This is a hint itself I guess ;-) 
Stay creative, and stay positive, even if you've never done kernel stuff before
this isn't beyond you, you can 0wn it.

    Over the course of the time allotted for this challenge, you will be
given hints, each hint more helpful than the last. Most of you probably won't
need them, but they are available to you nonetheless. 

    Lastly, this challenge will be judged qualitatively, so if you aren't
able to obtain the flag, you can still earn points based on
other information you've obtained and your ability to explain your
thought process and/or answer some simple questions. If you are
men of action and few words, the key (if you have it) will speak
volumes for you, and you will be awarded the full point value, without
needing to utter a word.



