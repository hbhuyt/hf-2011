//////////////////////////////////////////////////////////
// PTUSERIO.CPP�ļ�


#include <windows.h>
#include <stdio.h>
#include <ntddndis.h>
#include "../driver/IOCOMMON.h"
#include "ptutils.h"

void DisplayAdapterInfo(HANDLE hAdapter)
{
	char buffer[1024 + sizeof(PTUSERIO_OID_DATA) - 1];
	PPTUSERIO_OID_DATA pOid = (PPTUSERIO_OID_DATA)buffer;
	pOid->Length = 1024;


	// ��ѯ�����̵�������Ϣ
	pOid->Oid = OID_GEN_VENDOR_DESCRIPTION;
	pOid->Length = 1024;
	if(!PtAdapterRequest(hAdapter, pOid, TRUE))
	{
		printf(" PtAdapterRequest() failed \n ");
		return;
	}
	wprintf(L"	Description: %S \n", (LPWSTR)pOid->Data);

	// ��ѯ����ʹ�õ�ý��
	NDIS_MEDIUM NdisMedium;
	pOid->Oid = OID_GEN_MEDIA_IN_USE;
	pOid->Length = 1024;
	if(!PtAdapterRequest(hAdapter, pOid, TRUE))
	{
		printf(" PtAdapterRequest() failed \n ");
		return;
	}
	NdisMedium = *((PNDIS_MEDIUM )pOid->Data);

	switch(NdisMedium)
	{
	case NdisMedium802_3:
		printf("	Medium: NdisMedium802_3 \n");
		break;
	case NdisMediumWan:
		printf("	Medium: NdisMediumWan \n");
		break;
	default:
		printf("	unkown type \n");
		break;
	}

	// ��ѯ����״̬
	int nConnectedState;
	pOid->Oid = OID_GEN_MEDIA_CONNECT_STATUS;
	pOid->Length = 1024;
	if(!PtAdapterRequest(hAdapter, pOid, TRUE))
	{
		printf("	Media Connect Status: UNKNOWN \n ");
	}
	else
	{
		nConnectedState = *((int*)pOid->Data);
		printf( "	Media Connect Status: %s\n",
         nConnectedState == NdisMediaStateConnected ? "Connected" : "Disconnected");
	}
	
}

int main()
{
	HANDLE hControlDevice = PtOpenControlDevice();
	CIMAdapters adapters;
	if(!adapters.EnumAdapters(hControlDevice))
	{
		printf(" EnumAdapters failed \n");
		return -1;
	}

	printf(" Driver Bindings: \n");
	for(int i=0; i<adapters.m_nAdapters; i++)
	{
		// ��ʾ��������������
		printf("    \"%ws\" \n", adapters.m_pwszVirtualName[i]);
		// ��ʾ�²�����������
		printf("       \"%ws\" \n", adapters.m_pwszAdapterName[i]);
	

		HANDLE hLowerAdapter = PtOpenAdapter(adapters.m_pwszAdapterName[i]);
		if(hLowerAdapter != INVALID_HANDLE_VALUE)
		{
			DisplayAdapterInfo(hLowerAdapter);
			::CloseHandle(hLowerAdapter);
		}	
		
		printf(" \n");
	}
	::CloseHandle(hControlDevice);
	return 0;
}



/*
int main()
{
	HANDLE hControlDevice = PtOpenControlDevice();
	CIMAdapters adapters;
	if(!adapters.EnumAdapters(hControlDevice))
	{
		printf(" EnumAdapters failed \n");
		return -1;
	}

	printf(" Driver Bindings: \n");
	for(int i=0; i<adapters.m_nAdapters; i++)
	{
		// ��ʾ��������������
		printf("    \042%ws\042\n", adapters.m_pwszVirtualName[i]);
		// ��ʾ�²�����������
		printf("       \042%ws\042\n", adapters.m_pwszAdapterName[i]);
		printf(" \n");
	}
	::CloseHandle(hControlDevice);
	return 0;
}
*/

