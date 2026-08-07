import re

def valida_email(email):
    # Define uma expressão regular para validar o formato do e-mail
    padrao = r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$'
    
    # Utiliza a função match para verificar se o e-mail corresponde ao padrão
    if re.match(padrao, email):
        return True
    else:
        return False

def valida_CPF(cpf):
    # Define uma expressão regular para validar o formato do e-mail
    padrao = r'^\d{3}\.\d{3}\.\d{3}-\d{2}$'
    
    # Utiliza a função match para verificar se o e-mail corresponde ao padrão
    if re.match(padrao, cpf): 
        return True
    else:
        return False
        
def valida_matricula(matricula):
    # Define uma expressão regular para validar o formato do e-mail
    padrao = r'^2023\d{5}$'
    
    # Utiliza a função match para verificar se o e-mail corresponde ao padrão
    if re.match(padrao, matricula): 
        return True
    else:
        return False 
        
def valida_nome(nome):
    # Define uma expressão regular para validar o formato do e-mail
    padrao = r'^[A-Z]{1}[a-z ]+[A-Z]{1}[a-z]+$'
    
    # Utiliza a função match para verificar se o e-mail corresponde ao padrão
    if re.match(padrao, nome): 
        return True
    else:
        return False
        
def plural_singular(substantivo):
    # Define uma expressão regular para validar o formato do e-mail
    padrao = r'^[a-zA-Z]+[sS]$'
    
    # Utiliza a função match para verificar se o e-mail corresponde ao padrão
    if re.match(padrao, substantivo): 
        return True
    else:
        return False


# Teste
# email = input("Digite um endereço de e-mail: ")
#if valida_email(email):
 #   print("E-mail válido")
#else:
    #print("E-mail inválido")

#cpf = input("Digite um número de CPF: ")
#if valida_CPF(cpf):
 #   print("CPF válido")
#else:
 #   print("CPF inválido")

#matricula = input("Digite um número de matricula: ")
#if valida_matricula(matricula):
   # print("matricula válida")
#else:
    #print("matricula inválida")
    
#nome = input("Digite um nome: ")
#if valida_nome(nome):
    #print("nome válido")
#else:
    #print("nome inválido")
    
substantivo = input("Digite uma palavra: ")
if plural_singular(substantivo):
    print("plural")
else:
    print("singular")
