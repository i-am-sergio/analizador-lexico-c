#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

#define MAYOR '>'
#define PUNTOYCOMA ';'
#define PARI '('
#define ID 256
#define NUM 257
#define MAYORIGUAL 258
#define WHILE 259
#define IF 260

#define MENOR '<'
#define MENORIGUAL 261
#define ASIGNACION '='
#define IGUAL 262
#define DIFERENTE 263
#define SUMA '+'
#define RESTA '-'
#define MULTIPLICACION '*'
#define DIVISION '/'
#define PARD ')'
#define CORCHI '['
#define CORCHD ']'
#define COMA ','
#define LLAVI '{'
#define LLAVD '}'
#define ELSE 264
#define FOR 265
#define VOID 266
#define INIBLOQUECOMMENT 267
#define FINBLOQUECOMMENT 268


int scaner();
void mostrar(int);
int espalres();

FILE *f;
char lexema[80];

void main(int n, char *pal[])
{
    int token;
    f = stdin;
    // entrada estandar del teclado
    if (n == 2)
    {                            // si se especifico un archivo de lectura
        f = fopen(pal[1], "rt"); // lectura modo texto
        if (f == NULL)
            f = stdin;
    }

    if (f == stdin) // la lectura sera desde la entrada estandar
        printf("Ingrese texto ........ termine con Ctrl z \n");

    while (1)
    {
        token = scaner();
        if (token == EOF)
            break;
        mostrar(token);
    }

    if (f != stdin) // si la entrada fue de un archivo
        fclose(f);  // entonces cerrar el archivo.
}

int scaner()
{
    int c;
    int i;
    do
        c = fgetc(f);
    while (isspace(c)); // ignora blancos

    if (c == EOF)
        return EOF;

    if (isalpha(c)) // regla del ID
    {
        i = 0;
        do
        {
            lexema[i++] = c;
            c = fgetc(f);
        } while (isalnum(c) || c == '_');
        lexema[i] = 0;
        ungetc(c, f);   // se devuelve c al flujo de entrada
        i = espalres(); // verifica si es palabra reservada
        // WHILE , IF
        if (i >= 0)
            return i;
        return ID; // se trata de un ID
    }
    if (isdigit(c)) // regla del NUM
    {
        i = 0;
        do
        {
            lexema[i++] = c;
            c = fgetc(f);
        } while (isdigit(c));
        lexema[i] = 0;
        ungetc(c, f);
        return NUM;
    }
    // regla de PUNTOYCOMA y PARI
    if ((c == ';') || (c == '('))
        return c; // regla del ";" y "("

    if (c == '>') // regla de ">" o ">="
    {
        c = fgetc(f);
        if (c == '=') // return MAYORIGUAL
        {
            lexema[0] = '>';
            lexema[1] = '=';
            lexema[2] = 0;
            return MAYORIGUAL;
        }
        ungetc(c, f);
        return MAYOR; // return MAYOR
    }
    /*****************************************************/
    if (c == '<')
    {
        c = fgetc(f);
        if (c == '=')
        {
            lexema[0] = '<';
            lexema[1] = '=';
            lexema[2] = 0;
            return MENORIGUAL; // RETURN MENORIGUAL
        }
        ungetc(c, f);
        return MENOR; // return MENOR
    }
    if (c == '=')
    {
        c = fgetc(f);
        if (c == '=')
        {
            lexema[0] = '=';
            lexema[1] = '=';
            lexema[2] = 0;
            return IGUAL;
        }
        ungetc(c, f);
        return ASIGNACION; // return ASIGNACION
    }
    if (c == '!')
    {
        c = fgetc(f);
        if (c == '=')
        {
            lexema[0] = '!';
            lexema[1] = '=';
            lexema[2] = 0;
            return DIFERENTE;
        }
    }

    if (c == '/') 
    {
        c = fgetc(f);
        if (c == '/') // Para ignorar comentarios de línea
        {
            lexema[0] = '/';
            lexema[1] = '/';
            lexema[2] = 0;
            do
                c = fgetc(f);
            while (c != '\n');
            return scaner();
        }
        else if (c == '*') // Para ignorar comentarios en bloque
        {
            lexema[0] = '/';
            lexema[1] = '*';
            lexema[2] = 0;
            int commentClosed = 0; // Bandera para verificar si se cerro el comentario en bloque

            while (1)
            {
                c = fgetc(f);

                if (c == '*')
                {
                    c = fgetc(f);
                    if (c == '/')
                    {
                        commentClosed = 1;
                        break;
                    }
                }
                else if (c == EOF)
                {
                    break;
                }
            }

            if (!commentClosed)
            {
                printf("Error: Comentario en bloque no cerrado\n");
                return EOF;
            }
            else
            {
                return scaner();
            }
        }
        else
        {
            ungetc(c, f);
            return DIVISION;
        }
    }


    if (c == '+' || c == '-' || c == '*' || c == '/' || c == ')' || c == '[' || c == ']' || c == ',' || c == '{' || c == '}')
    {
        return c;
    }
    /******************************************************/
} // fin de scaner

int espalres()
{
    if (strcmp(lexema, "while") == 0)
        return WHILE;
    if (strcmp(lexema, "if") == 0)
        return IF;
    if (strcmp(lexema, "else") == 0)
        return ELSE;
    if (strcmp(lexema, "for") == 0)
        return FOR;
    if (strcmp(lexema, "void") == 0)
        return VOID;
    return -1;
}

void mostrar(int token)
{
    switch (token)
    {
    case ID:
        printf("token = ID [%s]\n", lexema);
        break;
    case NUM:
        printf("token = NUM [%s]\n", lexema);
        break;
    case MAYORIGUAL:
        printf("token= MAYORIGUAL[%s]\n", lexema);
        break;
    case WHILE:
        printf("token = WHILE [%s]\n", lexema);
        break;
    case IF:
        printf("token = IF [%s]\n", lexema);
        break;
    case PARI:
        printf("token = PARI [%c]\n", token);
        break;
    case MAYOR:
        printf("token = MAYOR [%c]\n", token);
        break;
    case PUNTOYCOMA:
        printf("token = PUNTOYCOMA [%c]\n", token);
        break;
    case MENOR:
        printf("token = MENOR [%c]\n", token);
        break;
    case MENORIGUAL:
        printf("token= MENORIGUAL [%s]\n", lexema);
        break;
    case ASIGNACION:
        printf("token = ASIGNACION [%c] \n", token);
        break;
    case IGUAL:
        printf("token = IGUAL [%s]\n", lexema);
        break;
    case DIFERENTE:
        printf("token = DIFERENTE [%s]\n", lexema);
        break;
    case SUMA: // +
        printf("token = SUMA [%c]\n", token);
        break;
    case RESTA:
        printf("token = RESTA [%c]\n", token);
        break;
    case MULTIPLICACION:
        printf("token = MULTIPLICACION [%c]\n", token);
        break;
    case DIVISION:
        printf("token = DIVISION [%c]\n", token);
        break;
    case PARD: // )
        printf("token = PARD [%c]\n", token);
        break;
    case CORCHI:
        printf("token = CORCHI [%c]\n", token);
        break;
    case CORCHD:
        printf("token = CORCHD [%c]\n", token);
        break;
    case COMA:
        printf("token = COMA [%c]\n", token);
        break;
    case LLAVI:
        printf("token = LLAVI [%c]\n", token);
        break;
    case LLAVD:
        printf("token = LLAVD [%c]\n", token);
        break;
    case ELSE:
        printf("token = ELSE [%s]\n", lexema);
        break;
    case FOR:
        printf("token = FOR [%s]\n", lexema);
        break;
    case VOID:
        printf("token = VOID [%s]\n", lexema);
        break;
    }
}
