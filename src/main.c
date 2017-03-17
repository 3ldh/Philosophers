/*
** main.c for  in /home/sauvau_m/rendu/PSU_2016_philo
**
** Made by Sauvau Mathieu
** Login   <mathieu.sauvau@epitech.eu>
**
** Started on  Mon Mar  6 10:22:43 2017 Sauvau Mathieu
** Last update Fri Mar 17 16:14:53 2017 Alexandre BLANCHARD
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "extern.h"
#include "philo.h"

int		g_nb_philo;
t_philo		*g_philo;
pthread_mutex_t	*g_chopsticks;

void		init_philo(int nb_eat)
{
  int		i;

  i = -1;
  while (++i < g_nb_philo)
    {
      g_philo[i].state = DEFAULT;
      g_philo[i].nb_eat = nb_eat;
      g_philo[i].pos = i;
      g_philo[i].do_break = 0;
      pthread_mutex_init(&g_chopsticks[i], NULL);
    }
  i = -1;
  while (++i < g_nb_philo)
    pthread_create(&(g_philo[i].thread_ref), NULL, philo_logic, &g_philo[i]);
}

bool		cancel_thread()
{
  int		i;
  int		j;

  i = -1;
  j = -1;
  while (++i < g_nb_philo)
    {
    if (g_philo[i].nb_eat == 0)
      {
	while (++j < g_nb_philo)
	  {
	    g_philo[j].do_break = 1;
	  }
	return (true);
      }
    }
  return (false);
}

void		wait_threads()
{
  int		i;

  i = -1;
  while (++i < g_nb_philo)
    pthread_join(g_philo[i].thread_ref, NULL);
}

int		main(int ac, char **av)
{
  t_args	args;

  if (ac != 5)
    {
      usage(av);
      return (1);
    }
  if (check_args(av, &args) == 1)
    return (1);
  RCFStartup(ac, av);
  g_nb_philo = args.philo;
  if (!(g_philo = malloc(sizeof(t_philo) * g_nb_philo)) ||
      !(g_chopsticks = malloc(sizeof(pthread_mutex_t) * g_nb_philo)))
    return (1);
  init_philo(args.occur);
  wait_threads();
  RCFCleanup();
  return (0);
}
