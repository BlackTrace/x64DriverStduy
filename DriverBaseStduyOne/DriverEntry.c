#include <ntddk.h>


void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	
	kdprint(("my driver unload ！！！！"));
	
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,PUNICODE_STRING pRegistryString)
{
	
	pDriverObject->DriverUnload = DriverUnload;
	
	kdprint(("load my driver success"));


	return STATUS_SUCCESS;
}