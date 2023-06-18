#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void matrix(bool diagonal)
{
    const int R = 4;
    const int C = 4;
    int data[4][4] = {{ 1, 2, 3, 4},
                      {11,12,13,14},
                      {21,22,23,24},
                      {31,32,33,34}};


    if(diagonal)
    {
        for(int c=0;c<C;c++)
        {
            for(int r=c;r<R;r++)
            {
               printf("[%d,%d ]: %d \n",r,c,data[r][c]);
            }
        }

    }
    else
    {
         for(int c=0;c<C;c++)
        {
            for(int r=c+1;r<R;r++)
            {
               fprintf(stderr,"[%d,%d ]: %d \n",r,c,data[r][c]);
            }
        }
    }
    // DOPLNTE KOD
    // -----------
    // Pomoci cyklu for prochazejte jednotlive prvky v matici
    // a na stadardni chybovy vystup (stderr) vypiste ty prvky,
    // ktere se nachazi pod diagonalou. Pokud bude parametr
    // 'diagonal' nastaven na true, pak zaroven vypisujte take
    // prvky na diagonale.
    //
    // Format vypisu:
    // [x, y]: v
    // x = prvni souradnice prvku
    // y = druha souradnice prvku
    // v = hodnota prvku
    // napriklad:
    // [1, 2]: 13

}

void words()
{
    int words_counter = 0;
    int longest_word = 0;
    int word=0;
    char c= getchar();
    while ((c=getchar())!= EOF )
    {
    word++;
        if(c==' '||c=='\n')
        {
            if(word>longest_word)
            {
                longest_word=word;

            }
            word=0;
            words_counter++;

        }






    }






    // DOPLNTE KOD
    // -----------
    // Pomoci funkce 'getchar' nacitejte znaky ze standardniho vstupu
    // a spocitejte pocet slov (promenna words_counter) na vstupu
    // a delku nejdelsiho slova (promenna longest_word). Samozrejme,
    // v ramci teto funkce si muze vytvaret libovolne vlastni promenne.

    printf("Number of words: %d\n", words_counter);
    printf("Length of the longest word: %d\n", longest_word-1); //-1 zufale osetrenie ale inak som to uz nevedel
}

int main(int argc, char * argv[])
{
    bool diagonal = false;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--diagonal") == 0)
        {
            diagonal = true;
        }
    }

    words();
    matrix(diagonal);

	return 0;
}
