/*
** main.c for  in /home/sauvau_m/rendu/PSU_2016_philo
**
** Made by Sauvau Mathieu
** Login   <mathieu.sauvau@epitech.eu>
**
** Started on  Mon Mar  6 10:22:43 2017 Sauvau Mathieu
** Last update Fri Mar 17 10:57:47 2017 Sauvau Mathieu
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "extern.h"
#include "philo.h"

int		g_nb_philo;
t_philo		*g_philo;
pthread_mutex_t	*g_chopsticks;

void		check_unlock(t_philo *philo, int chopstick_l,
			     int chopstick_r, bool record)
{
  if (!chopstick_l)
    {
      pthread_mutex_unlock(&g_chopsticks[philo->pos]);
      if (record)
	lphilo_release_chopstick(&g_chopsticks[philo->pos]);
    }
  if (!chopstick_r)
    {
      pthread_mutex_unlock(&g_chopsticks[(philo->pos + 1) % g_nb_philo]);
      if (record)
	lphilo_release_chopstick(&g_chopsticks[(philo->pos + 1) % g_nb_philo]);
    }
}

void		check_lock(t_philo *philo, int chopstick_l, int chopstick_r)
{
  if (!chopstick_r)
    lphilo_take_chopstick(&g_chopsticks[philo->pos]);
  if (!chopstick_l)
    lphilo_take_chopstick(&g_chopsticks[(philo->pos + 1) % g_nb_philo]);
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
