/*
** usage.c for usage in /home/blanch_p/rendu/Semestre3/Unix/PSU_2016_philo
**
** Made by Alexandre BLANCHARD
** Login   <blanch_p@epitech.net>
**
** Started on  Tue Mar 14 14:50:23 2017 Alexandre BLANCHARD
** Last update Tue Mar 14 14:50:49 2017 Alexandre BLANCHARD
*/

#include "philo.h"

void	usage(char **av)
{
  printf("Usage:\n\t%s -p X -e Y\n\n", av[0]);
  printf("\tX: Number of philosophers (minimum 2)\n");
  printf("\tY: Number of occurences (minimum 1)\n\n");
}
