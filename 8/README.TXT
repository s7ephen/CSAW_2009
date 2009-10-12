__   __          _                   _             _   ___ _  _  ___  _ 
\ \ / /__ _  _  | |_  __ ___ _____  | |__  __ _ __| | | _ ) || |/ _ \| |
 \ V / _ \ || | | ' \/ _` \ V / -_) | '_ \/ _` / _` | | _ \ __ | (_) |_|
  |_|\___/\_,_| |_||_\__,_|\_/\___| |_.__/\__,_\__,_| |___/_||_|\___/(_)
                                                                        
                        Challenge #8
                           450pts 

...oooOOO Backstory OOOooo...
    This particular piece of malware was found on a number of machines.
It is a banking trojan. It is designed to infect Internet Explorer
and watch for when it is surfing to specific sites. This particular
trojan is implemented as a BHO (or Browser Helper Object). This means
that it is a single dll that is "registered" in a special way to associate
itself with the Internet Explorer executable. Once running, internet explorer
allows BHOs to "hook" (watch for) many different events: when documents start loading; finish
loading; when keys are pressed; when mouse buttons are clicked; etc.
A colleague of yours has provided you with a copy of the dll and batch files to load it with. 
    
...oooOOO CTF Task OOOooo...
Using your reversing skills try to determine which URLs will trigger the 
BHO into action. Then trigger the BHO by surfing to these urls in internet
explorer. (To trigger, you might have to click around on the target site first).
Chances are the BHO trojan's actions will lead you to a flag!

For those unfamiliar with BHO's:
1.    TO "install" the BHO run "regbho.bat".
2.    Then use internet explorer (the bho will be watching you ;-)
3.    TO verify this, go to google.com and enter something into the search bar.
      You should immediately get a popup from Challenge #8 notifying you that
      it is enabled.
4.    To uninstall use "unregbho.bat"

