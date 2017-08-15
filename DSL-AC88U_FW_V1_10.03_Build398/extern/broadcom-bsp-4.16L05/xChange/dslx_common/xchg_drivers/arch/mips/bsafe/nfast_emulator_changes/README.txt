Patch the changes in these files to the emulator files of ncipher.

/opt/nfast/c/kdp/examples/emulator/commands.c
/opt/nfast/c/kdp/examples/utilities/Makefile

nfast emulator Quick start guide.

To compile the emulator
	follow the instructions in opt/nfast/c/kdp/README



	ie,	

	compile openssl[openssl-0.9.7c] in some directory and install it.
		   ./config --prefix=<mydir>/install --openssldir=<mydir>/install
   			make
   			make install
			check to see if install dir has the lib and include dir or create links.
				
	untar the emulator
	cd opt/nfast/c/kdp/examples/utilities
	edit Makefile
		edit the OPENSSL directory variable to point to the openssl directories.
	
	For bsafe_uhsm offload apply the commands.c and Makefile diffs to the target directory files.
	
	
	compile
		make

To Run.

	./uhsm-getkey  -r <region> 
	./uhsm-getkey  -r <region> -k devkit/test-rsa1048
	./uhsm-getkey  -r <region> -k devkit/test-rsa2048
	./uhsm-testharness
	./uhsm-listkeys

	./uhsm-initialize 
		to get the pub key
		enroll enrolment.pub in the key server.

	The enrolment.jun21.pub in this directory has been registered with the key server at ncipher[kpstest.ncipher.com:9102] under gigi.	All the fixed  keys in commands.c has been fixed based on those keys.
		


	

	
	

