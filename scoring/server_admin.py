#!/usr/bin/env python
"""
    CSAW '09 Reversing Challenge
        Administrative Interface
"""

from cmd import *
import os
import sys
import pprint 
import threading
import md5
import pickle
import code
import pprint as pp
 
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
        pp = pprint.PrettyPrinter(indent=4)
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
        try:
            exec(line) in self._locals, self._globals
        except Exception, e:
            #print e.__class__, ":", e
            print "\nBad command (%s)'" % (e)

    def do_exit(self, args):
        """
            Exit/Quit. 
        """
        return -1

    do_EOF = do_exit
 
class MasterUI(BaseUI):
    """ MasterUI """

    def __init__(self, prompt, intro):
        BaseUI.__init__(self)
        self.prompt = self.make_prompt(prompt)
        self.intro = intro
        self.intro = """

            ...oooOOO CSAW Scoreboard Admin Interface OOOooo...
   
            Please keep in mind that you should be careful with this
            especially while the server process is running because
            this interacts directly with the database files.
"""
#        self.doc_header = ""
#        self.undoc_header = ""
        self.misc_header = ""
        self.ruler = "-"

    def do_edit_users(self, *args):
        userdb_fh = open("./user.db","r")
        userdb = pickle.load(userdb_fh)
        userdb_fh.close()
        import pdb;pdb.set_trace()
        self.userdb = userdb
        if ('y' in raw_input("Save the database changes to file? [y/n]").lower()):
            print "Saving the database."
            userdb_fh = open("./user.db","w")
            userdb = pickle.dump(self.userdb,userdb_fh)
            userdb_fh.close()

    def do_edit_flags(self, *args):
        flagdb_fh = open("./flag.db", "r")
        flagdb = pickle.load(flagdb_fh)
        flagdb_fh.close()
        import pdb;pdb.set_trace()
        self.flagdb = flagdb
        if ('y' in raw_input("Save the database changes to file? [y/n]").lower()):
            print "Saving the database."
            flagdb_fh = open("./flag.db", "w")
            flagdb = pickle.dump(self.flagdb,flagdb_fh)
            flagdb_fh.close()

    def do_edit_messages(self, *args):
        messagedb_fh = open("./message.db","r")
        messagedb = pickle.load(messagedb_fh)
        messagedb_fh.close()
        import pdb;pdb.set_trace()
        #self.dropshell(messagedb)
        self.messagedb = messagedb
        if ('y' in raw_input("Save the database changes to file? [y/n]").lower()):
            print "Saving the database."
            messagedb_fh = open("./message.db","w")
            messagedb = pickle.dump(self.messagedb, messagedb_fh)
            messagedb_fh.close()

    def do_init_usersdb(self, *args):
        if ('y' in raw_input("You sure you wanna wipe everything? [y/n]").lower()):
            userdb_fh = open("./user.db","w")
            userdb = pickle.dump({},userdb_fh)
            userdb_fh.close()
            del(userdb_fh)
         
    def do_init_dbs(self, *args):
        print "Initializing all the databases..."
        if ('y' in raw_input("You sure you wanna wipe everything? [y/n]").lower()):
            scoresdb_fh = open("./scores.db", "w")
            scoresdb = pickle.dump({},scoresdb_fh)
            scoresdb_fh.close()
            del(scoresdb_fh)

            flagdb_fh = open("./flag.db", "w")
            flagdb = pickle.dump({},flagdb_fh)
            flagdb_fh.close()
            del(flagdb_fh)

            messagedb_fh = open("./message.db","w")
            messagedb = pickle.dump({}, messagedb_fh)
            messagedb_fh.close()
            del(messagedb_fh)

    def dropshell(self, db):
        """
            Drop to an interactive shell.
        """
        try:
            code.interact("\n\nDropping to Python shell.\nDatabase is in 'db'.", None, locals())
        except KeyboardInterrupt:
            print "Returning to admin interface"
            
if __name__ == '__main__':
    scorecli = """ 
"""
    MasterUI("CSAW09 ADMIN CLI", scorecli).cmdloop()
else:
    print __doc__
