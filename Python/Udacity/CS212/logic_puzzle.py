"""
Udacity CS212
Author: Zachary Jones <zacharytamas@gmail.com>

UNIT 2: Logic Puzzle

You will write code to solve the following logic puzzle:

# 1. The person who arrived on Wednesday bought the laptop.
# 2. The programmer is not Wilkes.
# 3. Of the programmer and the person who bought the droid,
   one is Wilkes and the other is Hamming. 
# 4. The writer is not Minsky.
# 5. Neither Knuth nor the person who bought the tablet is the manager.
# 6. Knuth arrived the day after Simon.
# 7. The person who arrived on Thursday is not the designer.
# 8. The person who arrived on Friday didn't buy the tablet.
# 9. The designer didn't buy the droid.
# 10. Knuth arrived the day after the manager.
# 11. Of the person who bought the laptop and Wilkes,
    one arrived on Monday and the other is the writer.
# 12. Either the person who bought the iphone or the person who bought the tablet
    arrived on Tuesday.

You will write the function logic_puzzle(), which should return a list of the
names of the people in the order in which they arrive. For example, if they
happen to arrive in alphabetical order, Hamming on Monday, Knuth on Tuesday, etc.,
then you would return:

['Hamming', 'Knuth', 'Minsky', 'Simon', 'Wilkes']

(You can assume that the days mentioned are all in the same week.)
"""

import itertools

PEOPLE = ["Wilkes", "Hamming", "Minsky", "Knuth", "Simon"]

def day_after(order, first_person, second_person):
    """Returns whether the first_person arrived the day
    after the second_person."""
    return (order.index(second_person) + 1) == order.index(first_person)

def logic_puzzle():
    "Return a list of the names of the people, in the order they arrive."
    
    permutations = list(itertools.permutations(range(5)))
    (Monday, Tuesday, Wednesday, Thursday, Friday) = range(5)
    
    for people in itertools.permutations(PEOPLE):
        
        # Rule #6 says that Knuth arrives the day after Simon.
        if not day_after(people, "Knuth", "Simon"):
            continue
        
        for (programmer, writer, manager, designer, _) in permutations:
            
            # Rule #10 says Knuth arrived the day after the manager.
            if not day_after(people, "Knuth", PEOPLE[manager]):
                continue
            
            # Rule #2 says Wilkes is not a programmer.
            if people.index("Wilkes") is programmer:
                continue
            
            # Rule #4 says Minsky is not a writer.
            if people.index("Minsky") is writer:
                continue
            
            # Rule #7 says the designer did not arrive on Thursday.
            if designer == Thursday:
                continue
            
            for (tablet, laptop, droid, iphone, _) in permutations:
                
                # Rule #1 says that the person who arrived Wednesday
                # bought the laptop.
                if laptop is not Wednesday:
                    continue
                
                # Rule #9 says the designer did not buy an android.
                if designer == droid:
                    continue
                
                # Rule #5 says that Knuth and the tablet purchaser
                # are not managers
                if manager in [people.index("Knuth"), tablet]:
                    continue
                
                # Rule #8 says that the person arriving on Friday
                # did not purchase a tablet.
                if tablet == Friday:
                    continue
                
                # Rule #12 says that the person who bought the iPhone
                # OR the person who bought the tablet arrived on Tuesday:
                if Tuesday not in [iphone, tablet]:
                    continue
                
                # Rule #11 says that of the laptop buyer and Wilkes,
                # one arrived on Monday and the other is a writer.
                if not (people.index("Wilkes") == writer and laptop == Monday or
                   people.index("Wilkes") == Monday and writer == laptop):
                   continue
                
                # Rule #3 says that of the programmer and the droid
                # buyer, one is Wilkes and the other is Hamming.
                Hamming = people.index("Hamming")
                Wilkes = people.index("Wilkes")
                if not ((Hamming is programmer and Wilkes is droid) or 
                    (Wilkes is programmer and Hamming is droid)):
                    continue
                
                return list(people)
    
    print "There were no solutions."
    
# print logic_puzzle()

def tests():
    
    assert day_after(["zach", "chelsea"], "zach", "chelsea") == False
    assert day_after(["zach", "chelsea"], "chelsea", "zach") == True

    people = logic_puzzle()
    assert people.index("Knuth") == (people.index("Simon") + 1)
    print "TESTS PASS"

# tests()