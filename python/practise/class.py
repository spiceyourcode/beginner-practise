class Person:
    def __init__(self,name):
        self.name=name
    def talk(self):
        print(f"my name is {self.name} and I can code!")
#2 break lines are necessary            
frank= Person("frankline")
frank.talk()     

nelly=Person("Nelly mogere")
nelly.talk()

# this shows inheritance 
class Mammal:
    def walk(self):
        print("A mammal can walk")
    def reproduce(self):
        print("a Mammal can reproduce ")    
        
class Dog(Mammal):
    pass


class Cat(Mammal):
    pass      


bosco= Dog()
bosco.reproduce()