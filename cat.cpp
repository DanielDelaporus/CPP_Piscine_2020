/*
** EPITECH PROJECT, 2020
** double_tree.c
** File description:
** same
*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

void my_cat(char *s)
{
    std::ifstream f(s);

    if (f.is_open()) {
        std::cout << f.rdbuf();
        std::cout << "\n";
    }
    else
        std::cerr << "my_cat: " << s << ": No such  file or  directory\n";
}

int main (int ac, char *argv[])
{
    if (ac < 2)
        std::cerr << "my_cat: Usage: ./ my_cat  file  [...]\n";
    for (int i = 1; i < ac; i++)
        my_cat(argv[i]);
    return 0;
}