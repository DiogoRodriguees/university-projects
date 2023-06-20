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
        # metodo shif empilha o estado
        # metodo reduce reduz pela regra
        
        'q0':{
            'i':{
                '':{("q7","i")}
            },
            "(":{
                "":{('q16', '(')}
            },
            'n':{
                '':{('q8', 'n')}
            }
        },
        
        'q1':{
            'i':{
                '':{("q7","i")}
            },
            "(":{
                "":{('q16', '(')}
            },
            'n':{
                '':{('q8', 'n')}
            }
        },
        
        'q2':{
            '+':{'':{('q11', '+')}},
            '-':{'':{('q12', '-')}},
            '$':{'E':{('q2', 'S')}},
            '':{'S':{("")}}
            
            
            
        },
        
        'q3':{
            
        },
        
        'q4':{
            "$":{( 'EXPRESSAO_ACEITA', '')}
        },
        
        'q5':{
            '*':{
                '':{('q13', '*')}
            },
            '/':{
                '':{('q14', '/')}
            }
        },
        
        'q6':{
            '=':{
                '':{('q15', '=')}
            }
        },
        
        'q7':{
            'd':{('q16', 'd')}
        },
        
        'q8':{
            'u':{('q17', 'u')}
        },
        
        'q9':{
            ')':{('q18', ')')},
            '+':{('q11', '+')},
            '/':{('q12', '/')}
            
        },
        
        'q10':{
           
        },
        
        'q11':{
            'i':{
                '':{("q7","i")}
            },
            "(":{
                "":{('q16', '(')}
            },
            'n':{
                '':{('q8', 'n')}
            }
        },
        
        'q12':{
            'i':{
                '':{("q7","i")}
            },
            "(":{
                "":{('q16', '(')}
            },
            'n':{
                '':{('q8', 'n')}
            }
        },
        
        'q13':{
            'i':{
                '':{("q7","i")}
            },
            "(":{
                "":{('q16', '(')}
            },
            'n':{
                '':{('q8', 'n')}
            }
        },
        
        'q14':{
            'i':{
                '':{("q7","i")}
            },
            "(":{
                "":{('q16', '(')}
            },
            'n':{
                '':{('q8', 'n')}
            }
        },
        
        'q15':{
            'i':{
                '':{("q7","i")}
            },
            "(":{
                "":{('q16', '(')}
            },
            'n':{
                '':{('q8', 'n')}
            }
        },
        
        'q16':{

        },
        
        'q17':{
            'm':{('q24', 'm')}
        },
        
        'q18':{
            
        },
        
        'q19':{
            '*':{('q13', '*')},
            '/':{('q14', '/')}
        },
        
        'q20':{
            '*':{('q13', '*')},
            '/':{('q14', '/')}
        },
        
        'q21':{
            
        },
        
        'q22':{
            
        },
        'q23':{
            '+':{('q11', '+')},
            '-':{('q12', '-')}
        },
        'q24':{
            
        },
        
      
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
