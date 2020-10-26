/*
 * trigger.c
 *
 *  Created on: Oct 16, 2020
 *      Author: lor
 */

#include "trigger.h"

void t_udpWait (void* param)
{
	ESP_LOGI(TAG_UDP, "Waiting for UDP trigger");
	int sock_udp = socket (AF_INET, SOCK_DGRAM, 0);
	int i_cmdlet = 0;
	int err = bind(sock_udp, (struct sockaddr *)&stu_serverAddressUdp, sizeof(struct sockaddr_in));
	if (err < 0) {
		ESP_LOGI(TAG_UDP, "Socket unable to bind: errno %d", errno);
	}
	while (i_cmdlet != CMD_rec)
	{
		fcheckTrigger(sock_udp, &i_cmdlet);
	}
	xEventGroupSetBits(eg_adc, BIT_ADC_WAIT);
	ESP_LOGI(TAG_UDP, "UDP trigger received");
	shutdown(sock_udp, 0);
	close(sock_udp);
	vTaskDelete(NULL);
}

int fcheckTrigger(int sock, int *i_cmdlet)
{
	int i_flag = 0;
	*i_cmdlet = 0;
	char data = '\0';
	for (int i = 0; i<5 ; i++)
	{
		recv(sock, &data, 1, 0);
		if ((data== '\r') || (data == '\n') || (data == '\0'))
		{
			break;
		}
		*i_cmdlet += (data << (8*(3-i)));
	}
	ESP_LOGV(TAG_TCP, "CMDlet received:%d\n",*i_cmdlet);
	return i_flag;
}

