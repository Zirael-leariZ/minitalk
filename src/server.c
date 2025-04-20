/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oishchen <oishchen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:49:44 by oishchen          #+#    #+#             */
/*   Updated: 2025/04/21 00:07:21 by oishchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
// #include "../mixlibft/libft/includes/libft.h"
// #include "../mixlibft/printf/includes/ft_printf.h"

volatile sig_atomic_t	kill_result;

void	ft_exit(int stauts)
{
	ft_printf("The server was terminated, status is: %d\n", stauts);
	exit(1);
}

int	ft_get_pid()
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
	(void)info;
	if (signum == SIGINT)
		ft_exit(1);
	else if (signum == SIGUSR1)
		c |= 1 << bit;
	// kill(info->si_pid, SIGUSR1);
	usleep(100);
	bit++;
	if (bit == 8)
	{
		if (c == '\0')
		{
			write(1, "\n", 1);
			kill_result = kill (info->si_pid, SIGUSR2);
		}
		else
			write(1, &c, 1);
		bit = 0;
		c = 0;
	}
}

int	main(void)
{
	struct sigaction	sa;
	int					print_res;

	print_res = ft_get_pid();
	if (print_res == -1)
		return(ft_printf("ERROR: FD is closed\n"), 1);
	ft_bzero(&sa, sizeof(sa));
	sa.sa_sigaction = ft_sig_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	kill_result = 0;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		return(ft_printf("ERROR: SIG mapping failed\n"), 1);
	if (kill_result)
		return (ft_printf("The kill failed\n"));
	while (1)
		pause();
}