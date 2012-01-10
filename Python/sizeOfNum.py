#
#  sizeOfNum.py
#  April 3rd 2011 -- Zachary Jones
#
#  Given a number and a base to represent it in, this returns the 0-index 
#  length of the resulting number. Basically, tells you how many characters 
#  it's going to take to represent that number in the target base.
#


def sizeOfNum(number, base):
    i = 0;
    j = 1;
    while(number >= j):
        j = j*base
        i+=1
    print i
    
sizeOfNum(25600, 16);