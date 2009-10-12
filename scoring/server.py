#!/usr/bin/env python
import Pyro.core
import threading
import pickle
import time

class Overlord(Pyro.core.ObjBase):
    daemon = None
    uri = None
    lock = None
    lock_val = None
 
    def __init__(self):
        self.lock = threading.Lock()
        self.lock_val = 0
        Pyro.core.ObjBase.__init__(self)

    def bootstrap(self):
        """ 
        """
        Pyro.core.initServer()
        self.daemon=Pyro.core.Daemon(host="localhost",port=7766)
        self.uri=self.daemon.connect(Overlord(),"csaw-ctf")
        print "CSAW Scoreboard running on port:",self.daemon.port
        print "The object's uri is:",self.uri
        self.daemon.requestLoop()
   
    def show_messages(self, num=20):
        messages = self._get_messagedb()
        keys = messages.keys()[:num]
        messageboard = ""
        for key in keys:
            messageboard += "\n----------------------\n" + time.ctime(key) + "\n"
            messageboard += "User '"+messages[key][0] + "' said:\n"
            messageboard += "\t"+messages[key][1] + "\n"
            messageboard += "----------------------\n"
        return messageboard

    def flag_check(self, flag):
        flagdb = self._get_flagdb()
	print "Checking flag: ", repr(flag)
#	print repr(flagdb.keys())
        for key in flagdb.keys():
#	    print repr(flagdb[key][1]),repr(flag)
            if flagdb[key][1] == flag:
                return True
	return False

    def list_challenges(self):
        flagdb = self._get_flagdb()
        newdict = {}
        for key in flagdb.keys():
            newdict[key] = [flagdb[key][0], flagdb[key][2]] #point value,story
        return newdict

    def submit(self, uname, passwd, challenge_name, flag):
	userdb = self._get_userdb()
        if self._check_creds(uname,passwd):
            print "bad credentials", uname, passwd
            return 2 #bad credentials
        flagdb = self._get_flagdb()
#        print challenge_name, flag
        if flagdb[challenge_name][1] != flag:
            print "Incorrect flag guess by: ",uname, "at", time.asctime()
            return 1
	if flag in userdb[uname][3]: #this is the list of their flags.
		return 3 #They submitted that flag already.
        else: #correct! now give them the points
            print "User ", uname,"scored",str(flagdb[challenge_name][0]),"points!"
            userdb = self._get_userdb() #possible TOCTOU between get and set.
            userdb[uname][2] += int(flagdb[challenge_name][0])
            userdb[uname][3].append(flag)
            self._set_userdb(userdb) 
            return 0
                     
    def post_message(self, uname, passwd, message):
        if self._check_creds(uname,passwd):
            print "bad credentials", uname, passwd
            return
        messages = self._get_messagedb()
        t = time.time()
        messages[t] = [uname,message]
        self._set_messagedb(messages)
        print uname, " posted message @ ", time.ctime(t)

    def players(self):
        # players keyed on name.
        #[password hash(string), description(string), score(int), keys(list)]  
        userdb = self._get_userdb()
        playerlist = "\n...oooOOO  Players Registered OOOooo...\n"
        i = 1
        for key in userdb.keys():
            playerlist += str(i)+". '"+key+"'\n" #user/teamname
            playerlist += "\t"+userdb[key][1]+"\n" #user/team comment
            i+=1
        playerlist += "\no-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o-o\n"
        return playerlist

    def scoreboard(self):
        # players keyed on name.
        #[password hash(string), description(string), score(int), keys(list)]  
        userdb = self._get_userdb()
        scoreboard = """
                CSAW CTF 2009
 ___                 _                      _ 
/ __| __ ___ _ _ ___| |__  ___  __ _ _ _ __| |
\__ \/ _/ _ \ '_/ -_) '_ \/ _ \/ _` | '_/ _` |
|___/\__\___/_| \___|_.__/\___/\__,_|_| \__,_|

""" #http://www.network-science.de/ascii/ "small"
        for key in userdb.keys():
            #print repr(userdb)
            scoreboard += "\nTeam: "+key+" --- "+"Score: "+str(userdb[key][2])+"\n"
            scoreboard += "\n-----------------------------------"
        return scoreboard

    def check_uname_availability(self, uname):
        userdb = self._get_userdb()
        if userdb.has_key(uname):
            return 1
        else: 
            return 0
 
    def register(self,uname,passwd,descr):
        # user data is: [password hash(string), description(string), score(int), keys(list)]
        if self.check_uname_availability(uname) or (len(uname) > 40):
            return #just in case people figure out how to connect directly
        userdb = self._get_userdb()
        userdb[uname] = [passwd,descr,0,[]] 
        self._set_userdb(userdb)

    def _check_creds(self, uname, passwd):
        userdb = self._get_userdb()
        if userdb.has_key(uname):
            if userdb[uname][0] == passwd:
                return 0 #success
            else:
                return 2 #wrong password
        else:
            return 1 #user does not exist
     
    def login(self,uname,passwd):
        print "Attempted login for user '",uname,"'", passwd
        return self._check_creds(uname,passwd)

    def _get_flagdb(self):
        self.lock.acquire()
        flagdb_fh = open("./flag.db",'r')
        db = pickle.load(flagdb_fh)
        flagdb_fh.close()
        self.lock.release()
        return db
    
    def _get_userdb(self):
        self.lock.acquire()
        userdb_fh = open("./user.db",'r')
        db = pickle.load(userdb_fh)
        userdb_fh.close()
        self.lock.release()
        return db

    def _set_userdb(self, dbobj):
        self.lock.acquire()
        userdb_fh = open("./user.db",'w')
        db = pickle.dump(dbobj,userdb_fh)
        userdb_fh.close()
        self.lock.release()

    def _get_messagedb(self):
        self.lock.acquire()
        messagedb_fh = open("./message.db",'r')
        db = pickle.load(messagedb_fh)
        messagedb_fh.close()
        self.lock.release()
        return db

    def _set_messagedb(self, dbobj):
        self.lock.acquire()
        messagedb_fh = open("./message.db",'w')
        db = pickle.dump(dbobj,messagedb_fh)
        messagedb_fh.close()
        self.lock.release()

if __name__ == '__main__':
    tmpinst = Overlord()
    tmpinst.bootstrap()

