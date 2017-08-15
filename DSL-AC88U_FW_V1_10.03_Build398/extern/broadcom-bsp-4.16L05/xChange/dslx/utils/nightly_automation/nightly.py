#################################################
# version 0.2 build Feb. 01. 2011
# see update log for detailed changes
#################################################
import sys
import os
import re
import telnetlib
import urllib2
import time
import subprocess
import datetime
from datetime import date
from time import strftime
from xml.etree.ElementTree import ElementTree
import smtplib
from email.MIMEMultipart import MIMEMultipart
from email.MIMEBase import MIMEBase
from email.MIMEText import MIMEText
from email.Utils import COMMASPACE, formatdate
from email import Encoders


###############################################################################################
# Some less frequently changed configuration parameters.
# (The more frequently changed configuration parameters are specified in
# test configuration xml file
###############################################################################################
# Prefix of the image files
SERVER_IMGPREFIX = 'BCA_CPE_CommEngine_Dev_'
#SERVER_IMGPREFIX = 'BCA_CPE_CommEngine_410_'

# Suffix of the image files
#SERVER_IMGSUFFIX = '_fs_kernel'
SERVER_IMGSUFFIX = '_cfe_fs_kernel'

# Local directory to temporarily save the nightly build images
#IMG_BASEDIR = 'C:\\automated_test_demo\\images\\'
#IMG_BASEDIR = '/home/lienh/projects/nightly_build/images/'

# HOST IP (the machine in the same LAN as of the DUT)
HOST_IP = '192.168.1.100'

# SMTP Server
SMTP_SERVER = 'smtphost.sj.broadcom.com'

#HOUR = 1 
#MIN = 00

#####################################################################################
# Private variables 
# Server Status codes (DO NOT CHANGE)
SERVER_STATUS_BADURL = -1  # Should never happen
SERVER_STATUS_200    =  0
SERVER_STATUS_404    =  1
SERVER_STATUS_UNREACHABLE   =  2

# DUT login
USER = 'admin'
PASSWORD = 'admin'
LOGIN_PROMPT = 'Login:'
PASSWORD_PROMPT = 'Password:'
LOGIN_TIMEOUT = 5 
COMMAND_PROMPT = '>'
DUMMY_READ_STRING = 'abcdcba'

#####################################################################################
# Phonebook tcl file that contains the phone xml string as tcl string variable
# The file will be included by the tcl script prior to each bulkcall test
PHONEBOOK_FILENAME = 'temp_phonebook.tcl'

#####################################################################################
# The following are used to contruct the xml string contained in temp_phonebook.tcl
# Phonebook xml prefix
PHONEBOOK_PREFIX = \
	'<phone-book type="EANALOG" side="Subscriber" phtype="Own">\n' + \
	'\t<analog-endpoints>\n'

# Phonebook xml suffix
PHONEBOOK_SUFFIX = \
	'\t</analog-endpoints>\n' + \
	'</phone-book>'


################################# Helper functions ############################################
def time_config():
	print '\nEnter hour:'
	temp_HOUR = input() # prompt for hour
	# boundary check, if condition not met keep prompting for input
	while temp_HOUR < 0 or temp_HOUR > 23: 
		print 'ERROR: Only 0 to 23 are accepted'
		temp_HOUR = input()
     
	print '\nEnter minute:'
	temp_MIN = input() # prompt for minute
	# boundary check, if condition not met keep prompting for input
	while temp_MIN < 0 or temp_MIN > 59:
		print 'ERROR: Only 0 to 59 are accepted'
		temp_MIN = input()
	return int(temp_HOUR), int(temp_MIN)


def store_img_dir_config():
	print '\nSpecify the full path of directory:'
	return raw_input()


def reset_config():
	return 1, 0, 'C:\\nightly_build\\images\\'


def exit_config():
	return 'Y'
	
	
def ConstructImgUrl(srvUrl, profile):
	if not srvUrl:
		return None
	# Url for image built last night
	today = date.today()
	yesterday = today - datetime.timedelta(1)
	datepath_str = yesterday.strftime('/%Y/%Y%m-%B/%Y%m-%B-%d/')
	return srvUrl + SERVER_IMGPREFIX + profile + datepath_str + 'bcm' + profile + SERVER_IMGSUFFIX 


def GetImage(url):
	f = None
	if url:
		try:
			print timestamp(), 'Retrieving image at', url, '.....',
			f = urllib2.urlopen(url)
			status = SERVER_STATUS_200

		except urllib2.HTTPError, e:
			print timestamp(), 'HTTP Error code:', e.code		
			status = SERVER_STATUS_404

		except urllib2.URLError:
			print timestamp(), 'NOT REACHABLE'
			status = SERVER_UNREACHABLE
	else:
		print timestamp(), 'INVALID SERVER URL'
		status = SERVER_STATUS_BADURL
	
	print 'DONE'
	return status,f


def SaveImage(fh, filename, IMG_BASEDIR):
	if not fh:
		return None
	try:
		# open the file for writing in binary mode
		print timestamp(), 'Saving image to', IMG_BASEDIR+filename, '.....',
		output = open(IMG_BASEDIR+filename, 'wb')
		output.write(fh.read())
		print 'DONE'
		output.close()
		return output
	except IOError:
		print timestamp(), __name__, ': cannot open file to save image'
	return None


def tel_connect(hostaddr):
	if not hostaddr:
		return None
	try:
		telconn = telnetlib.Telnet(hostaddr)
		telconn.set_debuglevel(0)
		# print 'Telnet connection to host established, Logging in'
		telbuff = telconn.read_until(LOGIN_PROMPT, LOGIN_TIMEOUT)
		telconn.write(USER + '\n')
		telbuff = telconn.read_until(PASSWORD_PROMPT, LOGIN_TIMEOUT)
		telconn.write(PASSWORD + '\n')

		telbuff = telconn.read_until(COMMAND_PROMPT)
		if telbuff.strip() is not '>':
			return None
		# print 'DUT is ready for commands'
	except IOError:
		# print 'Failed to connect host'
		telconn = None
	return telconn

def get_dutBuildProfile(currIpaddr, isUNI):
	# Setup the Telnet conenction
	telconn = tel_connect(currIpaddr)
	if not telconn:
		return None

	telconn.write('cat /proc/cpuinfo\n')
	telbuff = telconn.read_until('\r\n')
	telbuff = telconn.read_until('\r\n')

	# telbuff should contain the board Id at this point
	boardid_str = telbuff[telbuff.find(': ')+1:].strip()
	m = re.search('^([0-9]+)', boardid_str)
	if m:
		 board_id = m.group(0)

	# SPECIAL CASES for: 963293EPON and 963629EPON
	if board_id.endswith('293') or board_id.endswith('629'):
		board_id += 'EPONV'
	else:
		board_id += 'GWV'
	
	if isUNI == 'UNI':
		board_id += '_UNI'
	return board_id		


def get_testcfg_imgpath(filename):
	if not filename:
			return None

	TestCfg = ElementTree(file=filename)
	if not TestCfg:
		return None

	imagePathList = []
	for TestCase in TestCfg:
		print TestCase.get('imagePath')


def get_testcfg_info(filename):
	if not filename:
		return None

	TestCfg = ElementTree(file=filename)
	if not TestCfg:
		return None

	TestCases_DutList = []
	for TestCase in TestCfg.findall('TestCase'):	
		TestCase_info = {}
		# Get the ImagePath attribute 
		ImagePath = TestCase.get('ImageServer')
		TestCase_info['ImagePath'] = ImagePath

		# Repeat or not
		if TestCase.get('repeat') == 'Y':
			TestCase_info['Repeat'] = True
		else:
			TestCase_info['Repeat'] = False
		
		# Get the Abacus related attributes (Optional)
		AbacusEnv = TestCase.get('AbacusEnv')
		if AbacusEnv:
			TestCase_info['AbacusEnv'] = AbacusEnv
			TestCase_info['TestName'] = TestCase.get('TestName') #used as report folder prefix
			TestCase_info['ReportTemplate'] = TestCase.get('report_template') #used as report folder prefix
			
			# Get Abacus test duration in hours 
			TestDura = TestCase.get('Duration_HR')
			if TestDura:
				TestCase_info['Duration_HR'] = TestDura
			else:
				TestCase_info['Duration_HR'] = '0'

			# Get Abacus test duration in minutes 
			TestDura = TestCase.get('Duration_MIN')
			if TestDura:
				TestCase_info['Duration_MIN'] = TestDura
			else:
				if TestCase_info['Duration_HR'] == '0':
					return None
				TestCase_info['Duration_MIN'] = '0'
		else:
			return None

		TestInitSetup = TestCase.find('TestInitSetup')
		if TestInitSetup is None:
			return None

		DutList = []
		NumDut = len(TestInitSetup.getchildren())
		for DutCfg in TestInitSetup.getchildren():
			DutList.append({'IP':DutCfg.find('IP').text, \
					'PROFILE':DutCfg.find('PROFILE').text})

		TestExecution = TestCase.find('TestExecution')
		if TestExecution is None:
			return None

		TCLScript = TestExecution.get('TCLSCRIPT')
		if TCLScript:
			TestCase_info['TCLSCRIPT'] = TCLScript	

		i = 0
		for Dut in TestExecution.getchildren():
				if not Dut.find('Filename') == None:
					DutList[i]['CLIFile'] = Dut.find('Filename').text.strip()
				DutList[i]['From'] = Dut.find('FROM').text.strip()
				DutList[i]['To'] = Dut.find('TO').text.strip()
				i += 1
		
		DutList.append(TestCase_info)
		TestCases_DutList.append(DutList)
	return TestCases_DutList


	
def testsuite_summary(TestCases):
	if not TestCases or len(TestCases) < 1:
		print timestamp(), 'Invalid test configuration file'
		sys.exit(1)

	print '\n===============Tests Summary', timestamp(), '================'
	print 'Number of tests\t:\t', len(TestCases)
	print ''
	
	for CaseIx in range(len(TestCases)):
		print '+------------------------------+'
		print '| Test #' + str((CaseIx+1)) +  '                      |'
		print '+------------------------------+'
		print 'Image Location:\t\t\t',
		if TestCases[CaseIx][len(TestCases[CaseIx])-1]['ImagePath']:
			print TestCases[CaseIx][len(TestCases[CaseIx])-1]['ImagePath']
		else:
			print 'Not specified, will run test w/o new images'
		if 'AbacusEnv' in TestCases[CaseIx][len(TestCases[CaseIx])-1]:
			print 'Abacus Environment:\t\t\t' + TestCases[CaseIx][len(TestCases[CaseIx])-1]['AbacusEnv']
		if 'Duration_HR' in TestCases[CaseIx][len(TestCases[CaseIx])-1]:
			print 'Bulkcall duration Hours:\t\t\t' + TestCases[CaseIx][len(TestCases[CaseIx])-1]['Duration_HR']
		if 'Duration_MIN' in TestCases[CaseIx][len(TestCases[CaseIx])-1]:
			print 'Bulkcall duration Minutes:\t\t\t' + TestCases[CaseIx][len(TestCases[CaseIx])-1]['Duration_MIN']
		if 'TCLSCRIPT' in TestCases[CaseIx][len(TestCases[CaseIx])-1]:
			print 'TCL script:\t\t\t' + TestCases[CaseIx][len(TestCases[CaseIx])-1]['TCLSCRIPT'] + '\n'
		for DutIx in range(len(TestCases[CaseIx])-1):
			print 'DUT #' + str((DutIx+1))
			print '------------'
			Dut = TestCases[CaseIx][DutIx]
			for key in Dut:
				print '\t' + key + '\t:\t\t' + Dut[key]
		
	
def send_notification (toAddr, msg, filelist):
	if toAddr:	
		subject = '[DSLx Abacus Testing Automation test result]'
		mail = MIMEMultipart()
		mail['From'] = 'lienh'
		mail['To'] = toAddr	
		mail['Date'] = formatdate(localtime=True)
		mail['Subject'] = subject

		mail.attach(MIMEText(msg))
		
		for file in filelist:
			part = MIMEBase('application', 'octet-stream')
			part.set_payload( open(file+'/MyReport.pdf','rb').read() )
			Encoders.encode_base64(part)
			part.add_header('Content-Disposition', 'attachment; filename="%s.pdf"' % os.path.basename(file))
			mail.attach(part)

		server = smtplib.SMTP('smtphost.sj.broadcom.com')
		server.set_debuglevel(0)
		server.sendmail('lienh', toAddr, mail.as_string())
		server.close()
			
def timestamp():
	dt = list(time.localtime())
	return '['+str(dt[0])+'-'+str(dt[1])+'-'+str(dt[2])+' '+\
			str(dt[3])+':'+str('%02.0F'%dt[4])+':'+str('%02.0F'%dt[5])+']'

####################################Tasks##########################################################
TASK_STATUS_SUCCESS          = 0
TASK_STATUS_FAIL             = 1
TASK_STATUS_RETRY            = 2

ImageHandles = []
ReportDirList = []


def task_prepareImg(DUTCfgList, IMG_BASEDIR_temp2):
	'''
	Retrieve nightly build image from Hudson, and
	save it on local disk for each of the DUTs
	'''
	ImagePath = DUTCfgList[-1]['ImagePath']

	errmsg = ''
	for DUT in DUTCfgList[:-1]:
		profile = get_dutBuildProfile(DUT['IP'], DUT['PROFILE'])
		if profile is None:
			return TASK_STATUS_FAIL, 'Failed to retrieve board_id from DUT at '+DUT['IP']
		DUT['boardID'] = DUT['PROFILE'] + profile

		if not ImagePath:
			errmsg = 'No impage path specified, running the test w/o new images'
			continue

		filename = profile + SERVER_IMGSUFFIX
		DUT['image'] = 'bcm'+filename
		imgUrl = ConstructImgUrl(ImagePath, profile)
		
		server_status, fh = GetImage(imgUrl)
		if (server_status == SERVER_STATUS_404):
			# image is not ready or wrong filename
			return TASK_STATUS_RETRY, '404: image is not available'
		elif (server_status == SERVER_STATUS_UNREACHABLE):
			# HUDSON is down!!!!! a way to notify the team?
			return TASK_STATUS_FAIL, 'Server unreachable!!'
		elif (server_status == SERVER_STATUS_200):
			local_fh = SaveImage(fh, 'bcm'+filename, IMG_BASEDIR_temp2)
			if (local_fh):
				# For cleanup later
				global ImageHandles
				ImageHandles.append(local_fh)
			else:
				# Termination seems to be the only resolution
				# or report status to the mainloop???? if dont want to 
				# kill the process
				return TASK_STATUS_FAIL, 'Failed to save image'
		else:
			# HUDSON_STATUS_BADURL, should never happen
			return TASK_STATUS_FAIL, 'BAD SERVER URL!!!'
	return TASK_STATUS_SUCCESS, errmsg 

def task_prepareDUTs(DUTCfgList):
	# Check if new image is specified
	print DUTCfgList[-1]
	if not DUTCfgList[-1]['ImagePath']:
		return TASK_STATUS_SUCCESS, 'No impage path specified, no need to prepare DUT'

	# Get rid of the image server url
	DUTCfgList = DUTCfgList[:-1]
	
	# Load the conrresponding images to the DUTs
	dutCnt = 0
	for DUT in DUTCfgList:
		print timestamp(), '\tPreparing DUT #'+str(dutCnt+1)
		telconn = tel_connect(DUT['IP'])
		if not telconn:
			print timestamp(), 'Failed to prepare', DUT['boardID'] + '(' + DUT['IP'] + ')'
			return TASK_STATUS_FAIL, 'Telnet connection failed'

		# if new images are configured to be used
		if 'image' in DUT:
			telconn.write('tftp -g -t i -f ' + DUT['image'] + ' ' + HOST_IP + '\n')  
			print '\tFlashing image file:',DUT['image'], '....',
			telbuff = telconn.read_until('Flashing root', 30)

			# Wait for the DUT to flash the image
			time.sleep(40)
			telconn = tel_connect(DUT['IP'])
			retry = 0
			while not telconn:
				time.sleep(5)
				#print 'Retry connect to', DUT['IP']
				telconn = tel_connect(DUT['IP'])
				retry += 1
			if retry > 15:
				return TASK_STATUS_FAIL, 'Failed connecting to DUT (',DUT['IP'],\
													') after flashing image.'
			else:
				print '\tDone flashing.'
			time.sleep(20) # Wait until the DUT's fully booted

		# Erase the persistent data
		print timestamp(), '\tErasing persistent memory ....',
		telconn.write('restoredefault\n')
		time.sleep(60)
		telconn = tel_connect('192.168.1.1')
		retry = 0
		while not telconn:
			retry += 1
			if retry == 20:
				print timestamp(), 'Failed to prepare', DUT['boardID'] + '(' + DUT['IP'] + ')'
				return TASK_STATUS_FAIL, 'Telnet connection failed'
			time.sleep(5)
			# print 'Retry connect to DUT#'+str(dutCnt + 1)
			telconn = tel_connect('192.168.1.1')
		print '\tDone erasing.'

		# Switch to temporary IP to avoid conflict
		new_IP = '192.168.1.' + str(255 - dutCnt - 1)
		# print 'NewIP should be', new_IP
		print timestamp(), '\tSetting temporary IP', new_IP
		telconn.write('lan config --ipaddr primary ' + new_IP + \
			' 255.255.255.0\r\nsave\r\nreboot\r\n')
		dutCnt += 1

	time.sleep(60)
	
	# Restore the IP address to 192.168.1.1, 192.168.1.2, .....
	print timestamp(), '\tRestoring DUT IP addresses...',
	dutCnt = 0
	for DUT in DUTCfgList:
		new_IP = '192.168.1.'+ str(255 - dutCnt - 1)
		
		telconn = tel_connect(new_IP)
		if not telconn:
			print timestamp(), 'Failed to prepare', DUT['boardID'] + '(' + DUT['IP'] + ')'
			return TASK_STATUS_FAIL, 'Telnet connection failed'
		time.sleep(5)
		# Send at once, or IP's changed
		telconn.write('lan config --ipaddr primary ' + DUT['IP'] + \
			' 255.255.255.0\r\nsave\r\nvoice set defaults\nvoice save\n')
		time.sleep(10)
		dutCnt += 1
	
	# Start the voice with the desired IPs
	print timestamp(), 'DONE restoring.'
	print timestamp(), '\tStarting voice on:'
	dutCnt = 0
	for DUT in DUTCfgList:
		print timestamp(), '\tDUT #'+str(dutCnt+1)
		telconn = tel_connect(DUT['IP'])
		if not telconn:
			print timestamp(), 'Failed to prepare', DUT['boardID'] + '(' + DUT['IP'] + ')'
			return TASK_STATUS_FAIL, 'Telnet connection failed'
		telconn.write('voice stop\n')
		telbuff = telconn.read_until('>')
		time.sleep(15)
		telconn.write('voice start\n')
		time.sleep(10)
		#TODO: verification
		dutCnt += 1
	print timestamp(), '\tDUTs are ready for testing.'
	return TASK_STATUS_SUCCESS, ''
	
			
def gen_phonebookXml(DUTList):
	# Generate temporary phonebook xml for the TCL script
	PhonebookStr = 'set phonebook {\n'
	PhonebookStr += PHONEBOOK_PREFIX
	for DUT in DUTList:
		# Get the last byte of the IP
		IP = int(DUT['IP'].split('.')[-1])
		for acct in range(int(DUT['From']), int(DUT['To'])+1):
			phonenum = acct * 1000 + IP
			PhonebookStr += '\t\t<address>\n\t\t\t<phone>' + str(phonenum) + \
					'</phone>\n\t\t\t<cid-phone>'+ str(phonenum) + \
					'</cid-phone>\n\t\t\t<cid-name></cid-name>\n\t\t</address>\n'
	PhonebookStr += PHONEBOOK_SUFFIX
	PhonebookStr += '\n}\n'

	# Open file and write the phonebook string
	try:
		pbXml = open(PHONEBOOK_FILENAME, 'w')
		pbXml.write(PhonebookStr)
		pbXml.close()
		return pbXml
	except:
		print timestamp(), 'Cannot open temporary phonebook xml file.'
		return None
	

def task_runTest(TestCase):
	TestExecutionInfo = TestCase[-1]
	TestCase = TestCase[:-1]

	# Construct the report directory suffix, eg. SMP96362GWV_SMP96368GWV 
	TestExecutionInfo['DirSuffix'] = '' 
	for DUT in TestCase:
		TestExecutionInfo['DirSuffix'] += '_' + DUT['boardID'] 

	if 'TCLSCRIPT' in TestExecutionInfo:
		print timestamp(), '\tRun TCL test script, voice cli files are used as pre-test board configuration'
		pbxml_fh = gen_phonebookXml(TestCase)
		
		# Determine if there is CLI file supplied.
		# If there is CLI files, run number of CLI sets times
		runOnce = False
		CLIFiles = []
		for DUT in TestCase:
			if 'CLIFile' in DUT:
				CLIFiles.append(DUT['CLIFile'])
			else:
				runOnce = True

		# Construct CLI command lists from CLI xml files
		BoardCLIList = []
		for clifile in CLIFiles:
			root = ElementTree(file=clifile)
			cmd_iter = root.getiterator()
			cmdSets = []
			for cmdSet in cmd_iter:
				if cmdSet.getchildren():
					for child in cmdSet:
						cmd = {}
						# print child.get('Cmdname')
						cmd['Cmdname'] = child.get('Cmdname')
						cmd['Cmd'] = child.text
						cmdSets.append(cmd)
			BoardCLIList.append(cmdSets)
	
		if runOnce:
			total_numRuns = 1
		else:
			total_numRuns = len(BoardCLIList[0]) # number of CLI sets 

		for numRuns in range(total_numRuns):
			time.sleep(10)
			# Send voice cli commands to the DUTs
			CurrSuffix = ''
			if (len(BoardCLIList) > 0):
				DutIx = 0
				# Send the CLI command sets to the DUTs
				for DUT in TestCase:
					print timestamp(), '\tDUT#'+ str(DutIx+1)+ '@'+ DUT['IP'], 'Running command set:',\
						 BoardCLIList[DutIx][numRuns]['Cmdname']
					telconn = tel_connect(DUT['IP'])
					if not telconn:
						print timestamp(), 'Failed to send voice CLI commands', DUT['boardID'] + '(' + DUT['IP'] + ')'
						return TASK_STATUS_FAIL, 'Telnet connection failed'

					# Sends the numRuns command set
					cmdlist = BoardCLIList[DutIx][numRuns]['Cmd'].split('\n')
					for cmd in cmdlist:
						print timestamp()+'\t'+ DUT['IP']+'>', cmd
						telconn.write(cmd+'\n')
						time.sleep(10)
					CurrSuffix += '_' + BoardCLIList[DutIx][numRuns]['Cmdname'] # for the current result directory
					DutIx += 1

			# Run tcl script as subprocess and poll its status
			Dirname = TestExecutionInfo['TestName'] + '_' + strftime('%Y%m%d-%H-%M') + TestExecutionInfo['DirSuffix']+CurrSuffix
			print Dirname
			print timestamp(), '\tCreating tcl script process'
			if os.name == 'nt': # windows
				tclexe = 'C:\\Tcl\\bin\\tclsh.exe '
			else:        # unix-likes are 'posix'
				tclexe = './'
			print tclexe
			print TestExecutionInfo['TCLSCRIPT']
			tclProcess = subprocess.Popen([tclexe+TestExecutionInfo['TCLSCRIPT'],\
							 TestExecutionInfo['AbacusEnv'],\
							 TestExecutionInfo['Duration_HR'],\
							 TestExecutionInfo['Duration_MIN'],\
							 TestExecutionInfo['ReportTemplate'],\
							 PHONEBOOK_FILENAME,\
							 Dirname], shell=True)

			status = TASK_STATUS_SUCCESS
			errmsg = ''
			if tclProcess:
				# Poll the process status
				while status == TASK_STATUS_SUCCESS:
					returncode = tclProcess.poll()
					if returncode == 0:
						status = TASK_STATUS_SUCCESS
						errmsg = 'Tcl script done execution without error.'
						break;
					elif returncode == 1:
						status = TASK_STATUS_FAIL
						errmsg = 'Tcl script failed and terminated.'
					elif returncode == 2:
						status = TASK_STATUS_FAIL
						errmsg = 'Abacus test eneded with errors, please check report'
					time.sleep(15)
		# Remember the test report directories for notification
		global ReportDirList
		ReportDirList.append(Dirname)	
		return status, errmsg
	else:
		print timestamp(), 'Run voice cli only (NOT IMPLEMENTED)'
	return TASK_STATUS_SUCCESS, ''


def main(argv):

	# boundary checks may be removed in the future as it is not neccessary
	HOUR = 1
	MIN = 0
	IMG_BASEDIR_temp1 = 'C:\\automated_test_demo\\images\\'  # you may change the default path for image storage here 

	print '\n\nDefault configuration:'
	print '\n          Start time =   1 : 00'
	print '\n          Downloaded image directory =   ' + IMG_BASEDIR_temp1
	print '\n\nRun with default configuration?   Y / N'
	config_skip = raw_input()
	while config_skip != 'N' and config_skip != 'Y': # boundary check
		print 'ERROR: Invalid input'
		config_skip = raw_input()

	while config_skip == 'N':
		print '\n\n          ========== CONFIG MENU =========='
		print '\n          1. Start time:  ', HOUR,':','%02.0F'% MIN
		print '\n          2. Downloaded image directory:  ', IMG_BASEDIR_temp1
		print '\n          3. Reset to default'
		print '\n          4. Save and exit\n\n'
		print 'Select an option from the above'
		config_selection = raw_input()
		while config_selection != '1' and config_selection != '2' and config_selection != '3' and config_selection != '4':
		# wasteful implementation but safe
		# consider modulus method if necessary in the future
			print 'ERROR: Invalid input'
			config_selection = raw_input()
		
		if config_selection == '1':
			HOUR, MIN = time_config()
		elif config_selection == '2':
			IMG_BASEDIR_temp1 = store_img_dir_config()
		elif config_selection == '3':
			HOUR, MIN, IMG_BASEDIR_temp1 = reset_config()
		elif config_selection == '4':
			config_skip = exit_config()
			
	if config_skip == 'Y':
		print '\nConfiguration loaded, preparing to start...\n'


	# Check for email address
	emailaddr = None
	if len(argv) == 2:
		emailaddr = argv[1]
		print 'Test result will be sent to ', emailaddr

	# Retrieve the test configurations
	global TestCases_DUTCfgLists
	TestCases_DUTCfgLists = get_testcfg_info(argv[0])
	if not TestCases_DUTCfgLists:
		print 'Invalid test configuration file'
	
	# Report a summary of the test suite specified in the
	# test configuration xml file
	testsuite_summary(TestCases_DUTCfgLists)

	# Run forever
	while 1:
		TestCaseCnt = 1
		emailmsg = ''
		for TestCase in TestCases_DUTCfgLists:
			# Check if it's time to start testing 
			Start = False
			# Rpeatable test
			while not Start and TestCase[-1]['Repeat']: 
				dt = list(time.localtime())
				if dt[3] == HOUR and dt[4] == MIN:
					Start = True
				else:
					# print timestamp(), 'Waiting to run'
					print '.',
					time.sleep(10)

			# Start testing if it's not a repatable test or time's up
			print timestamp(), TestCase[-1]['TestName'], 'Testing start'
			send_notification(emailaddr, 'Test started', [])
			status = TASK_STATUS_SUCCESS
			errmsg = ''
			
			# Retrieve image
			retry = 0
			status = TASK_STATUS_RETRY
			while status == TASK_STATUS_RETRY:
				print timestamp(), 'task_prepareImg():', 'retrieving images'
				status, errmsg = task_prepareImg(TestCase, IMG_BASEDIR_temp1)
				if status == TASK_STATUS_RETRY:
					if retry == 5:
						print timestamp(), 'task_prepareImg(): Retried 5 times, giving up'
						emailmsg += 'Task_prepareImg():' + errmsg + '\n'
						send_notification(emailaddr, emailmsg, [])
						status = TASK_STATUS_FAIL
						break
					print timestamp(), 'task_prepareImg():', errmsg
					print timestamp(), 'task_prepareImg():', 'retry', str(retry+1), 'after 5 mins later'
					time.sleep(300)
					retry += 1
			# Give up, and wait for next time
			if status == TASK_STATUS_FAIL:
				time.sleep(60)
				break		

			# Prepare DUTs
			status, errmsg = task_prepareDUTs(TestCase)
			if not status == TASK_STATUS_SUCCESS:
				print timestamp(), 'task_prepareDUTs():', errmsg
				emailmsg += 'Task_prepareDuTs:' + errmsg + '\n'
				send_notification(emailaddr, emailmsg, [])
				break

			# Start Abacus test
			status, errmsg = task_runTest(TestCase)
			if not status == TASK_STATUS_SUCCESS:
				print timestamp(), 'task_runTest():', errmsg
				dt = list(time.localtime())
				emailmsg += '\nTask_runTest:'+ errmsg + str(dt)
				# fall through
			
			# Test terminate, gather information and send out 
			# notification if email address is specified.
			global ReportDirList
			if emailaddr:
				emailmsg += '\n Please see the attached report'
				send_notification(emailaddr, emailmsg, ReportDirList)			
			
			if not TestCase[-1]['Repeat']:
				exit(0)

if __name__ == '__main__':
	if len(sys.argv[1:]) > 2 or len(sys.argv[1:]) < 1:
		print 'Usage: python nightly.py <testcfg_filename> <email_address>' 
		sys.exit(1)
	main(sys.argv[1:])
