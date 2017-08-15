/*
 * libusb example program to list devices on the bus
 * Copyright (C) 2007 Daniel Drake <dsd@gentoo.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include <libusb.h>
#include <arcgpl.h>

/* definition of a data node for holding USB device information */
struct usb_entry {
	uint8_t root_port_number;
	uint16_t idVendor;
	uint16_t idProduct;
	char *Manufacturer;
	char *Product;
	char *SerialNumber;
	uint8_t DeviceClass;
	int speed;
	struct usb_entry *next;
};

struct usb_entry *head = (struct usb_entry *) NULL;
static int iUpdateUsbLed = 0;

char *speed2string(int speed)
{
	char *pString;
	switch (speed){
		case LIBUSB_SPEED_LOW:
			pString = "low speed (1.5MBit/s)";
		break;
		case LIBUSB_SPEED_FULL:
			pString = "full speed (12MBit/s)";
		break;
		case LIBUSB_SPEED_HIGH:
			pString = "high speed (480MBit/s)";
		break;
		case LIBUSB_SPEED_SUPER:
			pString = "super speed (5000MBit/s)";
		break;
		case LIBUSB_SPEED_UNKNOWN:
		default:
			pString = "known speed";
			break;
#if 0
	LIBUSB_SPEED_UNKNOWN = 0,

	/** The device is operating at low speed (1.5MBit/s). */
	LIBUSB_SPEED_LOW = 1,

	/** The device is operating at full speed (12MBit/s). */
	LIBUSB_SPEED_FULL = 2,

	/** The device is operating at high speed (480MBit/s). */
	LIBUSB_SPEED_HIGH = 3,

	/** The device is operating at super speed (5000MBit/s). */
	LIBUSB_SPEED_SUPER = 4,
#endif	
	}
	return pString;
}

char *convertDeviceClass2String(int iDevClass)
{
	char *pString = NULL;
	
	switch (iDevClass) {
		case 0x01:
			pString = "Audio";
			break;
		case 0x02:
			pString = "Communications and CDC Control";
			break;
		case 0x03:
			pString = "HID (Human Interface Device)";
			break;
		case 0x05:
			pString = "Physical";
			break;
		case 0x06:
			pString = "Image";
			break;
		case 0x07:
			pString = "Printer";
			break;
		case 0x08:
			pString = "Mass Storage";
			break;
		case 0x09:
			pString = "Hub";
			break;
		case 0x0A:
			pString = "CDC-Data";
			break;
		case 0x0B:
			pString = "Smart Card";
			break;
		case 0x0D:
			pString = "Content Security";
			break;
		case 0x0E:
			pString = "Video";
			break;
		case 0x0F:
			pString = "Personal Healthcare";
			break;
		case 0x10:
			pString = "Audio/Video Devices";
			break;
		case 0x11:
			pString = "Billboard Device Class";
			break;
		case 0xDC:
			pString = "Diagnostic Device";
			break;
		case 0xE0:
			pString = "Wireless Controller";
			break;
		case 0xEF:
			pString = "Miscellaneous";
			break;
		case 0xFE:
			pString = "Application Specific";
			break;
		case 0xFF:
			pString = "Vendor Specific";
			break;
		default:
			pString = "Unknown";
	}
	return pString;
}
struct usb_entry *init_usb_entry(
	uint8_t root_port_number,
	uint16_t idVendor,
	uint16_t idProduct,
	char *Manufacturer,
	char *Product,
	char *SerialNumber,
	uint8_t DeviceClass,
	int speed
)
{
	struct usb_entry *node = malloc(sizeof(struct usb_entry));
	if(node!=NULL) {
		node->root_port_number = root_port_number;
		node->idVendor = idVendor;
		node->idProduct = idProduct;
		node->Manufacturer = strdup(Manufacturer);
		node->Product = strdup(Product);
		node->SerialNumber = strdup(SerialNumber);
		node->DeviceClass = DeviceClass;
		node->speed = speed;
		node->next = NULL;
		#if 0
		fprintf(stdout, "new_usb_entry:\n");
		fprintf(stdout, "root_port_number %d\n", root_port_number);
		fprintf(stdout, "idVendor %04x:%04x\n", idVendor, idProduct);
		fprintf(stdout, "Manufacturer %s\n", Manufacturer);
		fprintf(stdout, "Product %s\n", Product);
		fprintf(stdout, "SerialNumber %s\n", SerialNumber);
		fprintf(stdout, "DeviceClass %d (%s)\n", (int)DeviceClass, convertDeviceClass2String(DeviceClass));
		#endif
	}
	return node;
}

void print_usb_entry(struct usb_entry *entry, FILE *out)
{
	if(out!=NULL) {
		fprintf(out, "%d:%04x:%04x:%s:%s:%s:%s:%s\n", \
					(int)entry->root_port_number, \
					entry->idVendor, \
					entry->idProduct, \
					entry->Manufacturer, \
					entry->Product, \
					entry->SerialNumber, \
					convertDeviceClass2String(entry->DeviceClass), \
					speed2string(entry->speed) );
	} else if (iUpdateUsbLed) {
		if(entry->root_port_number==1) //usb2.0
			//20160106: ASUS change spec, 3.0 and 2.0 share the same LED usb3
			//system("arc_led usb2 on");
			system("arc_led usb3 on");
		else
			system("arc_led usb3 on");
	} else{
		fprintf(stdout, "\n");
		fprintf(stdout, "USB attached to port number %d\n", (int)entry->root_port_number);
		fprintf(stdout, "idVendor %04x:%04x\n", entry->idVendor, entry->idProduct);
		fprintf(stdout, "Manufacturer \"%s\" Product \"%s\" SerialNumber \"%s\"\n", entry->Manufacturer, entry->Product, entry->SerialNumber);
		fprintf(stdout, "DeviceClass %d (%s) speed %d (%s)\n", (int)entry->DeviceClass, convertDeviceClass2String(entry->DeviceClass), entry->speed, speed2string(entry->speed));
	}
}

void printDevsTree(FILE *output)
{
	struct usb_entry *current_node = head;

	if(output!=NULL) {
		fprintf(output, "#FORMAT:\n");
		fprintf(output, "#USB_PORT:VID:PID:Manufacturrer:Product:SerialNumber:DeviceClass:Speed\n");
	}
	if (iUpdateUsbLed) {
		//Off all LED first.
		//20160106: ASUS change spec, 3.0 and 2.0 share the same LED usb3
		//system("arc_led usb2 off");
		system("arc_led usb3 off");
	}
	while(current_node!=NULL) {
		print_usb_entry(current_node,output);
		current_node = current_node->next;
	}
}


void append_usb_entry(struct usb_entry *new) 
{
	struct usb_entry *tail;

	if(head==NULL) {
		head = new;
	} else {
		tail = head;
		while(tail->next!=NULL){
			tail = tail->next;
		}
		tail->next = new;
	}
}

void UpdateAsusParameters()
{
	int i;
	char prefix[64];
	char cfgname[128];
	char serial[128];
	char act[64];
	char buffer[256];
	struct usb_entry *current_node = head;
	FILE *fp = NULL;

	for (i=1; i<=2; i++)
	{
		sprintf(prefix, "ASUS_usb_path%d", i);
		arcgpl_cfg_set(prefix, "");
		sprintf(cfgname, "%s_act", prefix);
		arcgpl_cfg_set(cfgname, "");
		sprintf(cfgname, "%s_pid", prefix);
		arcgpl_cfg_set(cfgname, "");
		sprintf(cfgname, "%s_vid", prefix);
		arcgpl_cfg_set(cfgname, "");
		sprintf(cfgname, "%s_serial", prefix);
		arcgpl_cfg_set(cfgname, "");
		sprintf(cfgname, "%s_speed", prefix);
		arcgpl_cfg_set(cfgname, "");
		sprintf(cfgname, "%s_manufacturer", prefix);
		arcgpl_cfg_set(cfgname, "");
	}

	system("lsblk -o KNAME,SERIAL,MODEL -n | grep sd > /tmp/lsblk_file");
	while(current_node != NULL) {
		sprintf(prefix, "ASUS_usb_path%d", current_node->root_port_number);
		if (current_node->DeviceClass == 0x8)
		{
			arcgpl_cfg_set(prefix, "storage");
			if (current_node->SerialNumber != NULL)
			{
				char *snptr = current_node->SerialNumber;
				while(*snptr == ' ') snptr++;
				fp = fopen("/tmp/lsblk_file", "r");
				if (fp != NULL)
				{
					while(fgets(buffer, sizeof(buffer), fp) != NULL)
					{
						if (sscanf(buffer, "%s %s", act, serial) == 2)
						{
							if (strcasecmp(snptr, serial) == 0)
							{
								sprintf(cfgname, "%s_act", prefix);
								arcgpl_cfg_set(cfgname, act);
								break;
							}
						}
					}
					fclose(fp);
				}
				sprintf(cfgname, "%s_serial", prefix);
				arcgpl_cfg_set(cfgname, snptr);
			}
		}

		if (current_node->Manufacturer)
		{
			sprintf(cfgname, "%s_manufacturer", prefix);
			arcgpl_cfg_set(cfgname, current_node->Manufacturer);
		}

		sprintf(cfgname, "%s_vid", prefix);
		sprintf(buffer, "%04x", current_node->idVendor);
		arcgpl_cfg_set(cfgname, buffer);
		sprintf(cfgname, "%s_pid", prefix);
		sprintf(buffer, "%04x", current_node->idProduct);
		arcgpl_cfg_set(cfgname, buffer);

		sprintf(cfgname, "%s_speed", prefix);
		switch (current_node->speed){
			case LIBUSB_SPEED_LOW:
				arcgpl_cfg_set(cfgname, "1.5");
			break;
			case LIBUSB_SPEED_FULL:
				arcgpl_cfg_set(cfgname, "12");
			break;
			case LIBUSB_SPEED_HIGH:
				arcgpl_cfg_set(cfgname, "480");
			break;
			case LIBUSB_SPEED_SUPER:
				arcgpl_cfg_set(cfgname, "5000");
			break;
			case LIBUSB_SPEED_UNKNOWN:
			default:
				break;
		}
		current_node = current_node->next;
	}
}

void freeDevsTree()
{
	struct usb_entry *current = head;

	while(current!=NULL) {
		struct usb_entry *free_node = current;
		current = current->next;
		free(free_node->Manufacturer);
		free(free_node->Product);
		free(free_node->SerialNumber);
		free(free_node);
	}
}

static void createDevsTree(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0;
	unsigned char strManufacturer[256];
	unsigned char strProduct[256];
	unsigned char strSerialNumber[256];
	uint8_t port_numbers[7] ={0};
	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		libusb_device_handle *devHandle = NULL;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			continue;
		}
		if(desc.idVendor == VENDOR_ID) continue;
		r = libusb_open (dev, &devHandle);
		if (r != LIBUSB_SUCCESS) continue;
		
		{
			uint8_t i,j,k;
			uint8_t root_port_number;
			
			bzero(strManufacturer, 256);
			if (desc.iManufacturer > 0)
			{
				r = libusb_get_string_descriptor_ascii(devHandle, desc.iManufacturer, strManufacturer, 256);
			}
			bzero(strProduct, 256);
			if (desc.iProduct > 0)
			{
				r = libusb_get_string_descriptor_ascii(devHandle, desc.iProduct, strProduct, 256);
			}
			bzero(strSerialNumber, 256);
			if (desc.iSerialNumber > 0)
			{
				r = libusb_get_string_descriptor_ascii(devHandle, desc.iSerialNumber, strSerialNumber, 256);
			}
			//fprintf(stdout, "    speed %d\n", libusb_get_device_speed(dev));
			r = libusb_get_port_numbers(dev, port_numbers, 7 );
			root_port_number = port_numbers[7-r];
			for(i=0;i<desc.bNumConfigurations;i++){
				struct libusb_config_descriptor *config;
				libusb_get_config_descriptor(dev, i, &config);
				for(j=0;j<config->bNumInterfaces;j++) {
					struct libusb_interface *interface = &(config->interface[j]);
					for(k=0;k<interface->num_altsetting;k++) {
						append_usb_entry( init_usb_entry( \
														root_port_number, \
														desc.idVendor, \
														desc.idProduct, \
														strManufacturer, \
														strProduct, \
														strSerialNumber, \
														interface->altsetting[k].bInterfaceClass, \
														libusb_get_device_speed(dev) )
						);
					}
				}
				libusb_free_config_descriptor(config);
			}
		libusb_close(devHandle);
		}
	}
}

int main(int argc, const char * argv[])
{
	libusb_device **devs;
	int r;
	ssize_t cnt;
	FILE *output = NULL;

	if(argc>2) return;
	
	r = libusb_init(NULL);
	if (r < 0)
		return r;

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0)
		return (int) cnt;

	createDevsTree(devs);
	libusb_free_device_list(devs, 1);
	libusb_exit(NULL);
	
	if(argc==2) {
		if(strncmp(argv[1], "UpdateUsbLed", 12)==0)
			iUpdateUsbLed = 1;
		else if (strcmp(argv[1], "UpdateAsusParameters")==0)
			UpdateAsusParameters();
		else
			output = fopen(argv[1], "w");
	}
	printDevsTree(output);
	if(output!=NULL)
		fclose(output);
	
	freeDevsTree();
	return 0;
}

