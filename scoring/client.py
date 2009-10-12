#!/usr/bin/env python
"""
    CSAW '09 Reversing Challenge
        Player Interface
"""

from cmd import *
import os
import sys
import pprint as pp 
import Pyro.core
import threading
import getpass
import md5
 
class BaseUI(Cmd):
    """

        The base User Interface object.

    """
    path =[] #this is how we fake the "path" of commands.
    name = ""

    def __init__(self):
        Cmd.__init__(self)

    def make_prompt(self, name=""):
        test_str = self.get_prompt()
        if test_str.endswith(name+"."):
            test_str += ">> "
            return(test_str)
        #the above is a little hack to test if the path
        #is already set for us, incase this object instance
        #is actually getting reused under the hood.
        self.path.append(name)
        tmp_name = ""
        tmp_name = self.get_prompt()
        tmp_name += ">> "
        return(tmp_name)

    def get_prompt(self):
        tmp_name = ""
        for x in self.path: #iterate through object heirarchy
            tmp_name += (x + ".")
        return tmp_name

    def do_help(self, args):
        """
           Getting help on "help" is kinda retarded dont you think?
        """
        #The only reason to define this method is for the help text in the
        #docstring   
        Cmd.do_help(self, args)

    def do_hist(self, args):
        """

            Display command history. 

        """
#        n = 0
#        for i in self._hist:
#            print "%d: %s" % (n, i)
#            n+=1 
        pp.pprint(self._hist)

    def emptyline(self):
        """
            Do nothing on empty input line
        """
        pass
    def preloop(self):
        """
            Initialization before prompting user for commands.
            Despite the claims in the Cmd documentaion, Cmd.preloop() is not a
            stub.
        """
        Cmd.preloop(self)   ## sets up command completion
        self._hist    = []      ## No history yet
        self._locals  = {}      ## Initialize execution namespace for user
        self._globals = {}

    def postloop(self):
        """
            Take care of any unfinished business.
            Despite the claims in the Cmd documentaion, Cmd.postloop() is not a
            stub.
        """
        Cmd.postloop(self)   ## Clean up command completion
        print "\nExiting..."

    def precmd(self, line):
        """ 
            This method is called after the line has been input but before
            it has been interpreted. If you want to modifdy the input line
            before execution (for example, variable substitution) do it here.

        """
        self._hist+=[line.strip()]
        return line

    def postcmd(self, stop, line):
        """
            If you want to stop the console, return something that evaluates to
            true. If you want to do some post command processing, do it here.

        """
        return stop

    def default(self, line):
        """
            Called on an input line when the command prefix is not recognized.
            In that case we execute the line as Python code.

        """
        print "\nBad command or filename.'"

    def do_exit(self, args):
        """
            Exit this level of the CLI. If you are in a submenu
            it will "cd .." to the parent menu.
            If you are in the main menu, it will exit.

        """
        return -1
    
    do_EOF = do_exit

class flagUI(BaseUI):
    """ Flag User Interface """
    def __init__(self, parent, intro):
        BaseUI.__init__(self)
        self.prompt = self.make_prompt("FlagUI")
        self.intro = """\n\n...oooOOO Welcome to the CSAW 2009 Flag Interface OOOooo...\n\n"""
        self.undoc_header = ""
        self.doc_header = ""
        self.p = parent
 
    def do_check(self, *args):
        """
Check a Flag
        
        So you think you have a flag? With this command you can check
        the correctness of the flag before you actually submit it.

        """
        if not self.p._loggedin():
            return 
        flag = raw_input("Please enter the flag: ")
        retval = self.p.rmi.flag_check(flag)
        if retval is True:
            print """
    .-'"'-.
   / #     \     
  : #       :  .-'"'-.
   \       /  / #     \ 
    \     /  : #       :
     `'q'`    \       /
       (       \     /
        )       `'p'`   CONGRATULATIONS! Thats a FLAG!
       (          )
        )        (
                  )
"""
        else:
            print """
          .-''''''-.
        .'          '.
       /   O      O   \ 
      :           `    :
      |                |  UNFORTUNATELY THAT ISNT A FLAG.
      :    .------.    :   KEEP TRYING THOUGH, YOU'LL GET IT!
       \  '        '  /
        '.          .'
          '-......-'
"""
            
    def do_list(self, *args):
        """
Display Challanges

        This command displays the names of each challenge
        and their difficulty/point value.

        """
        if not self.p._loggedin():
            return
        challenges = self.p.rmi.list_challenges()
        print ".....oooooOOOOO CHALLENGES OOOOOooooo.....\n"
        i = 1 
        for chal in challenges.keys():
            print "\t",chal, "(",challenges[chal][0],"points )"
            i+=1
        print "\n.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o"
    
    def do_get_info(self, *args):
        """
Get Challenge Info

        With this you can read the "backstory" of each challenge to
        better understand what the challenge is and what the objective
        is.

        """
        if not self.p._loggedin():
            return 
        challenges = self.p.rmi.list_challenges()
        print "\n\n.....oooooOOOOO CHALLENGES OOOOOooooo.....\n"
        i = 1 
        for chal in challenges.keys():
            print "\t",str(i), ".",chal, "(",challenges[chal][0],"points )"
            i+=1
        print "\n.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o"

        choice = raw_input("Which challenge did you want info on? ")
        if choice.isdigit():
            choice=int(choice)
            choice+=-1
            if choice in range(len(challenges.keys())):
                print "Challenge Name: ", challenges.keys()[choice]
                print "Challenge Value: ", str(challenges[challenges.keys()[choice]][0])
                print "Challenge Info:\n+=+=+=+=+=+\n", challenges[challenges.keys()[choice]][1], "\n+=+=+=+=+=+"
            else:
                print "Please enter the corresponding number of the challenge."
        else:
            print "Please enter the corresponding number of the challenge."

    def do_submit(self, flag):
        """
Submit a Challenge Flag

        With this command you will submit a flag for a challenge.
        The system will then add the appropriate amount of points
        to your player/team scorecard. 

        !!! WARNING !!!
        Be sure you are really ready to do this, it can NOT be undone.
        You will be penalized for incorrect flag submissions!

        """
        if not self.p._loggedin():
            return 
        challenges = self.p.rmi.list_challenges()
        print "\n\n.....oooooOOOOO CHALLENGES OOOOOooooo.....\n"
        i = 1 
        for chal in challenges.keys():
            print "\t",str(i), ".",chal, "(",challenges[chal][0],"points )"
            i+=1
        print "\n.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o.o"

        choice = raw_input("Which challenge are you submitting a flag for? ")
        if choice.isdigit():
            choice=int(choice)
            choice+=-1
            if choice in range(len(challenges.keys())):
                flag = raw_input("Ok, please enter the flag: ")
                retval = self.p.rmi.submit(self.p.uname,self.p.passwdh,challenges.keys()[choice],flag)
		if retval == 3:
			print "You have already submitted that flag!"
                if retval == 2:
                    print "Something is busticated, somehow that request sent incorrect credentials, within the client this should never happen."
                if retval == 1:
                    print "That is an incorrect flag! Please use the 'check' command."
                elif retval == 0:
                    print """
Success! 
Your Flag was received,it was correct, and
you have been alloted the requisite points! 
Use the 'scoreboard' command to verify. 
Congratulations!
"""
                    return 0
                     
            else:
                print "Please enter the corresponding number of the challenge."
        else:
            print "Please enter the corresponding number of the challenge."
          
class MessageBoardUI(BaseUI):
    """ MessageBoard """
    def __init__(self, parent, intro):
        BaseUI.__init__(self)
        self.p = parent
        self.prompt = self.make_prompt("messageboard")
        self.intro = """\n\n...oooOOO Welcome to the CSAW 2009 MESSAGE BOARD OOOooo...\n\n"""
        self.doc_header = "" 
        self.undoc_header = ""
 
    def do_post(self, *args):
        """
Post a message to the message board

        """
        if not self.p._loggedin():
            return 
        message = raw_input("Enter a one-line message to post> ");
        if len(message) > 256:
            shortmessage = message[:256]
            shortmessage += "... [TRUNCATED]"
            message = shortmessage
        self.p.rmi.post_message(self.p.uname, self.p.passwdh, message)

    def do_read(self, *args):
        """
Read 20 most recent messages posted to the message board
        """
        if not self.p._loggedin():
            return 
        print self.p.rmi.show_messages()
        
class MasterUI(BaseUI):
    """ MasterUI """

    def __init__(self, prompt, intro):
        BaseUI.__init__(self)
        self.prompt = self.make_prompt(prompt)
        self.intro = intro
        self.doc_header = "...oooOOO Player/Team UI Commands (for help, type: help <command>) OOOooo..."
        self.undoc_header = ""
        self.misc_header = ""
        self.ruler = "-"
        self.loggedin = False
        self.uname = ""
        self.passwd = ""
        self.passwdh = ""
        self.rmi = None
 
    def do_register(self, *args):
        """
Register a new player or team.

        This is the first step to participating in the
    game. With this command you will create a teamname or username which this client 
    will use to communicate with the Score Server. If you have already
    registered at: 

        http://www.poly.edu/csaw-CTF

    then that is great. (If you haven't it's no big deal) 
    Please use the same name (or something similar) to
    the one you registered on the CSAW site.
   
        """
        print ("Please enter the desired Login Name.")
        uname = raw_input("Login: ")
        uname = uname.strip()
        if len(uname) > 40:
            print "Please choose a shorter login name kthnx!"
            return
        if len(uname) <= 1:
            print "Ok funny guy, choose a longer username kthnx!"
            return
        self._get_rmi()
        if(self.rmi.check_uname_availability(uname)):
            print "That username exists, please retry 'register' command."
            return
        passwd = getpass.getpass("Password: ")
        passwd2 = getpass.getpass("Please reconfirm password: ")
        if passwd != passwd2:
            print ("Password mismatch, please retry 'register' command.")
            return
        tmp = md5.md5();tmp.update(passwd); 
        descr = raw_input("Please enter one-line description of team (school/motto/organization/etc.): ")
        retval = self.rmi.register(uname,tmp.hexdigest(),descr)
        print "Account Registered. Please use the 'login' command now."

    def do_login(self, *args):
        """
Login as an existing player or team.

        Once you have successfully registered, use this
    command to login and begin using the interface.

        """
        uname = raw_input("Login: ")
        uname = uname.strip()
        passwd = getpass.getpass("Password: ")
        tmp = md5.md5();tmp.update(passwd);
        self._get_rmi()
        retval = self.rmi.login(uname,tmp.hexdigest())
        if retval == 1:
            print "Login Failed! That user does not exist, please 'register'."
        elif retval == 2:
            print "Incorrect password."
        elif retval == 0:
            print "User %s successfully logged in!" % (uname)
            self.uname = uname
            self.passwd = passwd
            self.loggedin = True
            self.passwdh = tmp.hexdigest()
            self.prompt = "CSAW09["+uname+"]>>"
            self.path = ["CSAW09["+uname+"]"]

#disabled for CSAW
    def messageboard(self, *args):
        """
Enter the Messageboard User Interface.

        On the messageboard you may post or view messages.
        """
        if not self._loggedin():
            return    
        mb = MessageBoardUI(self, "Welcome to the Messageboard User Interface")  
        mb.cmdloop()
              
    def do_players(self, *args):
        """
List Players

    With this command you can see who has registered to play the game.
    (bascially the scoreboard without scores, and thus pointless)
        """
        if not self._loggedin():
            return    
        print self.rmi.players()
 
    def do_flags(self, *args):
        """
Enter the "Flags" user interface.

        In this user interface you can:

            1. Check Flags for correctness
            2. Submit flags to gain points.
            3. Get information about each challenge and it's flag.
   
        """
        if not self._loggedin():
            return    
        flagui = flagUI(self, "")  
        flagui.cmdloop()

    def do_scoreboard(self, *args):
        """
Display the Scoreboard
    
        Check the progress of all players.
        """
        if not self._loggedin():
            return    
        print self.rmi.scoreboard()
        
    def _loggedin(self):
        """
            Check to see if the user provided username and password.
        """
        if self.loggedin == False:
            print "\nYou are not logged in.\nPlease first use the 'login' command,\nor 'register' command if you have not yet created an account." 
            return False
        else:
            return True

    def _get_rmi(self):
        self.rmi = Pyro.core.getProxyForURI("PYROLOC://127.0.0.1:7766/csaw-ctf")    
 
if __name__ == '__main__':
    scorecli = """

            ...oooOOO Welcome to OOOooo...
  ___ ___   ___      __   ___ _____ ___    __  ___ 
 / __/ __| /_\ \    / /  / __|_   _| __|  /  \/ _ \\
| (__\__ \/ _ \ \/\/ /  | (__  | | | _|  | () \_, /
 \___|___/_/ \_\_/\_/    \___| |_| |_|    \__/ /_/ 
    
                Player/Team Scoreboard
                      Interface 


to get started issue the command 'help register'.
"""
    MasterUI("CSAW09 Player CLI", scorecli).cmdloop()
else:
    print __doc__
