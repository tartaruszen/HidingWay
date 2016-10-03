/*********************************************
* ���빱�ף�
* ע�����
1����һ�м��ϡ�#define HAVE_REMOTE���������Ͳ�Ҫ����ͷ�ļ� remote-ext.h,Ҳ����˵����Ч��һ�������Ƽ�ǰ�ߡ�
2���ܶ�̳��ж�ֻ�����ͷ�ļ���pcap.h�� ����ʾ��غ����޷���������Ҫ���������wpcap.lib
3���������ǵ�pcap_freealldevs()��
*********************************************/
#define HAVE_REMOTE
#include "pcap.h"
#pragma comment(lib,"wpcap.lib")

/*packet handler ����ԭ��*/
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

int main()
{
 pcap_if_t *alldevs;
 pcap_if_t *d;
 int inum;
 int i = 0;
 pcap_t *adhandle;
 char errbuf[PCAP_ERRBUF_SIZE];

 //��ȡ�����豸�б�
 if(pcap_findalldevs(&alldevs, errbuf) == -1)
 {
  printf("Error in pcap_findalldevs: %s\n", errbuf);
  exit(1);
 }

 //��ӡ�б�
 for(d = alldevs; d != NULL; d = d->next)
 {
  printf("%d. %s", ++i, d->name);
  if(d->description)
   printf(" (%s)\n", d->description);
  else
   printf(" (No description available) \n");
 }

 printf("Enter the interface number (1-%d):", i);
 scanf("%d", &inum);
 if(inum < 1 || inum > i)
 {
  printf("\nInterface number out of range.\n");
  //�ͷ��豸�б�
  pcap_freealldevs(alldevs);
  return -1;
 }

 //��ת��ѡ�е�������
 for(d = alldevs, i = 0; i < inum -1; d=d->next, i++);

 //���豸
if ( (adhandle= pcap_open_live(d->name, // name of the device
		65536, // portion of the packet to capture. 65536 grants that the whole packet will be captured on all the MACs.
		1, // ����ģʽ
		1000, // ���ó�ʱʱ��,����Ϊ��λ
		errbuf // ��������ʱ��Ŵ������ݵĻ�����
		) ) == NULL)
 {
  printf("\nUnable to open the adapter.%s is not supported by WinPcap\n", d->name);
  //�ͷ��豸�б�
  pcap_freealldevs(alldevs);
  return -1;
 }

 printf("\nlistening on %s... \n", d->description);
 //�ͷ��豸�б�
 pcap_freealldevs(alldevs);

 //��ʼ��׽
 pcap_loop(adhandle, 0, packet_handler, NULL);

 return 0;
}

//ÿ�β�׽�����ݰ�ʱ��libpcap�����Զ���������ص�����
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
 struct tm *ltime;
 char timestr[16];
 time_t local_tv_sec;

 //��ʱ���ת���ɿ�ʶ��ĸ�ʽ
 local_tv_sec = header->ts.tv_sec;
// printf("%d \n", local_tv_sec);
// ltime = localtime(&local_tv_sec);
// strftime(timestr, sizeof(timestr), "%H %M %S", ltime);
 printf("%s,%.6d len:%d\n", ctime(&local_tv_sec), header->ts.tv_usec, header->len);
 printf("%s\n",pkt_data);
}
