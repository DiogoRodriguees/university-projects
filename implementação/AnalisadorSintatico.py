from automata.pda.npda import NPDA
from sys import argv, exit

# verifica se a quantidade de parametros esta correta
if len(argv) != 2:
    print("Insira uma expressão")
    exit()

# recebendo a expressão da entrada
entrada = argv[1]

npda = NPDA(
    # estados
    states={"q0","q1", "q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15","q16","q17","q18","q19", "q20", "q21","q22","q23","q24","FINAL_STATE","EXPRESSAO_ACEITA"},
    
    # alfabeto de entrada
    input_symbols={"i", "d", "n", "u", "m" "+", "-", "/", "*", "(", ")", "$", "="},
    
    # simbolos da pilha
    stack_symbols={ 'E', 'F', 'S', 'T', 'V', 'i', 'd', 'n', 'u', 'm', '+', '-', '*', '/', '=', '(', ')', '0'},
    
    # trantições
    transitions={
        'q0':{
            'i':{
                '0': {('q0', ('i', '0'))}
            },
            'd':{
                'i': {('q5', ('d', 'i'))}
            },
        },
        
        "q1":{
            "$":{
                "S":{("EXPRESSAO_ACEITA", "")}
            }
        },
        
        "q2":{
            "":{
                "E":{('q1', "S")}
            }
        },
        
        'q3':{
            "":{
                "V":{('q6', 'F')}
            },
        },
        'q4':{
            "":{
                "T":{("q2", "E")}
            }
        },
        
        'q5':{
            "":{
                'd': {('q5', '')},
                'i': {('q3', "V")},
            },
        },
        
        'q6':{
            "":{
                "F":{('q4', 'T')}
            }
        }
    
    },
    
    # estado inicial
    initial_state="q0",
    
    # estado inicial da pilha
    initial_stack_symbol="0",
    
    # estado final
    final_states={"EXPRESSAO_ACEITA"},
    
    # modulo atingido pelo estado "aceito"
    acceptance_mode="final_state",
)


if npda.accepts_input(entrada + "$"):
    print(f"Expressao {entrada} : Aceita!")
else:
    print(f"Expressao {entrada} : Rejeitada!")
