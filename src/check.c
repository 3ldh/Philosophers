/*
** check.c for check in /home/blanch_p/rendu/Semestre3/Unix/PSU_2016_philo
**
** Made by Alexandre BLANCHARD
** Login   <blanch_p@epitech.net>
**
** Started on  Fri Mar 17 16:13:29 2017 Alexandre BLANCHARD
** Last update Fri Mar 17 16:14:07 2017 Alexandre BLANCHARD
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "extern.h"
#include "philo.h"

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
