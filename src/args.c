/*
** args.c for philo in /home/blanch_p/rendu/Semestre3/Unix/PSU_2016_philo
**
** Made by Alexandre BLANCHARD
** Login   <blanch_p@epitech.net>
**
** Started on  Mon Mar 13 10:25:23 2017 Alexandre BLANCHARD
** Last update Fri Mar 17 10:52:26 2017 Alexandre BLANCHARD
*/

#include "philo.h"

int	have_nb_philo(char **av, t_args *args)
{
  if (strcmp(av[1], "-p") == 0)
    {
      args->philo = atoi(av[2]);
      return (0);
    }
  else if (strcmp(av[3], "-p") == 0)
    {
      args->philo = atoi(av[4]);
      return (0);
    }
  else
    {
      printf("Warning !\n\tNo -p argument.\n\n");
      return (1);
    }
}

int	have_nb_occur(char **av, t_args *args)
{
  if (strcmp(av[1], "-e") == 0)
    {
      args->occur = atoi(av[2]);
      return (0);
    }
  else if (strcmp(av[3], "-e") == 0)
    {
      args->occur = atoi(av[4]);
      return (0);
    }
  else
    {
      printf("Warning !\n\tNo -e argument.\n\n");
      return (1);
    }
}

int	check_is_num(char **av)
{
  int	i;

  i = 0;
  while (av[2][i])
    {
      if (av[2][i] < 48 || av[2][i] > 57)
	{
	  printf("Warning !\n\tArgument 2 '%s' is not a number.\n\n", av[2]);
	  return (1);
	}
      i++;
    }
    i = 0;
  while (av[4][i])
    {
      if (av[4][i] < 48 || av[4][i] > 57)
	{
	  printf("Warning !\n\tArgument 4 '%s' is not a number.\n\n", av[4]);
	  return (1);
	}
      i++;
    }
  return (0);
}

int	check_args(char **av, t_args *args)
{
  if (check_is_num(av) != 0
      || have_nb_philo(av, args) != 0
      || have_nb_occur(av, args) != 0)
    {
      usage(av);
      return (1);
    }
  if (args->philo <= 1)
    {
      printf("WARNING !\n\tA philosopher can't eat alone !\n\n");
      usage(av);
      return (1);
    }
  if (args->occur < 1)
    {
      printf("WARNING !\n\tPhilosopher(s) need to eat !\n\n");
      usage(av);
      return (1);
    }
  return (0);
}
