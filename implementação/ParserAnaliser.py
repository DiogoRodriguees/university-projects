import sys
def is_accepting(state, accept_states):
    return state in accept_states

def process_input(input_string, states, input_alphabet, stack_alphabet, transitions,start_state, start_symbol, accept_states):
    stack = [start_symbol]
    state = start_stat
    
    print(stack)
    
    for char in input_string:
        current_symbol = stack[-1]
        print(state, char, current_symbol)
        if (state, char, current_symbol) in transitions:
            new_state, new_symbol = transitions[(state, char, current_symbol)]
            stack.pop()     
            
            print(new_state, new_symbol)
            
            if new_symbol != 'epsilon':
                stack.extend(list(new_symbol))
                state = new_state
        else:
            return False
    return is_accepting(state, accept_states) and len(stack) == 1 and stack[0] == start_symbol

if __name__ == "__main__":
    
    states = {'q0', 'q1', 'q2', 'q3', 'q4', 'q5'}
    input_alphabet = {'a', 'b'}
    stack_alphabet = {'A', 'B'}
    transitions = {
                # stado 0
                ('q0', '(', 'epsilon'): ('q1', '(', 'q0'),
                ('q0', 'i', 'epsilon'): ('q7', 'i', 'q0'),
                ('q0', 'd', 'epsilon'): ('q8', 'd', 'q0'),
                ('q0', '', 'V'): ('q6', 'V'),
                ('q6', '', 'V'):('q3')
                
                # estado 1
                ('q1', '(', 'epsilon'): ('q1', '(', 'q1'),
                ('q1', 'i', 'epsilon'): ('q7', 'i', 'q1'),
                ('q1', 'd', 'epsilon'): ('q8', 'd', 'q1'),
                
                # estado 2
                ('q2', '+', 'epsilon'): ('q11', '+', 'q2'),
                ('q2', '-', 'epsilon'): ('q12', '-', 'q2'),
                ('q2', '$', 'epsilon'): ('q4', 'S'),
                
                # estado 3
                ('q3', ')', 'q0'): ('q5', 'T', 'q3'),
                ('q3', '*', 'q1'): ('q5', 'T'),
                ('q3', '/', 'q11'): ('q19', 'T'),
                ('q3', '+', 'q12'): ('q20', 'T'),
                ('q3', '-', 'q15'): ('q5', 'T'),
                
                # estado 4
                ('q4', '$', 'epsilon'): ('acc', ''),
                    
                # estado 5
                ('q5', '*', 'epsilon'): ('q13', '*', 'q5'),
                ('q5', '/', 'epsilon'): ('q14', '/', 'q5'),
                ('q5', ')', 'epsilon'): ('q14', 'E'),
                
                # feito ate aqui
                ('q7', 'd', 'i'): ('q16', 'd'),
                
                ('q16', '$', 'epsilon'): ('q0', 'V'),
                
                
    }
                
    start_state = 'q0'
    start_symbol = 'A'
    accept_states = {'q1'}
    
    if len(sys.argv) < 2 :
        raise IOError("Use "+ sys.argv[0] + " file.exp")
    else :
        aux = sys.argv[1].split('.')
        if aux[-1] != 'exp':
            raise IOError("Not a .exp file!")
        data = open(sys.argv[1])
    
        input_string = data.read()
        input_string = input_string.strip()
    
    print('Testing:', input_string)
    if process_input(input_string, states, input_alphabet, stack_alphabet,
        transitions, start_state, start_symbol, accept_states):
        print("Accepted")
    else:
        print("Rejected")