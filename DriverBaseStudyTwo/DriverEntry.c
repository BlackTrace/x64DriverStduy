#include <ntddk.h>


#define DEVICE_NAME			L"\\Device\\krnlHW64Test"
#define LINK_NAME			L"\\DosDevices\\krnlHW64Test"
#define LINK_GLOBAL_NAME	L"\\DosDevices\\Global\\krnlHW64Test"

void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UNICODE_STRING uStrLink;
	if(IoIsWdmVersionAvailable(1,0x10))
	{
		RtlInitUnicodeString(&uStrLink,LINK_GLOBAL_NAME);
		
	}
	else
	{
		
		RtlInitUnicodeString(&uStrLink,LINK_NAME);
	}
	
	IoDeleteSymbolicLink(&uStrLink);
	IoDeleteDevice(pDriverObject->DeviceObject);
	DbgPrint("my driver unload !!!");
	
}


NTSTATUS DispatchCreate(PDEVICE_OBJECT pDeviceObject,PIRP pIrp)
{
	DbgPrint("DispatchCreate \n");
	
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);
	
	return STATUS_SUCCESS;
	
	
}


NTSTATUS DispatchClose(PDEVICE_OBJECT pDeviceObject,PIRP pIrp)
{
	DbgPrint("DispatchClose \n");
	
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);
	
	return STATUS_SUCCESS;
	
	
}


NTSTATUS DispatchControl(PDEVICE_OBJECT pDevObj,PIRP pIrp)
{
	
	 NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
	 PIO_STACK_LOCATION pIrpStack;
	 ULONG uIoControlCode;
	 PVOID pIoBuffer;
	 ULONG  uInSize;
	 ULONG  uOutSize;
	 
	 DbgPrint("DispatchControl\n");
	 
	 pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	 
	 //控制码
	 uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	 //输入输出缓冲区
	 pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	 
	 //输入区域大小
	 uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	 
	 //输出区域大小
	 uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;
	 
	 switch(uIoControlCode)
	 {
		 case 1:
		 break;
	 }
	 
	 if(status == STATUS_SUCCESS)
		pIrp->IoStatus.Information = uOutSize;
	 else
		 pIrp->IoStatus.Information = 0;
	 
	 pIrp->IoStatus.Status = status;
	 IoCompleteRequest(pIrp,IO_NO_INCREMENT);
	 
	 return status;
	 
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,PUNICODE_STRING pRegistryString)
{
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING uStrLinkName;
	UNICODE_STRING uStrDevName;
	PDEVICE_OBJECT pDevObj;
	
	
	pDriverObject->DriverUnload = DriverUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchControl;
	
	
	RtlInitUnicodeString(&uStrDevName,DEVICE_NAME);
	status = IoCreateDevice(pDriverObject,0,&uStrDevName,FILE_DEVICE_UNKNOWN,0,FALSE,&pDevObj);
	
	if(!NT_SUCCESS(status)) return status;
	
	
	if(IoIsWdmVersionAvailable(1,0x10))
	{
		RtlInitUnicodeString(&uStrLinkName,LINK_GLOBAL_NAME);
		
	}
	else
	{
		
		RtlInitUnicodeString(&uStrLinkName,LINK_NAME);
	}
	
	
	status = IoCreateSymbolicLink(&uStrLinkName,&uStrDevName);
	
	if(!NT_SUCCESS(status))
	{
		IoDeleteDevice(pDevObj);
		return status;
	}
	DbgPrint("load my driver success");


	return status;
}