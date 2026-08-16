# Simple Shell

## simple shell 0.1

créer un shell qui fonctionne en boucle :
    1. afficher un prompt
    2. lire la commande de l'utilisateur
    3. créer un processus enfant (fork)
    4. dans l'enfant : exécuter la commande (execve)
    5. dans le parent : attendre la fin de l'enfant (wait)
    6. retourner à l'étape 1

    compilation avec : gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

## Authors

- Raider Del Castillo Abalos
- Theo Golik
