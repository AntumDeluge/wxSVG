##############################################################################
## Name:        genFile.py
## Purpose:     saves generated files
## Author:      Alex Thuering
## Created:     2005/01/19
## RCS-ID:      $Id: genFile.py,v 1.2 2005-05-16 11:00:17 ntalex Exp $
## Copyright:   (c) 2005 Alex Thuering
## Notes:       some modules adapted from svgl project
##############################################################################

class genFile:
    def __init__(self, filename):
        self.filename = filename
        self.content = ""
        self.closed=0

    def __del__(self):
        if self.closed==0:
            self.close()

    def close(self):
        if(self.closed==1):
            raise self.filename+' already closed'
        writeit=0
        prevContent = None
        try:
            f = open(self.filename, 'r')
            prevContent = f.read()
            if(prevContent != self.content):
                #print 'different'
                writeit=1
            else:
                #print self.filename + " unchanged"
                pass
            f.close()
        except IOError:
            #print "can't open"
            writeit=1

        if(writeit==1):
            print self.filename + " changed"
            if(prevContent):
                f = open(self.filename+'.prev', 'w')
                f.write(prevContent)
                f.close()
            f = open(self.filename, 'w')
            f.write(self.content)
            
        f.close()
        self.closed=1

    def write(self, s):
        self.content = self.content + s

def gfopen(filename, rw='w'):
    return genFile(filename)

