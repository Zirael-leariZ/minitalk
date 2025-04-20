/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oishchen <oishchen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:49:51 by oishchen          #+#    #+#             */
/*   Updated: 2025/04/21 00:07:07 by oishchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minitalk.h"
#include "../mixlibft/libft/includes/libft.h"
#include "../mixlibft/printf/includes/ft_printf.h"

volatile sig_atomic_t	full_message;

void	ft_sig_handler(int signum, siginfo_t *info, void *contxt)
{
	static int	bit;
	(void)contxt;
	(void)info;
	if (signum == SIGUSR1)
		bit++;
	else if (signum == SIGUSR2)
	{
		bit++;
		ft_putnbr_fd(bit, 1);
		write(1, " bits were received\n", 20);
		full_message = 1;
	}
	usleep(100);
}

void	send_message(unsigned char c, int srv_pid, int *kill_sts)
{
	int	bit = 0;

	bit = 0;
	while (bit < 8)
	{
		full_message = 0;
		if ((c >> bit) & 1)
			*kill_sts = kill(srv_pid, SIGUSR1);
		else
			*kill_sts = kill(srv_pid, SIGUSR2);
		bit++;
		if (*kill_sts == -1)
			return ;
		usleep(100);
	}
}

int	main(int ac, char *av[])
{
	struct sigaction	sa;
	pid_t				srv_pid;
	int					kill_status;
	int					i;

	if (ac != 3)
	{
		ft_printf("Try <binary> <pid> <message>\n");
		return (1);
	}
	srv_pid = ft_atoi(av[1]);
	ft_bzero(&sa, sizeof(sa));
	sa.sa_sigaction = ft_sig_handler;
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		return (ft_printf("The SIG mapping failed\n"), 0);
	full_message = 0;
	kill_status = 0;
	i = 0;
	while (av[2][i] && !kill_status)
	{
		send_message(av[2][i], srv_pid, &kill_status);
		i++;
	}
	if (!kill_status)
		send_message('\0', srv_pid, &kill_status);
	if (kill_status == 1)
		return (ft_printf("The kill function failed!\n"), 0);
	while (!full_message)
		pause();
	ft_printf("The whole message was received!\n");
	return (0);
}