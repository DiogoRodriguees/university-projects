from automata.pda.npda import NPDA
from sys import argv, exit
import os

# Verifica se a quantidade de parâmetros está correta
if len(argv) != 2:
    print("Insira um caminho de arquivo válido")
    exit()

# Recebendo o caminho do arquivo da entrada
caminho_arquivo = argv[1]

# Verifica se a extensão do arquivo é .exp
nome_arquivo, extensao = os.path.splitext(caminho_arquivo)

if extensao != '.exp':
    print("O arquivo deve ter a extensão .exp")
    exit()

npda = NPDA(
    # estados
    states={"q0","q1", "q2","q3","q4","q5","q6","q7","q8","q9","q10","q11","q12","q13","q14","q15","q16","q17","q18","q19", "q20", "q21","q22","q23","q24","FINAL_STATE","EXPRESSAO_ACEITA"},
    
    # alfabeto de entrada
    input_symbols={"i", "d", "n", "u", "m", "+", "-", "/", "*", "(", ")", "$", "="},
    
    # simbolos da pilha
    stack_symbols={ 'E', 'F', 'S', 'T', 'V', 'i', 'd', 'n', 'u', 'm', '+', '-', '*', '/', '=', '(', ')', '0'},
    
    # trantições
    transitions={
        # metodo shif empilha o estado
        # metodo reduce reduz pela regra
        
        'q0':{
            'i':{
                '0':{("q7", ('i', '0'))}
            },
            '(':{
                '0':{('q1', ('(', '0'))}
            },
            'n':{
                '0':{('q8', ('n', '0'))}
            }
        },
        
        'q1':{
            'i':{
                '(':{("q7", ("i", '('))}
            },
            "(":{
                "(":{('q1', ('(', "("))}
            },
            'n':{
                '(':{('q8', ('n', '('))}
            }
        },
        
        'q2':{
            '+':{'E':{('q11', ('+', 'E'))}},
            '-':{'E':{('q12', ('-', 'E'))}},
            
            '$':{'E':{('q4', 'S')}},            
            '':{'E':{('q4', 'S')}},            
        },
        
        'q3':{
            # ')':{'F':{('q5', ('T', '')),('q19', ('T', '')),('q20', ('T', ''))}},
            '':{'F':{('q5', 'T'),('q19', 'T'),('q20', 'T')}},
        },
        
        'q4':{
            '':{'S':{("q4", '')}},
            "$":{'S':{('EXPRESSAO_ACEITA', '')}},
            
        },
        
        'q5':{
            # ')':{'T':{('q2', ('E', '')), ('q9', ('E', '')), ('q23', ('E', ''))}},
            
            '*':{
                'T':{('q13', ('*', 'T'))}
            },
            '/':{
                'T':{('q14', ('/', 'T'))}
            },
            
            '':{'T':{('q2', 'E'), ('q9', 'E'), ('q23', 'E')}},
            
        },
        
        'q6':{
            '=':{
                'V':{('q15', ('=', 'V'))}
            },
            
            '':{'V':{('q3', 'F'), ('q21', 'F'),('q22', 'F')}},
        },
        
        'q7':{
            'd':{'i':{('q16', ('d', 'i'))}}
        },
        
        'q8':{
            'u':{'n':{('q17',( 'u', 'n'))}}
        },
        
        'q9':{
            ')' :{'E':{('q18', (')', 'E'))}},
            '+' :{'E':{('q11', ('+', 'E'))}},
            '-' :{'E':{('q12', ('-', 'E'))}},
        },
        
        'q10':{
            # ')':{'V':{('q3', ('F', '')), ('q21', ('F', '')),('q22', ('F', ''))}},
            '':{'V':{('q3', 'F'), ('q21', 'F'),('q22', 'F')}},
        },
        
        'q11':{
            'i':{
                '+':{("q7",("i", '+'))}
            },
            '(':{
                '+':{('q1', ('(', '+'))}
            },
            'n':{
                '+':{('q8', ('n', '+'))}
            }
        },
        
        'q12':{
            'i':{
                '-':{("q7",("i", '-'))}
            },
            '(':{
                '-':{('q1', ('(', '-'))}
            },
            'n':{
                '-':{('q8', ('n', '-'))}
            }
        },
        
        'q13':{
            'i':{
                '*':{("q7",("i", '*'))}
            },
            '(':{
                '*':{('q16', ('(', '*'))}
            },
            'n':{
                '*':{('q8', ('n', '*'))}
            }
        },
        
        'q14':{
            'i':{
                '/':{("q7",("i", '/'))}
            },
            '(':{
                '/':{('q16', ('(', '/'))}
            },
            'n':{
                '/':{('q8', ('n', '/'))}
            }
        },
        
        'q15':{
            'i':{   
                '=':{("q7",("i", "="))}
            },
            '(':{
                "=":{('q1', ('(', "="))}
            },
            'n':{
                '=':{('q8', ('n', "="))}
            }
        },
        
        'q16':{
            '':{'i':{('q6', 'V'),('q10', 'V')},'d':{('q16', '')}},
        },
        
        'q17':{
            'm':{"u":{('q24', ('m', 'u'))}}
        },
        
        'q18':{
            '':{
                'E':{('q18', '')}, 
                ')':{('q18', '')},
                '(':{('q3', 'F'),('q21', 'F'),('q22', 'F')}},
        },
        
        'q19':{
            '':{'T':{('q19', '')}, '+':{('q19', '')}, 'E':{('q2', 'E'), ('q9', 'E'), ('q23', 'E')}},
        },
        
        'q20':{
            '':{'T':{('q20', '')}, '-':{('q20', '')}, 'E':{('q2', 'E'), ('q23', 'E'), ('q9', 'E')}},
            
            # '$':{'T':{('q20', '')}},
            # '+':{'T':{('q20', '')}},
            # '-':{'T':{('q20', '')}},
            # ')':{'T':{('q20', '')}},
        },
        
        'q21':{
            '':{'*':{('q21', '')},'F':{('q21', '')}, 'T':{('q5', 'T'), ('q19', 'T'), ('q20', 'T')}},
            
            # '$':{'F':{('q21', '')}},
            # '*':{'F':{('q21', '')}},
            # '/':{'F':{('q21', '')}},
            # '+':{'F':{('q21', '')}},
            # '-':{'F':{('q21', '')}},
            # ')':{'F':{('q21', '')}},
        },
        
        'q22':{
            '':{'F':{('q22', '')}, '/':{('q22', '')}, 'T':{('q5', 'T'),  ('q19', 'T'),('q20', 'T')}},
        },
        
        'q23':{
            '+':{'E':{('q11', ('+', "E"))}},
            '-':{'E':{('q12', ('-', "E"))}},
            '':{'E':{('q4', 'S')}}
            # acrscentar reduce2
        },
        
        'q24':{
            '':{'m':{('q24', '')},'u':{('q24', '')},'n':{('q3', 'F'), ('q21', 'F'),('q22', 'F')}},
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

try:
    with open(caminho_arquivo, 'r') as arquivo:
        for linha in arquivo:
            entrada = linha.strip() + "$"  # Adiciona o marcador de fim de entrada ($)
            if npda.accepts_input(entrada):
                print(f"Expressão {linha.strip()}: Aceita!\n")
            else:
                print(f"Expressão {linha.strip()}: Rejeitada!\n")
except FileNotFoundError:
    print("Arquivo não encontrado. Verifique o caminho do arquivo.")
    exit()
