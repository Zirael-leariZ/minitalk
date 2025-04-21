/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oishchen <oishchen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:49:44 by oishchen          #+#    #+#             */
/*   Updated: 2025/04/21 16:23:10 by oishchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_kill_result;

int	ft_get_pid(void)
{
	pid_t	srv_pid;

	srv_pid = getpid();
	return (ft_printf("Server PID: %d\n", srv_pid));
}

void	ft_sig_handler(int signum, siginfo_t *info, void *contxt)
{
	static int				bit = 0;
	static unsigned char	c = 0;

	(void)contxt;
	if (signum == SIGUSR1)
		c |= 1 << bit;
	bit++;
	if (bit == 8)
	{
		if (c == '\0')
		{
			write(1, "\n", 1);
			usleep(100);
			g_kill_result = kill (info->si_pid, SIGUSR2);
		}
		else
			write(1, &c, 1);
		bit = 0;
		c = 0;
	}
	usleep(100);
	g_kill_result = kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;
	int					print_res;

	print_res = ft_get_pid();
	if (print_res == -1)
		return (ft_printf ("ERROR: FD is closed\n"), 1);
	ft_bzero(&sa, sizeof(sa));
	sa.sa_sigaction = ft_sig_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	g_kill_result = 0;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		return (ft_printf("ERROR: SIG mapping failed\n"), 1);
	while (1)
	{
		if (g_kill_result == -1)
			return (ft_printf("The kill failed\n"), 1);
		pause();
	}
	return (0);
}
