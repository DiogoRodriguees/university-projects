from automata.pda.npda import NPDA
from sys import argv, exit

# verifica se a quantidade de parametros esta correta
if(len(argv) != 2):
        print("Insira uma expressão")
        exit()
        
# recebendo a expressão da entrada        
entrada = argv[1]

npda = NPDA(
    # estados
    states={'INITIAL_STATE', 'FINAL_STATE'},
    
    # alfabeto de entrada
    input_symbols={'i', 'd', '+', '-', '/', '*', '(', ')', '$', '='},
    
    # transições
    stack_symbols={},
    
    # estado inicial
    initial_state='INITIAL_STATE',
    
    # estado inicial da pilha
    initial_stack_symbol='0',
    
    # estado final
    final_states={'EXPRESSAO_ACEIT'},
    
    # modulo atingido pelo estado "aceito"
    acceptance_mode='FINAL_STATE'
    
)


if npda.acceptance_mode(entrada + "$"):
    print(f"Expressao {entrada} : Aceita!")
else:
    print(f"Expressao {entrada} : Rejeitada!")

