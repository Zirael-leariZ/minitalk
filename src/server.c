/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oishchen <oishchen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:49:44 by oishchen          #+#    #+#             */
/*   Updated: 2025/04/19 15:44:30 by oishchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
// #include "minitalk.h"

void	ft_get_pid(void)
{
	pid_t	pid;

	pid = getpid();
	printf("Server PID: %d\n", pid);
}

void	sig_handler(int signum, siginfo_t *info, void *context)
{
	static int				i = 0;
	static unsigned char	c = 0;

	(void)context;
	(void)info;

	// printf("inside the handler\n");
	if (signum == SIGUSR1)
		c |= 1 << (7 - i);
	i++;
	if (i == 8)
	{
		if (c == '\0')
		{
			write(1, "\n", 1);
			kill(info->si_pid, SIGUSR1);
		}
		else
		{
			write(1, &c, 1);
			// kill(info->si_pid, SIGUSR2);
		}
		i = 0;
		c = 0;
	}
}

int main(int ac, char *av[])
{
	pid_t				srv_pid;
	struct sigaction	sa;

	ft_get_pid();
	bzero(&sa, sizeof(sa));
	sa.sa_sigaction = sig_handler;
	sa.sa_flags = SA_SIGINFO;
	// printf("Just a check\n");
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		pause();
	return (0);
}