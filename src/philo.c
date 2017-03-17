/*
** philo.c for  in /home/sauvau_m/rendu/PSU_2016_philo
**
** Made by Sauvau Mathieu
** Login   <mathieu.sauvau@epitech.eu>
**
** Started on  Tue Mar 14 14:52:07 2017 Sauvau Mathieu
** Last update Fri Mar 17 11:06:27 2017 Sauvau Mathieu
*/

#include "unistd.h"
#include "extern.h"
#include "philo.h"

void		eat(t_philo *philo)
{
  //  usleep(2000);
  //printf("philo %d eat\n", philo->pos);
  philo->state = EAT;
  --philo->nb_eat;
  lphilo_eat();
}

void		rest(t_philo *philo)
{
  usleep(5000);
  //printf("philo %d rest\n", philo->pos);
  philo->state = REST;
  lphilo_sleep();
}

int		check_eat(t_philo *philo, int chopstick_l, int chopstick_r)
{
  if (!chopstick_l && !chopstick_r)
    {
      if (cancel_thread())
	{
	  check_unlock(philo, chopstick_l, chopstick_r, false);
	  return (2);
	}
      check_lock(philo, chopstick_l, chopstick_r);
      eat(philo);
      check_unlock(philo, chopstick_l, chopstick_r, true);
      return (1);
    }
  return (0);
}

void		think(t_philo *philo, int chopstick_l, int chopstick_r)
{
  int		b;

  check_lock(philo, chopstick_l, chopstick_r);
  //printf("philo %d think\n", philo->pos);
  philo->state = THINK;
  lphilo_think();
  check_unlock(philo, chopstick_l, chopstick_r, false);
  while (philo->state == THINK)
    {
      chopstick_l = pthread_mutex_trylock(&g_chopsticks[philo->pos]);
      chopstick_r = pthread_mutex_trylock(&g_chopsticks[(philo->pos + 1) % g_nb_philo]);
      b = check_eat(philo, chopstick_l, chopstick_r);
      if (b == 2)
	break;
      else if (b == 0)
	check_unlock(philo, chopstick_l, chopstick_r, false);
    }
}

void		*philo_logic(void *arg)
{
  t_philo	*philo;
  int		chopstick_r;
  int		chopstick_l;

  philo = (t_philo *)arg;
  chopstick_r = 1;
  while (philo->nb_eat > 0 && !philo->do_break)
    {
      chopstick_l = pthread_mutex_trylock(&g_chopsticks[philo->pos]);
      /* chopstick_r = pthread_mutex_trylock(&g_chopsticks[(philo->pos + 1) % g_nb_philo]); */
      /* if (philo->state != EAT && !chopstick_r && !chopstick_l) */
      /* 	{ */
      /* 	  check_lock(philo, chopstick_l, chopstick_r); */
      /* 	  eat(philo); */
      /* 	  check_unlock(philo, chopstick_l, chopstick_r, true); */
      /* 	} */
      /* else */ if (philo->state != EAT && (!chopstick_l))
	think(philo, chopstick_l, chopstick_r);
      else if (philo->state != REST)
	{
	  check_unlock(philo, chopstick_l, chopstick_r, false);
	  rest(philo);
	}
      //      cancel_thread(philo, chopstick_l, chopstick_r, false);
    }
  return (NULL);
}

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
