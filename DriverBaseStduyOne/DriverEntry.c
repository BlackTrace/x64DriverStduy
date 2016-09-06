#include <ntddk.h>


void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	
	DbgPrint("my driver unload !!!");
	
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,PUNICODE_STRING pRegistryString)
{
	
	pDriverObject->DriverUnload = DriverUnload;
	
	DbgPrint("load my driver success");


	return STATUS_SUCCESS;
}