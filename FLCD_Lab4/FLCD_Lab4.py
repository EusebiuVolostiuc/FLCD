from sys import displayhook


class FA:
    def __init__(self, Q, E, S, q0, F):
        self.Q = Q
        self.E = E
        self.S = S
        self.q0 = q0
        self.F = F

    def __init__(self, filename):
        with open(filename,"r") as f_in:
            for line in f_in:
                if("Q" in line):
                    self.Q = self.parseSet(line)
                elif("E" in line):
                    self.E = self.parseSet(line)
                elif("q0" in line):
                    self.q0 = line.strip().replace(" ","").split("=")[1]
                elif("F" in line):
                    self.F = self.parseSet(line)
                elif("S" in line):
                    text = line.strip()
                    try:
                        l = f_in.readline()
                        while "}" not in l:
                            text = text + l.strip()
                            l = f_in.readline()
                        text = text + l.strip()
                        self.S = self.parseTransitions(self.parseSet(text))
                    except StopIteration:
                        pass  

    def parseSet(self,line):
        Set = line.strip().split("=")[1].strip()
        elements = Set[1:-1].strip().split(",")
        result = []
        for elem in elements:
            result.append( elem.strip())
        return result

    def parseTransitions(self,transitionsSet):
        transitions = []
        for transitionText in transitionsSet:
            transitionText = transitionText.strip();
            x, tmp= transitionText[1:].split("+")
            x = x.strip()
            y,z = tmp.replace(" ","").split(")->")
            transitions.append(((x,y),z))
        return transitions

    def verifyDFA(self, text):
        currentState = self.q0
        for i in text:
            nextState = None
            for transition in self.S:
                if transition[0] == (currentState,i):
                   nextState = transition[1]
                   break
            if nextState is None:
                return 0
            else: currentState = nextState
        if currentState in self.F:
           return 1
        else: return 0
    
    def getStatesString(self):
        return 'Q = { ' + ', '.join(self.Q) + ' }\n'

    def getAlphabetString(self):
        return 'E = { ' + ', '.join(self.E) + ' }\n'

    def getTransitionsString(self):
        return 'S = {\n' + ',\n'.join(
                   [str("\t" + " + ".join(trans[0]) + " -> " + trans[1]) for trans in self.S]
                   ) + '\n}\n'

    def getInitialStateString(self):
        return 'q0 = ' + str(self.q0) + '\n'

    def getFinalStatesString(self):
        return 'F = { ' + ', '.join(self.F) + ' }\n'

    def __str__(self):
        return 'Q = { ' + ', '.join(self.Q) + ' }\n' \
               + 'E = { ' + ', '.join(self.E) + ' }\n' \
               + 'S = {\n' + ',\n'.join(
                   [str("\t" + " + ".join(trans[0]) + " -> " + trans[1]) for trans in self.S]
                   ) + '\n}\n' \
               + 'q0 = ' + str(self.q0) + '\n' \
               + 'F = { ' + ', '.join(self.F) + ' }' \


def display_menu():
    print("Commands:")
    print("print [param]")
    print("\t''\n\t\tDisplay FA")
    print("\tstates\n\t\tDisplay States")
    print("\talphabet\n\t\tDisplay Alphabet")
    print("\ttransitions\n\t\tDisplay Transitions")
    print("\tinitial\n\t\tDisplay Initial state")
    print("\tfinal\n\t\tDisplay Final states")
    print("check\n\tCheck DFA")
    print("h, help\n\tDisplay menu")
    print("exit\n\tExit")


def main():
    fa = FA("FA.in")

    display_menu()
    while True:
        x = input(">")
        if x == "":
            continue
        if x == "exit":
            break
        elif x == "print":
            print(fa)
        elif x == "print states":
            print(fa.getStatesString())
        elif x == "print alphabet":
            print(fa.getAlphabetString())
        elif x == "print transitions":
            print(fa.getTransitionsString())
        elif x == "print initial":
            print(fa.getInitialStateString())
        elif x == "print final":
            print(fa.getFinalStatesString())
        elif x == "check":
            dfa = input(" DFA: ")
            if fa.verifyDFA(dfa):
                print(f"{dfa} is valid")
            else:
                print(f"{dfa} is unvalid")
        elif x == "h" or x == "help":
            display_menu()
        else:
            print(f'{x} is an invalid command')
            
    
main()